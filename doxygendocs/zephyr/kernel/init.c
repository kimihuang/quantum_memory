/**
 * @file init.c
 * @brief Zephyr 内核初始化模块
 *
 * 该模块是 Zephyr RTOS 内核的核心初始化入口，包含从 C 运行环境准备
 * 完成后到用户应用 main() 函数执行之前的全部初始化流程。
 *
 * 主要函数及调用关系：
 *   z_cstart()                  -- 由 prep_c 调用，内核 C 入口
 *     ├── z_sys_init_run_level(EARLY)
 *     ├── arch_kernel_init()
 *     ├── z_device_state_init()
 *     ├── soc_early_init_hook() / board_early_init_hook()
 *     ├── z_sys_init_run_level(PRE_KERNEL_1)
 *     ├── z_sys_init_run_level(PRE_KERNEL_2)
 *     └── switch_to_main_thread(prepare_multithreading())
 *           └── bg_thread_main()    -- main 线程入口
 *                 ├── z_mem_manage_init()          [MMU]
 *                 ├── z_sys_init_run_level(POST_KERNEL)
 *                 ├── soc_late_init_hook() / board_late_init_hook()
 *                 ├── z_init_static_threads()
 *                 ├── z_smp_init()                  [SMP]
 *                 └── main()
 *
 * 初始化级别（init_level）按以下顺序依次执行：
 *   EARLY → PRE_KERNEL_1 → PRE_KERNEL_2 → POST_KERNEL → APPLICATION → [SMP]
 *
 * @note PRE_KERNEL_1/2 在调度器启动前执行，不可使用阻塞 API。
 * @note POST_KERNEL 及之后的级别在调度器运行后执行。
 *
 * @copyright Copyright (c) 2010-2014 Wind River Systems, Inc.
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <offsets_short.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/debug/stack.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <zephyr/toolchain.h>
#include <zephyr/kernel_structs.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/platform/hooks.h>
#include <ksched.h>
#include <kthread.h>
#include <ipi.h>
#include <zephyr/sys/dlist.h>
#include <kernel_internal.h>
#include <zephyr/drivers/entropy.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/debug/gcov.h>
#include <kswap.h>
#include <zephyr/timing/timing.h>
#include <zephyr/logging/log.h>
#include <zephyr/internal/syscall_handler.h>
#include <zephyr/arch/common/init.h>
#include <scheduler.h>

/** @brief 内核日志模块注册 */
LOG_MODULE_REGISTER(os, CONFIG_KERNEL_LOG_LEVEL);

/** @brief 全局唯一的内核数据结构实例 */
__pinned_bss
struct z_kernel _kernel;

#ifdef CONFIG_PM
/** @brief 当前活跃 CPU 数量（电源管理子系统使用） */
__pinned_bss atomic_t _cpus_active;
#endif

/*
 * main 线程和 idle 线程的栈与线程控制块
 */

/** @brief main 线程栈定义 */
K_THREAD_PINNED_STACK_DEFINE(z_main_stack, CONFIG_MAIN_STACK_SIZE);
/** @brief main 线程控制块 */
struct k_thread z_main_thread;

#ifdef CONFIG_MULTITHREADING
/** @brief 各 CPU 的 idle 线程控制块数组 */
__pinned_bss
struct k_thread z_idle_threads[CONFIG_MP_MAX_NUM_CPUS];

/** @brief 各 CPU 的 idle 线程栈数组 */
static K_KERNEL_PINNED_STACK_ARRAY_DEFINE(z_idle_stacks,
					  CONFIG_MP_MAX_NUM_CPUS,
					  CONFIG_IDLE_STACK_SIZE);

/**
 * @brief 初始化所有静态定义的线程
 *
 * 遍历 linker section 中所有 _static_thread_data 条目，调用 z_setup_new_thread()
 * 完成线程创建。在 userspace 模式下，还会处理 k_object_assignment 条目，
 * 将指定内核对象授权给对应线程。
 *
 * 静态线程可能设置了启动延迟（K_FOREVER 表示延迟启动，即 legacy 线程）。
 * 非延迟线程会立即被调度执行。为防止线程在遍历过程中被抢占执行，
 * 整个启动过程在调度器锁定期间完成。
 */
