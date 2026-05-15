/**
 * @file prep_c.c
 * @brief ARM Cortex-M 完整 C 运行环境初始化
 *
 * 该模块是 Zephyr 在 ARM Cortex-M 架构上从汇编启动代码进入 C 世界后的
 * 第一个 C 函数入口（z_prep_c），负责在跳转到内核主初始化流程 z_cstart()
 * 之前完成全部硬件级准备：
 *   1. 调用 SoC 级早期 hook（soc_prep_hook）
 *   2. 重定位向量表（从 Flash 到 SRAM，或设置 VTOR）
 *   3. 初始化 FPU（若 CPU 具备浮点单元）
 *   4. 清零 .bss 段、拷贝 .data 段（XIP 场景）
 *   5. 初始化中断控制器（NVIC 或 SoC 自定义中断控制器）
 *   6. 初始化 cache（若架构支持）
 *   7. 启用 NULL 指针异常检测（可选，通过 DWT 单元）
 *
 * @note 该模块执行时栈已可用，但 .data/.bss 段尚未初始化，
 *       因此不可使用任何全局/静态变量（除 .bss 零初始化语义等价的情况）。
 *
 * @copyright Copyright (c) 2013-2014 Wind River Systems, Inc.
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <kernel_internal.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/sys/barrier.h>
#include <zephyr/platform/hooks.h>
#include <zephyr/arch/cache.h>
#include <cortex_m/debug.h>
#include <zephyr/arch/common/xip.h>
#include <zephyr/arch/common/init.h>

/*
 * GCC 会检测 memcpy 是否传入 NULL 参数，但在 relocate_vector_table()
 * 中某些分支传入 NULL 是合法的，因此在此处抑制该警告。
 * 必须在 string.h 被包含之前执行，以影响 memcpy 的声明。
 */
TOOLCHAIN_DISABLE_WARNING(TOOLCHAIN_WARNING_NONNULL)

#include <string.h>

#if defined(CONFIG_SW_VECTOR_RELAY) || defined(CONFIG_SW_VECTOR_RELAY_CLIENT)
/** @brief 软件向量表中继指针 */
Z_GENERIC_SECTION(.vt_pointer_section) __attribute__((used)) void *_vector_table_pointer;
#endif

#ifdef CONFIG_CPU_CORTEX_M_HAS_VTOR

#ifdef CONFIG_SRAM_VECTOR_TABLE
/** @brief 向量表目标地址：SRAM 区域（可运行时修改） */
#define VECTOR_ADDRESS ((uintptr_t)_sram_vector_start)
#else
/** @brief 向量表目标地址：Flash 区域（XIP 或直接映射） */
#define VECTOR_ADDRESS ((uintptr_t)_vector_start)
#endif

/*
 * 在部分 Cortex-M3 实现中，SCB_VTOR bit[29] 被定义为 TBLBASE 位，
 * 用于选择向量表位于 Code (0) 还是 RAM (1) 区域。需将其纳入掩码。
 */
#ifdef SCB_VTOR_TBLBASE_Msk
/** @brief VTOR 寄存器有效位掩码（含 TBLBASE 位） */
#define VTOR_MASK (SCB_VTOR_TBLBASE_Msk | SCB_VTOR_TBLOFF_Msk)
#else
/** @brief VTOR 寄存器有效位掩码（仅偏移位） */
#define VTOR_MASK SCB_VTOR_TBLOFF_Msk
#endif

/**
 * @brief 重定位向量表到目标地址并设置 VTOR 寄存器
 *
 * 当配置了 CONFIG_SRAM_VECTOR_TABLE 时，先将向量表从 Flash 拷贝到 SRAM，
 * 然后将 SRAM 地址写入 SCB->VTOR，使 CPU 从 SRAM 读取异常向量。
 * 这允许运行时动态修改向量表（如安装新的 ISR）。
 *
 * 若未配置 CONFIG_SRAM_VECTOR_TABLE，直接将 Flash 中的向量表基地址
 * 写入 VTOR。
 *
 * @note 函数标记为 __weak，SoC 或板级代码可提供自定义实现。
 * @note 写入 VTOR 后执行完整的数据同步屏障和指令同步屏障，
 *       确保后续指令使用更新后的向量表地址。
 */
void __weak relocate_vector_table(void)
{
#ifdef CONFIG_SRAM_VECTOR_TABLE
	/* 将向量表从 Flash 拷贝到 SRAM */
	size_t vector_size = (size_t)_vector_end - (size_t)_vector_start;

	arch_early_memcpy(_sram_vector_start, _vector_start, vector_size);
#endif
	SCB->VTOR = VECTOR_ADDRESS & VTOR_MASK;
	barrier_dsync_fence_full();
	barrier_isync_fence_full();
}

