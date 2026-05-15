/**
 * @file reset.S
 * @brief ARM Cortex-M 复位处理程序（Reset Handler）
 *
 * 该文件是 Zephyr 在 ARM Cortex-M 架构上的**最高优先级启动代码**，
 * 是 CPU 上电复位后执行的第一段指令（复位向量指向此处）。
 * 也支持被 bootloader chain-load 或调试器直接跳转进入。
 *
 * 执行流程概览：
 *   z_arm_reset (复位入口)
 *     ├── [CONFIG_INIT_ARCH_HW_AT_BOOT] 清除 CONTROL/SPLIM 寄存器
 *     ├── [CONFIG_SOC_EARLY_RESET_HOOK]  调用 SoC 极早期 hook
 *     ├── [CONFIG_PM_S2RAM]              暂存 MSP 到中断栈，恢复 S2RAM
 *     ├── 设置 MSP → z_main_stack 顶部
 *     ├── [CONFIG_DEBUG_THREAD_INFO]     清除 z_sys_post_kernel 标志
 *     ├── [CONFIG_SOC_RESET_HOOK]        调用 SoC 复位 hook
 *     ├── [CONFIG_INIT_ARCH_HW_AT_BOOT] 禁用 MPU + 初始化架构硬件
 *     ├── 关闭中断（BASEPRI 或 cpsid i）
 *     ├── [CONFIG_WDOG_INIT]             初始化看门狗
 *     ├── [CONFIG_INIT_STACKS]           用 0xaa 填充中断栈（栈溢出检测）
 *     ├── 切换到 PSP（保留 MSP 给中断使用）
 *     └── bl z_prep_c → prep_c.c（进入 C 环境）
 *
 * 前置条件：
 *   - CPU 处于 Thread mode + Privileged 级别
 *   - MSP 已指向有效 SRAM 区域（由硬件从向量表偏移 0 处自动加载）
 *
 * @note 函数标记为 FUNC_NORETURN，不会返回。
 * @note 中断在此阶段被关闭，仅在切换到 main 线程后才解锁。
 *
 * @copyright Copyright (c) 2013-2014 Wind River Systems, Inc.
 * @copyright Copyright 2025 Arm Limited and/or its affiliates
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
#include "vector_table.h"

_ASM_FILE_PROLOGUE

/*
 * 全局符号导出声明，使链接器能解析这些符号的跨文件引用。
 */

GTEXT(z_arm_reset)              /**< 复位处理程序入口 */
GTEXT(arch_early_memset)        /**< 早期内存填充（栈初始化用） */
GDATA(z_interrupt_stacks)       /**< 中断栈数组基地址 */
GDATA(z_main_stack)             /**< main 线程栈基地址 */
#if defined(CONFIG_DEBUG_THREAD_INFO)
GDATA(z_sys_post_kernel)        /**< POST_KERNEL 阶段完成标志 */
#endif
#if defined(CONFIG_SOC_RESET_HOOK)
GTEXT(soc_reset_hook)           /**< SoC 复位 hook */
#endif
#if defined(CONFIG_INIT_ARCH_HW_AT_BOOT)
GTEXT(z_arm_init_arch_hw_at_boot) /**< 架构硬件初始化函数 */
#endif
#if defined(CONFIG_PM_S2RAM)
GTEXT(arch_pm_s2ram_resume)     /**< Suspend-to-RAM 恢复函数 */
#endif
#if defined(CONFIG_SOC_EARLY_RESET_HOOK)
GTEXT(soc_early_reset_hook)     /**< SoC 极早期复位 hook */
#endif


/*
 * PACBTI CONTROL 寄存器掩码：
 *   bit[4] = BTI_EN    — Branch Target Identification 使能（特权态）
 *   bit[5] = UBTI_EN   — Branch Target Identification 使能（用户态）
 *   bit[6] = PAC_EN    — Pointer Authentication 使能（特权态）
 *   bit[7] = UPAC_EN   — Pointer Authentication 使能（用户态）
 */

#ifdef CONFIG_ARM_PAC
/** @brief PAC 使能掩码：bit[6] PAC_EN + bit[7] UPAC_EN */
#define CONTROL_ARM_PAC_MASK	(1<<6)|(1<<7)
#else
#define CONTROL_ARM_PAC_MASK	0
#endif