static void z_init_static_threads(void)
{
	STRUCT_SECTION_FOREACH(_static_thread_data, thread_data) {
		z_setup_new_thread(
			thread_data->init_thread,
			thread_data->init_stack,
			thread_data->init_stack_size,
			thread_data->init_entry,
			thread_data->init_p1,
			thread_data->init_p2,
			thread_data->init_p3,
			thread_data->init_prio,
			thread_data->init_options,
			thread_data->init_name);

		thread_data->init_thread->init_data = thread_data;
	}

#ifdef CONFIG_USERSPACE
	/* 处理 userspace 对象授权：将内核对象访问权限授予指定线程 */
	STRUCT_SECTION_FOREACH(k_object_assignment, pos) {
		for (int i = 0; pos->objects[i] != NULL; i++) {
			k_object_access_grant(pos->objects[i],
					      pos->thread);
		}
	}
#endif /* CONFIG_USERSPACE */

	/*
	 * 非延迟静态线程可以立即启动或按指定延迟启动。
	 * 即使调度器已锁定，tick 仍可递增和处理。
	 * 加锁以防止线程在全部注册完成前被调度执行。
	 *
	 * 注意：使用 legacy API 定义的静态线程延迟为 K_FOREVER，
	 * 即不会在此处启动。
	 */
	k_sched_lock();
	STRUCT_SECTION_FOREACH(_static_thread_data, thread_data) {
		k_timeout_t init_delay = Z_THREAD_INIT_DELAY(thread_data);

		if (!K_TIMEOUT_EQ(init_delay, K_FOREVER)) {
			thread_schedule_new(thread_data->init_thread,
					    init_delay);
		}
	}
	k_sched_unlock();
}
#else
#define z_init_static_threads() do { } while (false)
#endif /* CONFIG_MULTITHREADING */

/*
 * 各初始化级别的 linker symbol，由链接脚本按优先级排序生成。
 * 每个级别的 init_entry 条目按其优先级从高到低排列在对应的 section 中。
 */
extern const struct init_entry __init_start[];
extern const struct init_entry __init_EARLY_start[];
extern const struct init_entry __init_PRE_KERNEL_1_start[];
extern const struct init_entry __init_PRE_KERNEL_2_start[];
extern const struct init_entry __init_POST_KERNEL_start[];
extern const struct init_entry __init_APPLICATION_start[];
extern const struct init_entry __init_end[];

/**
 * @brief 内核初始化级别枚举
 *
 * 定义系统初始化各阶段的执行顺序。每个级别对应一组通过
 * SYS_INIT() / INIT_ENTRY_DEFINE() 宏注册的初始化函数，
 * 链接脚本确保同级别内按优先级从高到低执行。
 */
enum init_level {
	INIT_LEVEL_EARLY = 0,          /**< 最早期的硬件初始化 */
	INIT_LEVEL_PRE_KERNEL_1,       /**< 内核子系统初始化第一阶段（调度器未启动） */
	INIT_LEVEL_PRE_KERNEL_2,       /**< 内核子系统初始化第二阶段（调度器未启动） */
	INIT_LEVEL_POST_KERNEL,        /**< 内核子系统初始化完成（调度器已启动） */
	INIT_LEVEL_APPLICATION,        /**< 应用层初始化 */
#ifdef CONFIG_SMP
	INIT_LEVEL_SMP,                /**< SMP 多核启动 */
#endif /* CONFIG_SMP */
};

#ifdef CONFIG_SMP
extern const struct init_entry __init_SMP_start[];
#endif /* CONFIG_SMP */

/*
 * 中断栈存储区域。
 *
 * 注意：该区域在内核初始化期间同时作为系统栈使用，
 * 因为此时内核尚未建立自己的栈区域。
 * 该区域的复用是安全的，因为在中断被禁用期间（直到内核
 * 上下文切换到 init 线程之前）不会有中断使用此栈。
 */