#else
#define VECTOR_ADDRESS 0

/**
 * @brief 重定位向量表（无 VTOR 支持的 Cortex-M 变体）
 *
 * 在不支持 VTOR 寄存器的 Cortex-M 实现上，向量表必须位于地址 0x00000000。
 * 当向量表编译地址不为 0 时（XIP 且 Flash 基址非 0，或 SRAM 基址非 0），
 * 需要将向量表拷贝到地址 0 处。
 *
 * 若配置了软件向量表中继（CONFIG_SW_VECTOR_RELAY），则将向量表地址
 * 存储到 _vector_table_pointer 全局变量中，由中继 stub 间接跳转。
 *
 * @note 函数标记为 __weak，可被 SoC 代码覆盖。
 */
void __weak relocate_vector_table(void)
{
#if defined(CONFIG_XIP) && (CONFIG_FLASH_BASE_ADDRESS != 0) ||                                     \
	!defined(CONFIG_XIP) && (CONFIG_SRAM_BASE_ADDRESS != 0)
	size_t vector_size = (size_t)_vector_end - (size_t)_vector_start;
	(void)memcpy(VECTOR_ADDRESS, _vector_start, vector_size);
#elif defined(CONFIG_SW_VECTOR_RELAY) || defined(CONFIG_SW_VECTOR_RELAY_CLIENT)
	_vector_table_pointer = _vector_start;
#endif
}

TOOLCHAIN_ENABLE_WARNING(TOOLCHAIN_WARNING_NONNULL)

#endif /* CONFIG_CPU_CORTEX_M_HAS_VTOR */

#if defined(CONFIG_CPU_HAS_FPU)

/**
 * @brief 初始化 ARM Cortex-M 浮点单元（FPU）
 *
 * 完成 FPU 相关系统寄存器的初始化配置：
 *   1. 清除 CPACR 中 CP10/CP11 的访问权限位（防止前序固件残留配置）
 *   2. 若启用了 CONFIG_FPU，按配置设置特权级或全访问权限
 *   3. 配置 FPCCR 控制浮点上下文的自动/惰性压栈行为
 *   4. 初始化 FPSCR 状态控制寄存器
 *   5. 确保 CONTROL.FPCA 位被清除，防止异常出栈错误
 *
 * @details CPACR 访问权限配置策略：
 *   - CONFIG_FPU + CONFIG_USERSPACE → CP10/CP11 完全访问（特权+用户态）
 *   - CONFIG_FPU 无 CONFIG_USERSPACE → CP10/CP11 仅特权访问
 *
 * @details FPCCR 配置策略：
 *   - 多线程但不共享 FPU 寄存器 → 禁用 ASPEN/LSPEN，
 *     避免中断时自动压栈 FP 寄存器，降低中断延迟和栈开销
 *   - 共享 FPU 寄存器或单线程 → 启用 ASPEN/LSPEN，
 *     实现惰性压栈：异常入口仅预留栈空间但不实际保存 FP 寄存器，
 *     直到线程切换或 ISR 使用 FP 指令时才真正压栈
 */
