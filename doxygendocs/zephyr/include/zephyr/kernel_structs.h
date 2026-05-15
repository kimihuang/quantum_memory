/**
 * @file kernel_structs.h
 * @brief Zephyr 内核核心数据结构定义
 *
 * 本文件定义了 Zephyr 内核中最基础、最关键的数据结构，包括：
 *   - 线程状态位掩码（_THREAD_xxx）
 *   - 调度器就绪队列（_ready_q 及其三种实现）
 *   - CPU 核心数据结构（_cpu）
 *   - 全局内核数据结构（z_kernel / _kernel）
 *   - 等待队列（_wait_q_t）
 *   - 超时机制（_timeout）
 *
 * 设计约束：
 *   1. 本文件不得依赖 kernel.h（直接或间接），以便其他头文件
 *      可以在不拉入完整内核依赖的情况下使用这些结构体。
 *   2. kernel.h 会隐式包含本文件，使用者无需显式 include。
 *
 * @note 本文件同时被 C 代码和汇编代码引用（通过 _ASMLANGUAGE 宏区分），
 *       因此结构体布局和偏移量在修改时需谨慎。
 *
 * @copyright Copyright (c) 2016 Wind River Systems, Inc.
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_
#define ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_

#if !defined(_ASMLANGUAGE)
#include <zephyr/sys/atomic.h>
#include <zephyr/types.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/sys_heap.h>
#include <zephyr/arch/structs.h>
#include <zephyr/kernel/stats.h>
#include <zephyr/kernel/obj_core.h>
#include <zephyr/sys/rb.h>
#endif

/** @brief 线程优先级总数 = 抢占优先级数 + 协作优先级数 + 1（idle 优先级） */
#define K_NUM_THREAD_PRIO (CONFIG_NUM_PREEMPT_PRIORITIES + CONFIG_NUM_COOP_PRIORITIES + 1)

/** @brief 优先级位图大小（unsigned long 数组元素个数），用于快速查找最高优先级 */
#define PRIQ_BITMAP_SIZE  (DIV_ROUND_UP(K_NUM_THREAD_PRIO, BITS_PER_LONG))

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ============================================================
 * 线程状态位掩码（struct k_thread.thread_state 字段）
 * ============================================================
 *
 * 编码规则：通用状态使用低位（bit[0:7]），架构特定状态使用高位。
 * 多个状态位可以同时置位（组合状态）。
 *
 * 状态位定义必须放在 kernel_arch_data.h 之前，因为架构头文件可能依赖这些定义。
 */

/** @brief 哑线程标志：非真实线程（如启动阶段的 dummy thread） */
#define _THREAD_DUMMY (BIT(0))

/** @brief 等待标志：线程正在等待某个内核对象（信号量、互斥量、队列等） */
#define _THREAD_PENDING (BIT(1))

/** @brief 睡眠标志：线程处于睡眠状态（k_sleep / k_msleep） */
#define _THREAD_SLEEPING (BIT(2))

/** @brief 终止标志：线程已执行完毕或被终止，资源可回收 */
#define _THREAD_DEAD (BIT(3))

/** @brief 挂起标志：线程被显式挂起（k_thread_suspend） */
#define _THREAD_SUSPENDED (BIT(4))

/** @brief 中止中标志：线程正在执行中止操作（k_thread_abort 进行中） */
#define _THREAD_ABORTING (BIT(5))

/** @brief 挂起中标志：线程正在执行挂起操作（状态转换中间态） */
#define _THREAD_SUSPENDING (BIT(6))

/** @brief 就绪队列标志：线程当前在就绪队列中，可被调度执行 */
#define _THREAD_QUEUED (BIT(7))

/* 状态位定义结束 */

#ifdef CONFIG_STACK_SENTINEL
/**
 * @def STACK_SENTINEL
 * @brief 栈 sentinel 魔数（0xF0F0F0F0），存放在栈最低 4 字节
 *
 * @note 用于检测栈溢出：如果该值被覆盖，说明栈已溢出。
 */
#define STACK_SENTINEL 0xF0F0F0F0
#endif

/**
 * @def _NON_PREEMPT_THRESHOLD
 * @brief 非抢占阈值：当 preempt >= 此值时，线程不可被抢占
 *
 * @details 值为 0x0080，协作线程（coop thread）的 preempt 值 >= 此阈值。
 */