K_KERNEL_PINNED_STACK_ARRAY_DEFINE(z_interrupt_stacks,
				   CONFIG_MP_MAX_NUM_CPUS,
				   CONFIG_ISR_STACK_SIZE);

/** @brief idle 线程入口函数声明（在 sched.c 中定义） */
extern void idle(void *unused1, void *unused2, void *unused3);

#ifdef CONFIG_OBJ_CORE_SYSTEM
/** @brief CPU 对象类型描述 */
static struct k_obj_type obj_type_cpu;
/** @brief 内核对象类型描述 */
static struct k_obj_type obj_type_kernel;

#ifdef CONFIG_OBJ_CORE_STATS_SYSTEM
/**
 * @brief CPU 统计信息描述符
 *
 * @details 提供 CPU cycle 统计的 raw 和 query 接口，
 *          用于追踪 CPU 执行统计信息。
 */
static struct k_obj_core_stats_desc  cpu_stats_desc = {
	.raw_size = sizeof(struct k_cycle_stats),
	.query_size = sizeof(struct k_thread_runtime_stats),
	.raw   = z_cpu_stats_raw,
	.query = z_cpu_stats_query,
	.reset = NULL,
	.disable = NULL,
	.enable  = NULL,
};

/**
 * @brief 内核统计信息描述符
 *
 * @details 提供全内核 cycle 统计的 raw 和 query 接口。
 */
static struct k_obj_core_stats_desc  kernel_stats_desc = {
	.raw_size = sizeof(struct k_cycle_stats) * CONFIG_MP_MAX_NUM_CPUS,
	.query_size = sizeof(struct k_thread_runtime_stats),
	.raw   = z_kernel_stats_raw,
	.query = z_kernel_stats_query,
	.reset = NULL,
	.disable = NULL,
	.enable  = NULL,
};
#endif /* CONFIG_OBJ_CORE_STATS_SYSTEM */
#endif /* CONFIG_OBJ_CORE_SYSTEM */

#ifdef CONFIG_REQUIRES_STACK_CANARIES
#ifdef CONFIG_STACK_CANARIES_TLS
/** @brief 栈保护金丝雀值（TLS 存储，每线程独立） */
extern Z_THREAD_LOCAL volatile uintptr_t __stack_chk_guard;
#else
/** @brief 栈保护金丝雀值（全局共享） */
extern volatile uintptr_t __stack_chk_guard;
#endif /* CONFIG_STACK_CANARIES_TLS */
#endif /* CONFIG_REQUIRES_STACK_CANARIES */

/** @brief 标记 POST_KERNEL 阶段已完成，用于条件编译检查 */
__pinned_bss
bool z_sys_post_kernel;

/** @brief 设备初始化函数（在 device.c 中实现） */
extern int do_device_init(const struct device *dev);

/**
 * @brief 初始化所有静态设备的状态对象
 *
 * 遍历 linker section 中所有 device 实例，调用 k_object_init()
 * 初始化其内核对象状态。设备状态对象在 .bss 中被零初始化，
 * 但某些场景下需要额外的初始化处理。
 */
static void z_device_state_init(void)
{
	STRUCT_SECTION_FOREACH(device, dev) {
		k_object_init(dev);
	}
}

/**
 * @brief 执行指定初始化级别的所有初始化条目
 *
 * 遍历由链接脚本按优先级排序的 init_entry 数组，依次调用每个
 * 条目的初始化函数。每个条目可能关联一个 device（通过 dev 字段），
 * 或仅包含一个 init_fn 纯函数。
 *
 * 对于关联了 device 的条目，若设备设置了 DEVICE_FLAG_INIT_DEFERRED
 * 标志，则跳过在此处初始化（通常由驱动模型在稍后阶段处理）。
 *
 * @param[in] level 要执行的初始化级别
 *
 * @see INIT_ENTRY_DEFINE()  注册初始化条目的宏
 * @see SYS_INIT()           便捷的设备初始化注册宏
 */