static inline void z_arm_floating_point_init(void)
{
	/*
	 * 复位后 CPACR 通常为 0x00000000，但 Zephyr 之前的固件
	 * 可能未清除该寄存器，因此先清除 CP10/CP11 的访问权限位。
	 */
	SCB->CPACR &= (~(CPACR_CP10_Msk | CPACR_CP11_Msk));

#if defined(CONFIG_FPU)
	/*
	 * 使能 CP10 和 CP11 协处理器访问权限，以允许使用浮点寄存器。
	 */
#if defined(CONFIG_USERSPACE)
	/* 特权态和用户态均可访问 */
	SCB->CPACR |= CPACR_CP10_FULL_ACCESS | CPACR_CP11_FULL_ACCESS;
#else
	/* 仅特权态可访问 */
	SCB->CPACR |= CPACR_CP10_PRIV_ACCESS | CPACR_CP11_PRIV_ACCESS;
#endif  /* CONFIG_USERSPACE */
	/*
	 * 复位后 FPCCR 的值为 0xC0000000（ASPEN 和 LSPEN 均使能）。
	 */
#if defined(CONFIG_MULTITHREADING) && !defined(CONFIG_FPU_SHARING)
	/*
	 * 非共享 FP 寄存器模式（多线程环境下仅单一线程使用 FPU）。
	 * 禁用异常入口时的 FP 寄存器自动压栈（CONTROL.FPCA 自动设置），
	 * 因为 FP 寄存器仅被单一上下文使用且 ISR 中不支持 FP 指令。
	 * 此配置可减少中断延迟并降低使用 FPU 的线程栈内存需求。
	 */
	FPU->FPCCR &= (~(FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk));
#else
	/*
	 * 共享 FP 寄存器模式（多线程）或单线程模式。
	 *
	 * 使能 FP 上下文的自动和惰性状态保存。
	 * 若线程使用了浮点寄存器，CONTROL 寄存器的 FPCA 位将被自动设置。
	 * 由于惰性保存机制，异常入口时 volatile FP 寄存器不会立即入栈，
	 * 但栈帧中会预留所需空间。此配置改善了中断延迟。
	 * FP 寄存器最终在线程切换或 ISR 尝试执行 FP 指令时才真正入栈。
	 */
	FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
#endif /* CONFIG_FPU_SHARING */

	/* 确保修改 FPCCR 的副作用立即生效 */
	barrier_dsync_fence_full();
	barrier_isync_fence_full();

	/* 初始化浮点状态和控制寄存器（FPSCR） */
#if defined(CONFIG_ARMV8_1_M_MAINLINE)
	/*
	 * ARMv8.1-M FPU 的 FPSCR[18:16] LTPSIZE 字段必须设置为 0b100
	 * （表示"未应用尾预测"），这也是其复位默认值。
	 */
	__set_FPSCR(4 << FPU_FPDSCR_LTPSIZE_Pos);
#else
	__set_FPSCR(0);
#endif

	/*
	 * 注意：
	 * FP 寄存器组的访问已使能，但 FP 上下文（CONTROL 寄存器的 FPCA 位）
	 * 仅在实际执行浮点指令时才会被激活。
	 */

#endif /* CONFIG_FPU */

	/*
	 * 复位后 CONTROL.FPCA 通常被清除，但 Zephyr 之前的固件可能未清除。
	 * 必须清除该位以防止异常出栈时出错。
	 *
	 * 注意：
	 * 在共享 FP 寄存器模式下，切换到 main 之前 CONTROL.FPCA 会被清除，
	 * 因此此处可以跳过（节省几个启动周期）。
	 *
	 * 若设置了 CONFIG_INIT_ARCH_HW_AT_BOOT，CONTROL 在复位时已被清除。
	 */
#if (!defined(CONFIG_FPU) || !defined(CONFIG_FPU_SHARING)) &&                                      \
	(!defined(CONFIG_INIT_ARCH_HW_AT_BOOT))

	__set_CONTROL(__get_CONTROL() & (~(CONTROL_FPCA_Msk)));
#endif
}

#endif /* CONFIG_CPU_HAS_FPU */

extern FUNC_NORETURN void z_cstart(void);

/**
 * @brief 准备并执行 C 代码（C 运行环境初始化入口）
 *
 * 这是 Cortex-M 启动流程中从汇编代码跳转到的第一个 C 函数。
 * 按顺序完成以下硬件级初始化后，跳转到 Zephyr 内核主初始化入口 z_cstart()：
 *
 *   1. **soc_prep_hook()** — 调用 SoC 级早期初始化 hook
 *   2. **relocate_vector_table()** — 重定位向量表（拷贝到 SRAM 或设置 VTOR）
 *   3. **z_arm_floating_point_init()** — 初始化 FPU（若 CPU 具备浮点单元）
 *   4. **arch_bss_zero()** — 清零 .bss 段
 *   5. **arch_data_copy()** — 从 Flash 拷贝 .data 段到 SRAM（XIP 场景）
 *   6. **z_soc_irq_init() / z_arm_interrupt_init()** — 初始化中断控制器
 *   7. **arch_cache_init()** — 初始化指令/数据 cache（若架构支持）
 *   8. **z_arm_debug_enable_null_pointer_detection()** — 启用 NULL 指针异常检测
 *
 * @note 此函数不会返回（FUNC_NORETURN）。
 * @note 在 arch_bss_zero/arch_data_copy 之前，不可使用未显式零初始化的
 *       全局/静态变量。
 *
 * @see z_cstart() Zephyr 内核主初始化流程
 * @see soc_early_init_hook SoC 早期初始化 hook
 */
FUNC_NORETURN void z_prep_c(void)
{
	soc_prep_hook();

	relocate_vector_table();
#if defined(CONFIG_CPU_HAS_FPU)
	z_arm_floating_point_init();
#endif
	arch_bss_zero();
	arch_data_copy();
#if defined(CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER)
	/* 调用 SoC 特定的中断控制器初始化 */
	z_soc_irq_init();
#else
	z_arm_interrupt_init();
#endif /* CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER */
#if CONFIG_ARCH_CACHE
	arch_cache_init();
#endif

#ifdef CONFIG_NULL_POINTER_EXCEPTION_DETECTION_DWT
	z_arm_debug_enable_null_pointer_detection();
#endif
	z_cstart();
	CODE_UNREACHABLE;
}