#ifdef CONFIG_ARM_BTI
/** @brief BTI 使能掩码：bit[4] BTI_EN + bit[5] UBTI_EN */
#define CONTROL_ARM_BTI_MASK	(1<<4)|(1<<5)
#else
#define CONTROL_ARM_BTI_MASK	0
#endif

/**
 * @brief ARM Cortex-M 复位向量处理程序
 *
 * CPU 上电复位或被 bootloader chain-load 后的第一段执行代码。
 *
 * 此时的处理器状态：
 *   - 运行在 Thread mode + Privileged 级别
 *   - MSP 从向量表偏移 0 处自动加载
 *   - 中断使能状态未定义（取决于前序镜像或硬件默认值）
 *
 * 主要工作：
 *   1. 清理前序镜像残留的架构状态（CONTROL/MPU 等）
 *   2. 设置栈指针（MSP → main stack，PSP → interrupt stack）
 *   3. 关闭中断防止启动过程被打断
 *   4. 跳转到 z_prep_c() 进入 C 运行环境
 */
SECTION_SUBSEC_FUNC(TEXT,_reset_section,z_arm_reset)

/*
 * __start 符号别名：某些 XIP 镜像作为 bootloader 时
 * 会搜索 __start 符号而非 z_arm_reset，在此创建别名。
 */
SECTION_SUBSEC_FUNC(TEXT,_reset_section,__start)

#if defined(CONFIG_INIT_ARCH_HW_AT_BOOT)
    /* ------------------------------------------------
     * 清除 CONTROL 寄存器，确保干净的状态。
     * 复位后 CONTROL 通常为 0，但 chain-load 场景下
     * 前序镜像可能留下非零值（如 SPSEL=1 表示使用 PSP），
     * 必须显式清除。
     * ------------------------------------------------ */
    movs.n r0, #0          /* r0 = 0（窄指令 .n 编码节省空间） */
    msr CONTROL, r0        /* CONTROL = 0：清除所有控制位，切回 MSP */
    isb                     /* 指令同步屏障：确保后续指令使用新的 CONTROL 值 */

#if defined(CONFIG_CPU_CORTEX_M_HAS_SPLIM)
    /* ------------------------------------------------
     * 清除栈指针限制寄存器。
     * SPLIM 用于栈溢出检测，残留值可能导致合法的栈操作
     * 触发 UsageFault。复位后应清零以禁用限制检查。
     * ------------------------------------------------ */
    movs.n r0, #0          /* r0 = 0 */
    msr MSPLIM, r0         /* 主栈指针限制 = 0（无限制） */
    msr PSPLIM, r0         /* 进程栈指针限制 = 0（无限制） */
#endif /* CONFIG_CPU_CORTEX_M_HAS_SPLIM */

#endif /* CONFIG_INIT_ARCH_HW_AT_BOOT */

#if defined(CONFIG_SOC_EARLY_RESET_HOOK)
    /*
     * SoC 极早期复位 hook：在任何栈设置和 RAM 访问之前执行。
     * 典型用途：启用 RAM 的时钟/电源域、配置 SRAM 时序参数。
     * 此时仅能使用 MSP（向量表加载的值）和寄存器操作。
     */
    bl soc_early_reset_hook
#endif

#if defined(CONFIG_PM_S2RAM)
    /*
     * Suspend-to-RAM (S2RAM) 恢复路径。
     *
     * 将 MSP 暂时设置为中断栈顶部，使 arch_pm_s2ram_resume()
     * 能够使用栈来调用 pm_s2ram_mark_check_and_clear()。
     *
     * 安全性分析：
     * - suspend 不会从 ISR 中调用，因此中断栈在 suspend 期间未活跃，
     *   复用中断栈不会破坏数据。
     * - 若确实从 suspend 恢复，MSP 将从 CPU 上下文中恢复，此处设置无效。
     * - 若未从 suspend 恢复（正常冷启动），MSP 稍后会被设置为 z_main_stack。
     */
    ldr r0, =z_interrupt_stacks + CONFIG_ISR_STACK_SIZE + MPU_GUARD_ALIGN_AND_SIZE
                            /* r0 = 中断栈顶部地址（含 MPU guard 区域） */
    msr msp, r0            /* MSP = r0，临时切换到中断栈 */

    bl arch_pm_s2ram_resume /* 调用 S2RAM 恢复函数 */