static void z_sys_init_run_level(enum init_level level)
{
	/** 各级别在 linker section 中的起始位置 */
	static const struct init_entry *levels[] = {
		__init_EARLY_start,
		__init_PRE_KERNEL_1_start,
		__init_PRE_KERNEL_2_start,
		__init_POST_KERNEL_start,
		__init_APPLICATION_start,
#ifdef CONFIG_SMP
		__init_SMP_start,
#endif /* CONFIG_SMP */
		/* 结束标记 */
		__init_end,
	};
	const struct init_entry *entry;

	for (entry = levels[level]; entry < levels[level+1]; entry++) {
		const struct device *dev = entry->dev;
		int result = 0;

		sys_trace_sys_init_enter(entry, level);
		if (dev != NULL) {
			if ((dev->flags & DEVICE_FLAG_INIT_DEFERRED) == 0U) {
				result = do_device_init(dev);
			}
		} else {
			result = entry->init_fn();
		}
		sys_trace_sys_init_exit(entry, level, result);
	}
}

#ifdef CONFIG_STATIC_INIT_GNU

/**
 * @brief GNU 构造器风格的静态初始化
 *
 * 遍历 __zephyr_init_array 中的函数指针数组并依次调用，
 * 用于支持 GNU 工具链的 __attribute__((constructor)) 语义。
 *
 * @note MWDT 工具链会在数组末尾放置 NULL 哨兵值。
 */
extern void (*__zephyr_init_array_start[])();
extern void (*__zephyr_init_array_end[])();

static void z_static_init_gnu(void)
{
	void	(**fn)();

	for (fn = __zephyr_init_array_start; fn != __zephyr_init_array_end; fn++) {
		/* MWDT 工具链在数组末尾放置 NULL */
		if (*fn == NULL) {
			break;
		}
		(**fn)();
	}
}

#endif

/**
 * @brief 内核 main 线程入口函数
 *
 * 这是 Zephyr 内核 main 线程的主体函数，在调度器启动后执行。
 * 按顺序完成以下工作：
 *   1. 内存管理初始化（MMU 模式下）
 *   2. 设置 POST_KERNEL 完成标志
 *   3. 执行 POST_KERNEL 级别初始化
 *   4. 调用 SoC/Board 延迟初始化 hook
 *   5. 启用栈指针随机化
 *   6. 执行 APPLICATION 级别初始化
 *   7. 初始化并启动所有静态定义的线程
 *   8. SMP 多核初始化（若启用）
 *   9. 调用用户应用 main() 函数
 *  10. main() 返回后标记线程为非核心线程
 *  11. 导出覆盖率数据（若启用）
 *
 * @param[in] unused1 未使用参数
 * @param[in] unused2 未使用参数
 * @param[in] unused3 未使用参数
 */