#define _NON_PREEMPT_THRESHOLD 0x0080U

/**
 * @def _PREEMPT_THRESHOLD
 * @brief 抢占阈值：当 preempt <= 此值时，线程可被抢占
 *
 * @details 值为 0x007F，抢占线程（preempt thread）的 preempt 值 <= 此阈值。
 */
#define _PREEMPT_THRESHOLD (_NON_PREEMPT_THRESHOLD - 1U)

#if !defined(_ASMLANGUAGE)

/*
 * ============================================================
 * 线程优先级队列抽象
 * ============================================================
 *
 * Zephyr 提供三种优先级队列实现，分别适用于不同场景：
 *
 * 1. CONFIG_SCHED_SIMPLE（简单双向链表）：
 *    - 数据结构：sys_dlist_t（排序双向链表）
 *    - 插入复杂度：O(N)
 *    - 代码体积：最小
 *    - 适用场景：线程数量少、对代码体积敏感的系统
 *
 * 2. CONFIG_SCHED_SCALABLE（可扩展红黑树）：
 *    - 数据结构：rbtree（平衡二叉搜索树）
 *    - 插入复杂度：O(logN)
 *    - 代码体积：较大
 *    - 适用场景：线程数量多、需要 O(logN) 调度性能的系统
 *
 * 3. CONFIG_SCHED_MULTIQ（多队列）：
 *    - 数据结构：每个优先级一个独立链表 + 位图索引
 *    - 插入复杂度：O(1)
 *    - 代码体积：RAM 占用较大（每个优先级一个链表头）
 *    - 适用场景：优先级数量有限、需要极快调度响应的系统
 *    - 限制：不适用于 deadline scheduling 等需要大优先级空间的特性
 *
 * 等待队列（wait_q）可选用简单链表或红黑树。
 * 系统就绪队列（ready_q）可选用三种中任意一种。
 * 均在构建时通过 Kconfig 配置选择。
 */

/**
 * @brief 基于红黑树的优先级队列
 *
 * @details 使用平衡二叉搜索树（rbtree）实现可扩展的优先级队列。
 *          线程按优先级排序存储，支持 O(logN) 的插入和删除。
 *          next_order_key 用于在同优先级线程之间维护 FIFO 顺序。
 */
struct _priq_rb {
	struct rbtree tree;       /**< 红黑树根节点 */
	int next_order_key;       /**< 下一个排序键，同优先级线程按此值排序保证 FIFO */
};

/**
 * @brief 基于多队列的优先级队列
 *
 * @details 为每个优先级维护一个独立的双向链表，通过位图（bitmask）
 *          快速定位非空链表中的最高优先级，实现 O(1) 的最高优先级查找。
 */
struct _priq_mq {
	sys_dlist_t queues[K_NUM_THREAD_PRIO]; /**< 每个优先级一个链表 */
	unsigned long bitmask[PRIQ_BITMAP_SIZE]; /**< 位图：bit[i]=1 表示优先级 i 有就绪线程 */
#ifndef CONFIG_SMP
	unsigned int cached_queue_index; /**< 缓存的最高优先级索引（单核优化，避免重复位图扫描） */
#endif
};

/**
 * @brief 系统就绪队列
 *
 * @details 调度器的核心数据结构，存放所有处于就绪状态（可被调度）的线程。
 *          根据配置使用不同的底层实现（简单链表/红黑树/多队列）。
 */
struct _ready_q {
#ifndef CONFIG_SMP
	/**
	 * @brief 调度缓存：指向下一个应被执行的线程
	 *
	 * @details 单核模式下始终包含下一个要运行的线程，不允许为 NULL。
	 *          初始化时设为 main 线程，避免空指针检查开销。
	 */
	struct k_thread *cache;
#endif

#if defined(CONFIG_SCHED_SIMPLE)
	sys_dlist_t runq;         /**< 简单双向链表：按优先级排序的线程链表 */
#elif defined(CONFIG_SCHED_SCALABLE)
	struct _priq_rb runq;     /**< 红黑树：可扩展的优先级队列 */
#elif defined(CONFIG_SCHED_MULTIQ)
	struct _priq_mq runq;     /**< 多队列：每个优先级独立链表 + 位图索引 */
#endif
};

/** @brief 就绪队列类型别名 */
typedef struct _ready_q _ready_q_t;