#endif /* CONFIG_PM_S2RAM */

    /* ------------------------------------------------
     * 设置 MSP → z_main_stack 顶部。
     *
     * 需要覆盖以下场景：
     * 1. CONFIG_PM_S2RAM 启用：MSP 被临时设为中断栈，
     *    若非 suspend 恢复需恢复为 main stack。
     * 2. CONFIG_INIT_ARCH_HW_AT_BOOT 启用但非 S2RAM：
     *    chain-load 或调试器启动时 MSP 值不确定。
     * 3. CONFIG_INIT_STACKS 启用：需确保 MSP 指向 main stack
     *    而非中断栈，避免栈填充逻辑破坏 main stack。
     * ------------------------------------------------ */
    ldr r0, =z_main_stack + CONFIG_MAIN_STACK_SIZE
                            /* r0 = main stack 顶部地址（栈向低地址增长，取最高地址） */
    msr msp, r0            /* MSP = main stack 顶部 */

    /*
     * 注意：z_sys_post_kernel 等变量的初始化必须在
     * arch_pm_s2ram_resume() 之后执行，避免干扰
     * suspend/resume 路径。详见 issue #83660。
     */
#if defined(CONFIG_DEBUG_THREAD_INFO)
    /* 清除 z_sys_post_kernel 标志，供 RTOS 感知调试器使用 */
    movs.n r0, #0          /* r0 = 0 */
    ldr r1, =z_sys_post_kernel   /* r1 = z_sys_post_kernel 变量地址 */
    strb r0, [r1]          /* *r1 = 0：写入 1 字节（bool 类型），标记尚未进入 POST_KERNEL */
#endif /* CONFIG_DEBUG_THREAD_INFO */

#if defined(CONFIG_SOC_RESET_HOOK)
    /*
     * SoC 复位 hook：在栈设置完成后、架构硬件初始化之前调用。
     * 典型用途：配置 SoC 特定的时钟、电源域、引脚复用等。
     * 此时可以使用 main stack 进行函数调用。
     */
    bl soc_reset_hook
#endif

#if defined(CONFIG_INIT_ARCH_HW_AT_BOOT)
#if defined(CONFIG_CPU_HAS_ARM_MPU)
    /* ------------------------------------------------
     * 禁用 ARM MPU。
     * 清零 MPU_CTRL 寄存器以关闭 MPU，防止前序镜像的
     * MPU 区域配置阻止后续的内存访问（如清零 .bss）。
     * ------------------------------------------------ */
    movs.n r0, #0          /* r0 = 0 */
    ldr r1, =_SCS_MPU_CTRL /* r1 = MPU_CTRL 寄存器地址（System Control Space） */
    str r0, [r1]          /* MPU_CTRL = 0：禁用 MPU */
    dsb                     /* 数据同步屏障：确保 MPU 禁用对后续内存访问可见 */
#endif /* CONFIG_CPU_HAS_ARM_MPU */

    /* 初始化核心架构寄存器和系统块（VTOR/SCB/SysTick 等） */
    bl z_arm_init_arch_hw_at_boot
#endif /* CONFIG_INIT_ARCH_HW_AT_BOOT */

    /* ------------------------------------------------
     * 关闭中断。
     *
     * ARMv6-M / ARMv8-M Baseline：
     *   使用 cpsid i 指令直接设置 PRIMASK = 1，
     *   屏蔽所有可配置中断（仅保留 NMI 和 HardFault）。
     *
     * ARMv7-M / ARMv8-M Mainline：
     *   使用 BASEPRI 寄存器屏蔽优先级低于
     *   _EXC_IRQ_DEFAULT_PRIO 的所有中断。
     *   这允许 NMI 和 HardFault 仍然触发（它们的优先级
     *   为负数，不受 BASEPRI 影响），实现更好的安全性。
     *
     * 中断将在 switch_to_main_thread() 中切换到
     * main 线程时才被解锁。
     * ------------------------------------------------ */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    cpsid i                 /* PRIMASK = 1：全局关闭中断 */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    movs.n r0, #_EXC_IRQ_DEFAULT_PRIO
                            /* r0 = 默认中断优先级掩码值 */
    msr BASEPRI, r0        /* BASEPRI = r0：屏蔽低于此优先级的中断 */