__boot_func
static void bg_thread_main(void *unused1, void *unused2, void *unused3)
{
	ARG_UNUSED(unused1);
	ARG_UNUSED(unused2);
	ARG_UNUSED(unused3);

#ifdef CONFIG_MMU
	/*
	 * 内存管理子系统在此初始化，以便 backing store 或驱逐算法
	 * 可以初始化内核对象，同时允许 POST_KERNEL 及之后的所有任务
	 * 执行内存管理操作（k_mem_map_phys_bare() 除外，它随时可用）。
	 */
	z_mem_manage_init();
#endif /* CONFIG_MMU */
	z_sys_post_kernel = true;

#if CONFIG_IRQ_OFFLOAD
	arch_irq_offload_init();
#endif
	z_sys_init_run_level(INIT_LEVEL_POST_KERNEL);

	soc_late_init_hook();

	board_late_init_hook();

#if defined(CONFIG_STACK_POINTER_RANDOM) && (CONFIG_STACK_POINTER_RANDOM != 0)
	z_stack_adjust_initialized = 1;
#endif /* CONFIG_STACK_POINTER_RANDOM */

#ifdef CONFIG_STATIC_INIT_GNU
	z_static_init_gnu();
#endif /* CONFIG_STATIC_INIT_GNU */

	/* 应用层初始化（用户 main() 之前的最后一个 init level） */
	z_sys_init_run_level(INIT_LEVEL_APPLICATION);

	z_init_static_threads();

#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(sys_cache_is_mem_coherent(&_kernel));
#endif /* CONFIG_KERNEL_COHERENCE */

#ifdef CONFIG_SMP
	if (!IS_ENABLED(CONFIG_SMP_BOOT_DELAY)) {
		z_smp_init();
	}
	z_sys_init_run_level(INIT_LEVEL_SMP);
#endif /* CONFIG_SMP */

#ifdef CONFIG_MMU
	z_mem_manage_boot_finish();
#endif /* CONFIG_MMU */

#ifdef CONFIG_BOOTARGS
	extern int main(int, char **);
	extern char **sys_boot_prepare_main_args(int *argc);

	int argc = 0;
	char **argv = sys_boot_prepare_main_args(&argc);
	(void)main(argc, argv);
#else
	extern int main(void);

	(void)main();
#endif /* CONFIG_BOOTARGS */

	/* main() 已完成，标记 main 线程为非核心线程 */
	z_thread_essential_clear(&z_main_thread);

#ifdef CONFIG_COVERAGE_DUMP
	/* main() 退出后导出覆盖率数据 */
	gcov_coverage_dump();
#elif defined(CONFIG_COVERAGE_SEMIHOST)
	gcov_coverage_semihost();
#endif /* CONFIG_COVERAGE_DUMP */
} /* LCOV_EXCL_LINE ... 覆盖率数据已在上方导出 */

#if defined(CONFIG_MULTITHREADING)
/**
 * @brief 创建指定 CPU 的 idle 线程
 *
 * 为 CPU i 创建 idle 线程，以最低优先级（K_IDLE_PRIO）运行，
 * 标记为核心线程（K_ESSENTIAL），线程入口为 idle() 函数。
 *
 * @param[in] i CPU 编号
 */
__boot_func
static void init_idle_thread(int i)
{
	struct k_thread *thread = &z_idle_threads[i];
	k_thread_stack_t *stack = z_idle_stacks[i];
	size_t stack_size = K_KERNEL_STACK_SIZEOF(z_idle_stacks[i]);

#ifdef CONFIG_THREAD_NAME

#if CONFIG_MP_MAX_NUM_CPUS > 1
	char tname[8];
	snprintk(tname, 8, "idle %02d", i);
#else
	char *tname = "idle";
#endif /* CONFIG_MP_MAX_NUM_CPUS */

#else
	char *tname = NULL;
#endif /* CONFIG_THREAD_NAME */

	z_setup_new_thread(thread, stack,
			  stack_size, idle, &_kernel.cpus[i],
			  NULL, NULL, K_IDLE_PRIO, K_ESSENTIAL,
			  tname);
	z_mark_thread_as_not_sleeping(thread);

#ifdef CONFIG_SMP
	thread->base.is_idle = 1U;
#endif /* CONFIG_SMP */
}

/**
 * @brief 初始化指定 CPU 的内核数据
 *
 * 完成单个 CPU 的 idle 线程创建、中断栈设置、CPU ID 分配、
 * 电源管理计数更新、对象核心系统注册和 IPI 工作队列初始化。
 *
 * @param[in] id CPU 编号
 */
void z_init_cpu(int id)
{
	init_idle_thread(id);
	_kernel.cpus[id].idle_thread = &z_idle_threads[id];
	_kernel.cpus[id].id = id;
	_kernel.cpus[id].irq_stack =
		(K_KERNEL_STACK_BUFFER(z_interrupt_stacks[id]) +
		 K_KERNEL_STACK_SIZEOF(z_interrupt_stacks[id]));
#ifdef CONFIG_SCHED_THREAD_USAGE_ALL
	_kernel.cpus[id].usage = &_kernel.usage[id];
	_kernel.cpus[id].usage->track_usage =
		CONFIG_SCHED_THREAD_USAGE_AUTO_ENABLE;
#endif

#ifdef CONFIG_PM
	/*
	 * 递增活跃 CPU 计数。电源管理子系统从此处开始跟踪。
	 */
	atomic_inc(&_cpus_active);
#endif

#ifdef CONFIG_OBJ_CORE_SYSTEM
	k_obj_core_init_and_link(K_OBJ_CORE(&_kernel.cpus[id]), &obj_type_cpu);
#ifdef CONFIG_OBJ_CORE_STATS_SYSTEM
	k_obj_core_stats_register(K_OBJ_CORE(&_kernel.cpus[id]),
				  _kernel.cpus[id].usage,
				  sizeof(struct k_cycle_stats));
#endif
#endif

#ifdef CONFIG_SCHED_IPI_SUPPORTED
	sys_dlist_init(&_kernel.cpus[id].ipi_workq);
#endif
}