/**
 * @brief CPU 核心数据结构
 *
 * @details 每个 CPU 核心对应一个 _cpu 实例，存储该核心的运行状态：
 *          当前执行的线程、idle 线程、中断栈、嵌套中断计数等。
 *          SMP 系统中有多个实例，单核系统仅有一个。
 */
struct _cpu {
	/** @brief 嵌套中断计数器，记录当前中断嵌套深度 */
	uint32_t nested;

	/** @brief 中断栈基地址（栈向低地址增长，此为栈区域最高地址） */
	char *irq_stack;

	/** @brief 当前正在该 CPU 上执行的线程指针 */
	struct k_thread *current;

	/** @brief 该 CPU 的 idle 线程指针（每 CPU 一个） */
	struct k_thread *idle_thread;

#ifdef CONFIG_SCHED_CPU_MASK_PIN_ONLY
	/**
	 * @brief CPU 私有的就绪队列（仅 CONFIG_SCHED_CPU_MASK_PIN_ONLY 模式）
	 *
	 * @details 在线程被绑定到特定 CPU 的调度模式下，
	 *          每个 CPU 维护独立的就绪队列，不共享全局队列。
	 */
	struct _ready_q ready_q;
#endif

#if (CONFIG_NUM_METAIRQ_PRIORITIES > 0)
	/**
	 * @brief 被 metairq 抢占的协作线程指针
	 *
	 * @details 当一个 metairq（元中断）抢占了一个协作线程时，
	 *          保存被抢占线程的指针，以便 metairq 处理完成后恢复。
	 *          若无抢占则为 NULL。
	 */
	struct k_thread *metairq_preempted;
#endif

	/** @brief CPU 逻辑编号（0, 1, 2, ...） */
	uint8_t id;

#if defined(CONFIG_FPU_SHARING)
	/**
	 * @brief FPU 上下文保存指针
	 *
	 * @details 指向当前线程的 FPU 寄存器保存区域。
	 *          线程切换时用于保存/恢复浮点寄存器状态。
	 */
	void *fp_ctx;
#endif

#ifdef CONFIG_SMP
	/**
	 * @brief 上下文切换许可标志（SMP 专用）
	 *
	 * @details 为 true 时表示当前线程允许被上下文切换。
	 *          在持有自旋锁等临界区内会被临时设为 false，
	 *          防止意外的线程迁移导致死锁。
	 */
	uint8_t swap_ok;
#endif

#ifdef CONFIG_SCHED_THREAD_USAGE
	/**
	 * @brief 线程执行时间戳标记
	 *
	 * @details 记录当前线程执行窗口的起始时间戳。
	 *          特殊值 0 表示线程执行已停止（但统计功能未禁用）。
	 */
	uint32_t usage0;

#ifdef CONFIG_SCHED_THREAD_USAGE_ALL
	/**
	 * @brief 指向该 CPU 的 cycle 统计信息结构体
	 *
	 * @details 包含该 CPU 上各线程的执行周期统计，
	 *          用于线程级 CPU 使用率分析。
	 */
	struct k_cycle_stats *usage;
#endif
#endif

#ifdef CONFIG_OBJ_CORE_SYSTEM
	/** @brief CPU 对象核心实例，用于对象跟踪和调试框架 */
	struct k_obj_core  obj_core;
#endif

#ifdef CONFIG_SCHED_IPI_SUPPORTED
	/**
	 * @brief IPI（处理器间中断）工作队列
	 *
	 * @details 存放需要通过 IPI 发送给其他 CPU 的待处理工作项。
	 *          用于 SMP 调度器中的跨核线程迁移通知。
	 */
	sys_dlist_t ipi_workq;
#endif

	/** @brief CPU 架构特定的私有数据（在中断栈末尾对齐存放） */
	struct _cpu_arch arch;
};

/** @brief CPU 核心数据结构类型别名 */
typedef struct _cpu _cpu_t;

/**
 * @brief Zephyr 全局内核数据结构（唯一实例 _kernel）
 *
 * @details 这是 Zephyr 内核的全局状态根结构体，在 init.c 中定义。
 *          包含系统中所有 CPU 的状态、全局就绪队列、线程监控列表、
 *          电源管理状态和 SMP IPI 机制等。
 *
 * 内存布局：该结构体位于 .pinned_bss section，确保缓存对齐且不被换出。
 */