#else
#error Unknown ARM architecture
#endif

#ifdef CONFIG_WDOG_INIT
    /*
     * 看门狗初始化：某些 SoC 的看门狗在复位后默认使能，
     * 如果不及时喂狗或禁用，系统会在启动过程中被复位。
     * 此处的初始化通常为禁用看门狗或设置较长的超时时间。
     */
    bl z_arm_watchdog_init
#endif

/*
 * 注意：在所有 Cortex-M 变体中，中断栈区域至少为
 * CONFIG_ISR_STACK_SIZE + MPU_GUARD_ALIGN_AND_SIZE
 * （实际可能因栈指针对齐而更大，但初始化期间按此计算已足够）。
 */

#ifdef CONFIG_INIT_STACKS
    /*
     * 用 0xaa 填充中断栈区域。
     * 后续可通过检查栈中剩余的 0xaa 来判断栈使用情况
     * 和检测栈溢出（stack usage analysis）。
     */
    ldr r0, =z_interrupt_stacks   /* r0 = 中断栈基地址 */
    ldr r1, =0xaa                 /* r1 = 填充值 0xaa */
    ldr r2, =CONFIG_ISR_STACK_SIZE + MPU_GUARD_ALIGN_AND_SIZE
                                    /* r2 = 填充长度（字节） */
    bl arch_early_memset           /* memset(r0, 0xaa, r2) */
#endif

    /* ------------------------------------------------
     * 切换到 PSP（Process Stack Pointer）执行后续代码。
     *
     * 设计意图：
     *   MSP 将在后续初始化中被设置为指向中断栈
     *   （z_interrupt_stacks），作为中断/异常的专用栈。
     *   如果继续使用 MSP 执行 C 代码，后续切换时会导致
     *   栈混乱。因此提前切换到 PSP，MSP 保留给中断使用。
     *
     * 操作步骤：
     *   1. 将中断栈顶部地址加载到 PSP
     *   2. 读取 CONTROL 寄存器
     *   3. 设置 CONTROL.SPSEL (bit[1]) = 1 切换到 PSP
     *   4. 根据配置设置 PAC/BTI 使能位
     *   5. 写回 CONTROL
     *   6. 执行 ISB 确保栈切换生效
     * ------------------------------------------------ */
    ldr r0, =z_interrupt_stacks   /* r0 = 中断栈基地址 */
    ldr r1, =CONFIG_ISR_STACK_SIZE + MPU_GUARD_ALIGN_AND_SIZE
                                    /* r1 = 中断栈大小（含 guard） */
    adds r0, r0, r1       /* r0 += r1：r0 = 中断栈顶部地址 */
    msr PSP, r0           /* PSP = 中断栈顶部 */
    mrs r0, CONTROL       /* r0 = 当前 CONTROL 值 */
    movs r1, #(2 | CONTROL_ARM_PAC_MASK | CONTROL_ARM_BTI_MASK)
                            /* r1 = SPSEL(1) | PAC 掩码 | BTI 掩码
                             *   bit[1] = 1 → 切换到 PSP
                             *   bit[4:5] = BTI_EN/UBTI_EN（若启用）
                             *   bit[6:7] = PAC_EN/UPAC_EN（若启用） */
    orrs r0, r1           /* r0 |= r1：合并新控制位 */
    msr CONTROL, r0       /* 写回 CONTROL：现在使用 PSP */
    /*
     * ARM 架构要求：修改栈指针选择（SPSEL）后，
     * 必须紧跟 ISB 指令，确保后续指令使用新的栈指针。
     * 否则处理器可能使用旧的栈指针执行后续指令，
     * 导致栈数据损坏（不可预测行为）。
     */
    isb

    /*
     * 使用 'bl' 指令跳转到 z_prep_c()。
     *
     * 为什么用 bl 而非 b？
     *   bl (Branch with Link) 是所有 ARM 平台上跳转范围
     *   最大的分支指令（±16MB offset），能确保在 XIP
     *   等代码位置不确定的配置下正确跳转。
     *   虽然 z_prep_c 不会返回（FUNC_NORETURN），
     *   但链接寄存器 LR 会被破坏，这不影响后续执行。
     */
    bl z_prep_c