/**
 * @brief 准备多线程运行环境
 *
 * 初始化内核调度器、创建 main 线程、初始化 CPU 0 的 idle 线程
 * 及其中断栈。在单核模式下，预先将 main 线程加载到调度器优先级缓存中，
 * 因为它将是第一个运行的线程。
 *
 * @return main 线程的初始栈指针，用于上下文切换
 *
 * @note 调用此函数时 _kernel 结构体的所有字段为零（.bss 已清零），
 *       对许多字段而言零初始化已足够。
 */
__boot_func
static char *prepare_multithreading(void)
{
	char *stack_ptr;

	/* _kernel.ready_q 已全部清零 */
	z_sched_init();

#ifndef CONFIG_SMP
	/*
	 * 预先在调度器缓存中加载 main 线程，因为：
	 *   - 调度器缓存不允许为 NULL
	 *   - main 线程将第一个被调度执行
	 *   - 此时无其他线程初始化，其优先级字段含有垃圾值，
	 *     会干扰缓存加载算法
	 */
	_kernel.ready_q.cache = &z_main_thread;
#endif /* CONFIG_SMP */
	stack_ptr = z_setup_new_thread(&z_main_thread, z_main_stack,
				       K_THREAD_STACK_SIZEOF(z_main_stack),
				       bg_thread_main,
				       NULL, NULL, NULL,
				       CONFIG_MAIN_THREAD_PRIORITY,
				       K_ESSENTIAL, "main");
	z_mark_thread_as_not_sleeping(&z_main_thread);
	z_ready_thread(&z_main_thread);

	z_init_cpu(0);

	return stack_ptr;
}

/**
 * @brief 上下文切换到 main 线程
 *
 * 从当前伪线程（启动上下文）切换到 main 线程。
 * 伪线程不在任何等待队列或就绪队列上，因此切换后永远不会被重新调度。
 *
 * @param[in] stack_ptr main 线程的初始栈指针
 */
__boot_func
static FUNC_NORETURN void switch_to_main_thread(char *stack_ptr)
{
#ifdef CONFIG_ARCH_HAS_CUSTOM_SWAP_TO_MAIN
	arch_switch_to_main_thread(&z_main_thread, stack_ptr, bg_thread_main);
#else
	ARG_UNUSED(stack_ptr);
	/*
	 * 上下文切换到 main 任务（入口函数为 bg_thread_main）：
	 * 当前伪线程不在任何等待队列或就绪队列上，
	 * 因此切换后永远不会被重新调度。
	 */
	z_swap_unlocked();
#endif /* CONFIG_ARCH_HAS_CUSTOM_SWAP_TO_MAIN */
	CODE_UNREACHABLE; /* LCOV_EXCL_LINE */
}
#endif /* CONFIG_MULTITHREADING */

/**
 * @brief 获取早期启动阶段的伪随机数
 *
 * 在内核完整初始化之前提供伪随机数据，用于栈保护金丝雀值等
 * 需要在启动早期获取随机数据的场景。
 *
 * 优先尝试使用硬件 entropy 驱动（ISR 模式），若驱动不可用
 * 或返回数据不足，则回退到基于 cycle counter 的线性同余
 * 伪随机数生成器（LCG）。
 *
 * @param[out] buf    输出缓冲区
 * @param[in]  length 需要生成的随机数据字节数
 *
 * @note 此函数使用固定种子的 LCG，安全性不适用于加密用途。
 * @note 函数标记为 __weak，可被平台代码覆盖以使用硬件 TRNG。
 */