struct z_kernel {
	/** @brief 各 CPU 核心的数据数组，大小由 CONFIG_MP_MAX_NUM_CPUS 决定 */
	struct _cpu cpus[CONFIG_MP_MAX_NUM_CPUS];

#ifdef CONFIG_PM
	/**
	 * @brief 内核 idle tick 数（电源管理使用）
	 *
	 * @details 记录内核进入 idle 状态前的 tick 值，
	 *          电源管理子系统据此计算 idle 持续时间并决定低功耗策略。
	 */
	int32_t idle;
#endif

	/**
	 * @brief 全局就绪队列（非 CPU 绑定模式）
	 *
	 * @details 存放系统中所有就绪线程，调度器从中选择下一个执行的线程。
	 *          该字段较大（尤其是多队列模式），因此放在小字段之后，
	 *          以减少某些架构（如 ARC）上偏移量编码的限制。
	 */
#ifndef CONFIG_SCHED_CPU_MASK_PIN_ONLY
	struct _ready_q ready_q;
#endif

#if defined(CONFIG_THREAD_MONITOR)
	/**
	 * @brief 所有线程的单向链表头指针
	 *
	 * @details 用于线程调试和遍历：通过此链表可以枚举系统中
	 *          所有活跃的线程。链表按线程创建顺序链接。
	 */
	struct k_thread *threads;
#endif
#ifdef CONFIG_SCHED_THREAD_USAGE_ALL
	/**
	 * @brief 各 CPU 的 cycle 统计数据数组
	 *
	 * @details 存放各 CPU 上所有线程的执行周期统计数据，
	 *          _cpu.usage 指针指向此数组中对应 CPU 的条目。
	 */
	struct k_cycle_stats usage[CONFIG_MP_MAX_NUM_CPUS];
#endif

#ifdef CONFIG_OBJ_CORE_SYSTEM
	/** @brief 内核对象核心实例，用于对象跟踪和调试框架 */
	struct k_obj_core  obj_core;
#endif

#if defined(CONFIG_SMP) && defined(CONFIG_SCHED_IPI_SUPPORTED)
	/**
	 * @brief 待发送 IPI 的目标 CPU 位图
	 *
	 * @details 原子变量，记录在下一次调度点需要发送 IPI 的目标 CPU。
	 *          每个 bit 对应一个 CPU，bit[i]=1 表示 CPU i 需要被通知。
	 */
	atomic_t pending_ipi;
#endif
};

/** @brief 全局内核数据结构类型别名 */
typedef struct z_kernel _kernel_t;

/** @brief 全局内核实例（在 init.c 中定义为 __pinned_bss） */
extern struct z_kernel _kernel;

/** @brief 活跃 CPU 数量原子计数器（电源管理子系统使用） */
extern atomic_t _cpus_active;

#ifdef CONFIG_SMP

/**
 * @brief 检查当前上下文是否可被抢占且可迁移到其他 CPU
 *
 * @return true 当前线程可以被迁移到其他 CPU 核心
 *
 * @note 在 SMP 系统中，持有自旋锁等不可迁移上下文中返回 false。
 */
bool z_smp_cpu_mobile(void);

/**
 * @brief 获取当前 CPU 指针（SMP 安全版本）
 *
 * @return 指向当前 CPU 的 _cpu 结构体指针
 *
 * @note 断言当前上下文不可迁移（z_smp_cpu_mobile() == false），
 *       确保获取的 CPU 指针在后续操作中不会失效。
 */
#define _current_cpu ({ __ASSERT_NO_MSG(!z_smp_cpu_mobile()); \
			arch_curr_cpu(); })

/**
 * @brief 获取当前执行线程指针（SMP 安全版本）
 *
 * @return 指向当前正在执行的 k_thread 结构体指针
 */
__attribute_const__ struct k_thread *z_smp_current_get(void);

/** @brief 获取当前线程（SMP 模式宏） */
#define _current z_smp_current_get()

#else
/** @brief 获取当前 CPU 指针（单核模式：直接返回 CPU 0） */
#define _current_cpu (&_kernel.cpus[0])
/** @brief 获取当前线程（单核模式：直接返回 CPU 0 的 current） */
#define _current _kernel.cpus[0].current
#endif

/** @brief 获取当前 CPU ID（单核系统编译时优化为常量 0） */
#define CPU_ID ((CONFIG_MP_MAX_NUM_CPUS == 1) ? 0 : _current_cpu->id)

/**
 * @brief 设置当前 CPU 的执行线程
 *
 * @param[in] thread 要设置为当前执行的线程指针
 *
 * @note 仅在抢占被禁用的上下文中调用（如中断锁定期间、调度器内部），
 *       确保设置过程中不会被调度器打断。
 */
#define z_current_thread_set(thread) ({ _current_cpu->current = (thread); })

#ifdef CONFIG_ARCH_HAS_CUSTOM_CURRENT_IMPL
/* 架构自定义的当前线程实现（覆盖通用宏） */
#undef _current
#define _current arch_current_thread()
#undef z_current_thread_set
/**
 * @brief 设置当前线程（架构自定义版本）
 *
 * @details 同时更新架构特定的当前线程缓存和通用 _cpu.current 字段。
 */
#define z_current_thread_set(thread) \
	arch_current_thread_set(({ _current_cpu->current = (thread); }))
#endif

/*
 * ============================================================
 * 内核等待队列
 * ============================================================
 *
 * 等待队列用于存放因等待某个内核对象而阻塞的线程。
 * 两种实现可选：
 *   - 简单双向链表（默认）：O(N) 插入，代码体积小
 *   - 红黑树（CONFIG_WAITQ_SCALABLE）：O(logN) 插入，适合大量等待线程
 */

#ifdef CONFIG_WAITQ_SCALABLE

/**
 * @brief 基于红黑树的等待队列
 */
typedef struct {
	struct _priq_rb waitq; /**< 红黑树实现的优先级等待队列 */
} _wait_q_t;

/** @brief 红黑树等待队列的节点比较函数（在 kernel/priority_queues.c 中实现） */
bool z_priq_rb_lessthan(struct rbnode *a, struct rbnode *b);

/** @brief 等待队列静态初始化宏（红黑树版本） */
#define Z_WAIT_Q_INIT(wait_q) { { { .lessthan_fn = z_priq_rb_lessthan } } }

#else

/**
 * @brief 基于双向链表的等待队列
 */
typedef struct {
	sys_dlist_t waitq; /**< 双向链表：按优先级排序的等待线程 */
} _wait_q_t;

/** @brief 等待队列静态初始化宏（链表版本） */
#define Z_WAIT_Q_INIT(wait_q) { SYS_DLIST_STATIC_INIT(&(wait_q)->waitq) }

#endif /* CONFIG_WAITQ_SCALABLE */

/*
 * ============================================================
 * 内核超时机制
 * ============================================================
 */

/** @brief 超时结构体前向声明 */
struct _timeout;

/**
 * @brief 超时回调函数指针类型
 *
 * @param[in] t 指向触发此回调的 _timeout 结构体
 *
 * @note 回调在中断上下文（timeout 系统的软件中断）中执行。
 */
typedef void (*_timeout_func_t)(struct _timeout *t);

/**
 * @brief 内核超时记录结构体
 *
 * @details 用于实现 k_sleep、k_sem、k_mutex 等带有超时参数的 API。
 *          当超时到期时，注册的回调函数被调用以唤醒等待的线程。
 *          多个 _timeout 通过 sys_dnode_t 链接成超时队列。
 */
struct _timeout {
	/** @brief 双向链表节点，用于挂入超时队列 */
	sys_dnode_t node;

	/** @brief 超时到期时调用的回调函数 */
	_timeout_func_t fn;

#ifdef CONFIG_TIMEOUT_64BIT
	/**
	 * @brief 超时时间（tick 数），64 位精度
	 *
	 * @details 值为正数表示未来到期，负数或零表示已到期。
	 *          使用 64 位类型避免 32 位溢出问题。
	 */
	int64_t dticks;
#else
	/**
	 * @brief 超时时间（tick 数），32 位精度
	 *
	 * @details 值为正数表示未来到期，负数或零表示已到期。
	 */
	int32_t dticks;
#endif
};

/**
 * @brief 线程时间片到期回调函数指针类型
 *
 * @param[in] thread 时间片到期的线程
 * @param[in] data   注册时传入的用户数据指针
 *
 * @note 回调在调度器上下文中执行。
 */
typedef void (*k_thread_timeslice_fn_t)(struct k_thread *thread, void *data);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_ */