__boot_func
void __weak z_early_rand_get(uint8_t *buf, size_t length)
{
	static uint64_t state = (uint64_t)CONFIG_TIMER_RANDOM_INITIAL_STATE;
	int rc;

#ifdef CONFIG_ENTROPY_HAS_DRIVER
	const struct device *const entropy = DEVICE_DT_GET_OR_NULL(DT_CHOSEN(zephyr_entropy));

	if ((entropy != NULL) && device_is_ready(entropy)) {
		/* 尝试使用驱动提供的 ISR 专用 API 获取真随机数 */
		rc = entropy_get_entropy_isr(entropy, buf, length, ENTROPY_BUSYWAIT);
		if (rc > 0) {
			length -= rc;
			buf += rc;
		}
	}
#endif /* CONFIG_ENTROPY_HAS_DRIVER */

	/* 回退到基于 cycle counter 的 LCG 伪随机数生成 */
	while (length > 0) {
		uint32_t val;

		state = state + k_cycle_get_32();
		state = state * 2862933555777941757ULL + 3037000493ULL;
		val = (uint32_t)(state >> 32);
		rc = min(length, sizeof(val));
		arch_early_memcpy((void *)buf, &val, rc);

		length -= rc;
		buf += rc;
	}
}

/**
 * @brief Zephyr 内核 C 入口函数
 *
 * 这是内核从汇编启动代码（经 prep_c）进入后的第一个 C 函数。
 * 处理器必须处于正确的运行模式（32 位），且 .bss 段必须已清零。
 *
 * 按以下顺序执行初始化：
 *   1. gcov 覆盖率静态初始化
 *   2. EARLY 级别系统初始化
 *   3. 架构相关内核初始化
 *   4. 日志核心初始化
 *   5. dummy 线程初始化（多线程模式）
 *   6. 静态设备状态初始化
 *   7. SoC/Board 早期初始化 hook
 *   8. PRE_KERNEL_1 级别初始化
 *   9. SMP 架构初始化（若启用）
 *  10. PRE_KERNEL_2 级别初始化
 *  11. 栈保护金丝雀值初始化
 *  12. timing 功能初始化（若启用）
 *  13. 切换到 main 线程（多线程）或直接调用 bg_thread_main（单线程）
 *
 * @note 此函数不会返回（FUNC_NORETURN）。
 * @note 标记为 FUNC_NO_STACK_PROTECTOR，因为在栈保护金丝雀值
 *       初始化之前调用，此时 __stack_chk_guard 尚未设置。
 */
__boot_func
FUNC_NO_STACK_PROTECTOR
FUNC_NORETURN void z_cstart(void)
{
	/* gcov hook，获取覆盖率报告所需 */
	gcov_static_init();

	/* EARLY 级别系统初始化 */
	z_sys_init_run_level(INIT_LEVEL_EARLY);

	/* 架构相关的内核初始化 */
	arch_kernel_init();

	LOG_CORE_INIT();

#if defined(CONFIG_MULTITHREADING)
	z_dummy_thread_init(&_thread_dummy);
#endif /* CONFIG_MULTITHREADING */
	/* 初始化所有静态设备的状态对象 */
	z_device_state_init();

	soc_early_init_hook();

	board_early_init_hook();

	/* 基础硬件初始化 */
	z_sys_init_run_level(INIT_LEVEL_PRE_KERNEL_1);
#if defined(CONFIG_SMP)
	arch_smp_init();
#endif
	z_sys_init_run_level(INIT_LEVEL_PRE_KERNEL_2);

#ifdef CONFIG_REQUIRES_STACK_CANARIES
	uintptr_t stack_guard;

	z_early_rand_get((uint8_t *)&stack_guard, sizeof(stack_guard));
	__stack_chk_guard = stack_guard;
	__stack_chk_guard <<= 8;
#endif	/* CONFIG_REQUIRES_STACK_CANARIES */

#ifdef CONFIG_TIMING_FUNCTIONS_NEED_AT_BOOT
	timing_init();
	timing_start();
#endif /* CONFIG_TIMING_FUNCTIONS_NEED_AT_BOOT */

#ifdef CONFIG_MULTITHREADING
	switch_to_main_thread(prepare_multithreading());
#else
#ifdef ARCH_SWITCH_TO_MAIN_NO_MULTITHREADING
	/* 自定义架构例程：在无多线程模式下切换到 main() */
	ARCH_SWITCH_TO_MAIN_NO_MULTITHREADING(bg_thread_main,
		NULL, NULL, NULL);
#else
	bg_thread_main(NULL, NULL, NULL);

	/* LCOV_EXCL_START
	 * 覆盖率数据已在上方导出
	 */
	irq_lock();
	while (true) {
	}
	/* LCOV_EXCL_STOP */
#endif /* ARCH_SWITCH_TO_MAIN_NO_MULTITHREADING */
#endif /* CONFIG_MULTITHREADING */

	/*
	 * 编译器无法推断上述函数不会返回，需要显式标记
	 * 以消除警告。
	 */

	CODE_UNREACHABLE; /* LCOV_EXCL_LINE */
}

#ifdef CONFIG_OBJ_CORE_SYSTEM
/**
 * @brief 初始化 CPU 对象核心系统
 *
 * 注册 CPU 对象类型到对象核心框架，并绑定统计信息描述符。
 *
 * @return 0（成功）
 *
 * @note 通过 SYS_INIT() 宏注册到 PRE_KERNEL_1 级别。
 */
static int init_cpu_obj_core_list(void)
{
	/* 初始化 CPU 对象类型 */

	z_obj_type_init(&obj_type_cpu, K_OBJ_TYPE_CPU_ID,
			offsetof(struct _cpu, obj_core));

#ifdef CONFIG_OBJ_CORE_STATS_SYSTEM
	k_obj_type_stats_init(&obj_type_cpu, &cpu_stats_desc);
#endif /* CONFIG_OBJ_CORE_STATS_SYSTEM */

	return 0;
}

/**
 * @brief 初始化内核对象核心系统
 *
 * 注册内核对象类型并初始化全局 _kernel 实例的对象核心链接，
 * 同时绑定内核级统计信息。
 *
 * @return 0（成功）
 *
 * @note 通过 SYS_INIT() 宏注册到 PRE_KERNEL_1 级别。
 */
static int init_kernel_obj_core_list(void)
{
	/* 初始化内核对象类型 */

	z_obj_type_init(&obj_type_kernel, K_OBJ_TYPE_KERNEL_ID,
			offsetof(struct z_kernel, obj_core));

#ifdef CONFIG_OBJ_CORE_STATS_SYSTEM
	k_obj_type_stats_init(&obj_type_kernel, &kernel_stats_desc);
#endif /* CONFIG_OBJ_CORE_STATS_SYSTEM */

	k_obj_core_init_and_link(K_OBJ_CORE(&_kernel), &obj_type_kernel);
#ifdef CONFIG_OBJ_CORE_STATS_SYSTEM
	k_obj_core_stats_register(K_OBJ_CORE(&_kernel), _kernel.usage,
				  sizeof(_kernel.usage));
#endif /* CONFIG_OBJ_CORE_STATS_SYSTEM */

	return 0;
}

/** @brief 注册 CPU 对象核心初始化到 PRE_KERNEL_1 级别 */
SYS_INIT(init_cpu_obj_core_list, PRE_KERNEL_1,
	 CONFIG_KERNEL_INIT_PRIORITY_OBJECTS);

/** @brief 注册内核对象核心初始化到 PRE_KERNEL_1 级别 */
SYS_INIT(init_kernel_obj_core_list, PRE_KERNEL_1,
	 CONFIG_KERNEL_INIT_PRIORITY_OBJECTS);
#endif /* CONFIG_OBJ_CORE_SYSTEM */
