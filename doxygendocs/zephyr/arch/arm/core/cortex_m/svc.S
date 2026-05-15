/**
 * @file svc.S
 * @brief ARM Cortex-M SVC（Supervisor Call）异常处理
 *
 * 该文件实现了 ARM Cortex-M 架构上的 SVC 异常向量处理程序 z_arm_svc()。
 * SVC 指令用于从非特权态（用户态）陷入内核态（特权态），是 Zephyr
 * 用户态系统调用的底层实现机制。
 *
 * SVC 异常的三种用途：
 *   1. SVC #1 — 中断 offload（CONFIG_IRQ_OFFLOAD）
 *   2. SVC #2 — kernel panic/oops（软件触发的致命异常）
 *   3. SVC #3 — 系统调用（CONFIG_USERSPACE）
 *
 * @note SVC #0 未使用。
 *
 * SVC 异常处理流程：
 *   z_arm_svc()
 *     ├── 确定栈帧位置（MSP 或 PSP）
 *     ├── 从栈帧中提取 SVC 号（PC-2 处的低字节）
 *     ├── SVC #1 → z_irq_do_offload() → z_arm_int_exit()
 *     ├── SVC #2 → z_do_kernel_oops()
 *     └── SVC #3 → 构建特权栈帧 → 切换到特权态 → bx lr → z_arm_do_syscall()
 *
 * @copyright Copyright (c) 2013-2014 Wind River Systems, Inc.
 * @copyright Copyright (c) 2017-2019 Nordic Semiconductor ASA
 * @copyright Copyright (c) 2020 Stephanos Ioannidis <root@stephanos.io>
 * @copyright Copyright 2026 Arm Limited and/or its affiliates
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <offsets_short.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/syscall.h>
#include <zephyr/linker/sections.h>

_ASM_FILE_PROLOGUE

GTEXT(z_arm_svc)              /**< SVC 异常处理程序入口 */
GTEXT(z_do_kernel_oops)       /**< kernel oops 处理函数（C 代码） */
#if defined(CONFIG_USERSPACE)
GTEXT(z_arm_do_syscall)       /**< 系统调用分发函数（C 代码） */
#endif

GDATA(_kernel)                /**< 全局内核数据结构 */

/**
 * @brief SVC（Supervisor Call）异常处理程序
 *
 * CPU 执行 SVC 指令时触发的异常处理入口。
 * ARM 硬件在进入 SVC handler 之前自动将以下寄存器压栈：
 *   - Handler mode：压入 MSP（如果 CONFIG_ARMV7_M_ARMV8_M_MAINLINE）
 *   - Thread mode（MSP）：压入 MSP
 *   - Thread mode（PSP）：压入 PSP
 *
 * 硬件压栈顺序（从高地址到低地址）：
 *   xPSR, PC, LR, R12, R3, R2, R1, R0
 *
 * LR 中保存 EXC_RETURN 值，编码了异常返回信息（使用哪个栈、是否带 FPU 等）。
 *
 * @note 该函数通过 bx lr 返回，硬件自动从栈中恢复寄存器并返回到调用者。
 */
SECTION_FUNC(TEXT, z_arm_svc)

  /* ------------------------------------------------
   * 第一步：根据 EXC_RETURN 判断栈帧位于 MSP 还是 PSP。
   *
   * EXC_RETURN 格式中 bit[2] (SPSEL) 指示异常入栈时使用的栈：
   *   - SPSEL=0 → Handler mode，栈帧在 MSP 上
   *   - SPSEL=1 → Thread mode，栈帧在 PSP 上
   * ------------------------------------------------ */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
  /* ARMv6-M / ARMv8-M Baseline：不支持 ite 指令，使用分支实现 */
  movs r0, #_EXC_RETURN_SPSEL_Msk   /* r0 = SPSEL 位掩码 (0x4) */
  mov r1, lr                         /* r1 = EXC_RETURN 值 */
  tst r1, r0                         /* 测试 SPSEL 位是否置位 */
  beq .L_stack_frame_msp              /* SPSEL=0：栈帧在 MSP → 跳转 */
  mrs r0, PSP                        /* SPSEL=1：栈帧在 PSP → r0 = PSP */
  bne .L_stack_frame_endif           /* 跳过 MSP 路径 */
.L_stack_frame_msp:
  mrs r0, MSP                        /* r0 = MSP（栈帧在主栈上） */
.L_stack_frame_endif:
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
  /* ARMv7-M / ARMv8-M Mainline：支持 ite（if-then-else）指令 */
  tst lr, #_EXC_RETURN_SPSEL_Msk     /* 测试 EXC_RETURN 的 SPSEL 位 */
    ite eq                            /* if-then-else：根据 Z 标志选择 */
        mrseq r0, MSP                 /*   Z=1 (SPSEL=0)：Handler mode → r0 = MSP */
        mrsne r0, PSP                 /*   Z=0 (SPSEL=1)：Thread mode → r0 = PSP */
#endif

  /* ------------------------------------------------
   * 第二步：从栈帧中提取 SVC 号。
   *
   * 硬件压栈的栈帧布局（r0 指向栈帧最低地址）：
   *   offset 0:  R0
   *   offset 4:  R1
   *   offset 8:  R2
   *   offset 12: R3
   *   offset 16: R12
   *   offset 20: LR（返回地址）
   *   offset 24: PC（SVC 指令地址）
   *   offset 28: xPSR
   *
   * SVC 指令编码（Thumb 模式，2 字节）：
   *   15:8 = 0xDF（SVC 操作码前缀）
   *    7:0 = SVC 立即数（服务调用号）
   *
   * 所以需要从 PC 指向的地址向前 2 字节处读取 SVC 号。
   * ------------------------------------------------ */

  ldr r1, [r0, #24]  /* r1 = 栈帧中 PC 的值，即 SVC 指令的地址 */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
  /* ARMv6-M 不支持负偏移寻址，需分步计算 */
  subs r1, r1, #2     /* r1 = SVC 指令地址 - 2，指向 SVC 指令首字节 */
  ldrb r1, [r1]       /* r1 = SVC 指令的低字节 = SVC 号 */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
  /* ARMv7-M 支持负偏移寻址 */
  ldrb r1, [r1, #-2]  /* r1 = [PC地址 - 2] 的低字节 = SVC 号 */
#endif

   /* ------------------------------------------------
    * SVC 号分配：
    *   0: 未使用
    *   1: 中断 offload（CONFIG_IRQ_OFFLOAD）
    *   2: kernel oops（软件触发的致命异常）
    *   3: 系统调用（CONFIG_USERSPACE）
    * ------------------------------------------------ */

#if defined(CONFIG_USERSPACE)
    /* ------------------------------------------------
     * 用户态系统调用路径：先检查 SVC 号是否为 3。
     * ------------------------------------------------ */
    mrs r2, CONTROL     /* r2 = CONTROL 寄存器值（用于后续权限检查） */

    cmp r1, #3          /* SVC 号 == 3？ */
    beq .L_do_syscall   /* 是 → 跳转到系统调用处理 */

    /*
     * 非 SVC #3 的情况下，必须验证当前是否处于特权态。
     * 用户态代码只能发起 SVC #3（系统调用），
     * 其他 SVC 号仅允许特权态使用。
     * 如果非特权态尝试使用 SVC #1 或 #2，则触发 oops。
     */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    movs r3, #0x1       /* r3 = 0x1：CONTROL.nPRIV 位掩码 */
    tst r2, r3          /* 测试 nPRIV 位：0=特权态，1=非特权态 */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    tst r2, #0x1        /* 测试 CONTROL.nPRIV 位 */
#endif
    bne .L_oops         /* nPRIV=1（非特权）→ 非法 SVC 调用 → oops */

#endif /* CONFIG_USERSPACE */

    /* ------------------------------------------------
     * SVC #2：kernel oops 路径。
     * 所有特权态的非 #1、非 #3 SVC 调用均走此路径。
     * （实际上只有 SVC #2 会到达此处）
     * ------------------------------------------------ */
    cmp r1, #2          /* SVC 号 == 2？ */
    beq .L_oops         /* 是 → 跳转到 kernel oops 处理 */

    /* ------------------------------------------------
     * SVC #1：中断 offload 路径。
     * 用于在 ISR 上下文中触发需要在线程上下文执行的延迟工作。
     * ------------------------------------------------ */
#if defined(CONFIG_IRQ_OFFLOAD)
    push {r0, lr}       /* 保存栈帧指针(r0)和返回链接(lr)到 MSP */
    bl z_irq_do_offload  /* 调用 C 函数执行 offload 工作 */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    /* ARMv6-M 不支持 pop {r0, lr} 的组合写法 */
    pop {r0, r3}        /* r0 = 栈帧指针，r3 = 原始 lr */
    mov lr, r3          /* 恢复 lr */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    pop {r0, lr}        /* 直接恢复 r0 和 lr */
#endif

#ifdef CONFIG_USE_SWITCH
    /*
     * 使用 context switch 实现（CONFIG_USE_SWITCH）。
     * 调用 arm_m_legacy_exit 以便 offload 工作完成后
     * 正确切换回被中断的线程。
     *
     * FIXME：此处使用的是 pre-hook 的 LR，但应该从栈帧中
     * 重新获取正确的 LR，否则 irq_offload() 的上下文切换
     * 可能不正确（目前未被测试覆盖）。
     */
    push {r0, lr}       /* 再次保存 r0 和 lr */
    bl arm_m_legacy_exit /* 调用上下文切换退出函数 */
    pop {r0, lr}        /* 恢复 r0 和 lr */
    bx lr               /* 返回（通过 EXC_RETURN 自动恢复栈帧） */
#else
    /* 非context switch 实现：直接跳转到中断退出处理 */
    ldr r0, =z_arm_int_exit  /* r0 = z_arm_int_exit 函数地址 */
    bx r0               /* 跳转到 z_arm_int_exit */
#endif

#endif /* CONFIG_IRQ_OFFLOAD */

    /* ------------------------------------------------
     * .L_oops：kernel oops 处理路径。
     * 软件触发的致命异常，用于不可恢复的错误报告。
     * ------------------------------------------------ */
.L_oops:
    push {r0, lr}       /* 保存栈帧指针(r0)和 EXC_RETURN(lr) */
#if defined(CONFIG_EXTRA_EXCEPTION_INFO)
    /*
     * 构建 _callee_saved_t 结构体用于调试信息收集。
     * 结构体定义要求依次存放 PSP, padding, r11-r4。
     * 这些寄存器值来自被中断线程的上下文。
     */
    mrs r1, PSP         /* r1 = 当前 PSP（被中断线程的栈指针） */
    push {r1, r2}       /* 压入 PSP 和 r2（r2 作为 padding） */
#if defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    push {r4-r11}       /* 压入 callee-saved 寄存器 r4-r11 */
#elif defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    /* ARMv6-M 的 push 只能操作低 8 个寄存器，需分批压栈 */
    mov  r1, r10        /* r1 = r10 */
    mov  r2, r11        /* r2 = r11 */
    push {r1, r2}       /* 压入 r10, r11 */
    mov  r1, r8         /* r1 = r8 */
    mov  r2, r9         /* r2 = r9 */
    push {r1, r2}       /* 压入 r8, r9 */
    push {r4-r7}        /* 压入 r4, r5, r6, r7 */
#else
#error Unknown ARM architecture
#endif
    mov  r1, sp         /* r1 = 指向 _callee_saved_t 结构体的指针 */
#endif /* CONFIG_EXTRA_EXCEPTION_INFO */
    mov r2, lr          /* r2 = EXC_RETURN 值（传给 oops handler） */
    bl z_do_kernel_oops /* 调用 C 函数处理 kernel oops */

    /* SVC 异常从 z_do_kernel_oops 返回后的恢复逻辑 */
#if defined(CONFIG_EXTRA_EXCEPTION_INFO)
    /*
     * 此处不需要恢复任何 callee-saved 寄存器，
     * 因为在 SVC handler 中未修改它们。
     * 只需将 MSP 跳过之前压入的 _callee_saved_t 大小即可。
     *
     * _callee_saved_t 布局：PSP(4) + padding(4) + r4-r11(32) = 40 字节
     */
    add sp, #40         /* MSP 跳过 _callee_saved_t 结构体 */
#endif /* CONFIG_EXTRA_EXCEPTION_INFO */

#ifdef CONFIG_USE_SWITCH
    bl arm_m_legacy_exit /* 通过 context switch 实现退出 */
#endif

    pop {r0, pc}        /* 恢复 r0（栈帧指针），并跳转到 lr (EXC_RETURN) */

#if defined(CONFIG_USERSPACE)
    /* ================================================================
     * .L_do_syscall：系统调用处理路径（仅 CONFIG_USERSPACE）
     *
     * 系统调用通过 SVC #3 从用户态陷入内核态。
     *
     * 核心思路：在特权栈上构建一个新的异常栈帧，
     * 修改返回地址为 z_arm_do_syscall()。这样当 SVC 异常返回时，
     * 硬件会自动从特权栈弹出栈帧，并跳转到 z_arm_do_syscall()，
     * 实现从 SVC handler 到 C 系统调用处理函数的过渡。
     *
     * 进入此路径时的寄存器约定（由编译器在调用 SVC 前设置）：
     *   r0 = arg1（第 1 个参数）
     *   r1 = arg2（第 2 个参数）
     *   r2 = arg3（第 3 个参数）
     *   r3 = arg4（第 4 个参数）
     *   r4 = arg5（第 5 个参数）
     *   r5 = arg6（第 6 个参数）
     *   r6 = call_id（系统调用 ID）
     *   r8 = saved link register（保存的返回地址）
     *
     * SVC 异常硬件压栈的栈帧布局（用户栈上，r0 指向最低地址）：
     *   offset 0:  R0
     *   offset 4:  R1
     *   offset 8:  R2
     *   offset 12: R3
     *   offset 16: R12
     *   offset 20: LR（返回地址）
     *   offset 24: PC（SVC 指令地址）
     *   offset 28: xPSR
     *   [可选] offset 32: 4字节对齐 pad（当 PSP 未 8 字节对齐时）
     *   [可选] FP 寄存器 + FPSCR（CONFIG_FPU_SHARING 时）
     *
     * 构建在特权栈上的布局：
     *   +------+----------------------------+
     *   |偏移 | 内容                        |
     *   +------+----------------------------+
     *   |  0   | R0                          |  <- PSP 将指向此处
     *   |  4   | R1                          |
     *   |  8   | R2                          |
     *   | 12   | R3                          |
     *   | 16   | R12                         |
     *   | 20   | LR（来自用户栈帧）          |
     *   | 24   | z_arm_do_syscall 地址       |  <- 替换原始 PC
     *   | 28   | xPSR（pad bit 已清除）     |
     *   [可选 FP 寄存器 + FPSCR]
     *   | --   | 原始用户 SP 值（pre-SVC）  |  <- z_arm_do_syscall 读取
     *   | --   | SVC 返回地址（SVC 下一条）  |  <- z_arm_do_syscall 读取
     *   +------+----------------------------+
     * ================================================================ */
.L_do_syscall:

  /* ------------------------------------------------
   * 计算特权栈顶部地址。
   * 特权栈位于当前线程的 thread->priv_stack_start 区域，
   * 大小为 CONFIG_PRIVILEGED_STACK_SIZE。
   * ------------------------------------------------ */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    ldr r1, =_kernel                      /* r1 = _kernel 结构体地址 */
    ldr r1, [r1, #_kernel_offset_to_current]  /* r1 = _kernel.current（当前线程指针） */
    adds r1, r1, #_thread_offset_to_priv_stack_start  /* r1 = &thread->priv_stack_start */
    ldr r1, [r1]                           /* r1 = priv_stack_start（特权栈底部） */
    ldr r3, =CONFIG_PRIVILEGED_STACK_SIZE  /* r3 = 特权栈大小 */
    subs r3, #(_EXC_HW_SAVED_BASIC_SF_SIZE+8)  /* r3 = 栈大小 - 基本栈帧(32) - 8(原始sp+pc) */
    add r1, r3                             /* r1 = 特权栈中特权栈帧的起始偏移 */
    mov ip, r1                             /* ip(r12) = 特权栈帧起始地址 */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    ldr ip, =_kernel                      /* ip = _kernel 结构体地址 */
    ldr ip, [ip, #_kernel_offset_to_current]  /* ip = 当前线程指针 */
    ldr ip, [ip, #_thread_offset_to_priv_stack_start]  /* ip = priv_stack_start（特权栈底部） */
    add ip, #CONFIG_PRIVILEGED_STACK_SIZE  /* ip = 特权栈顶部 */

#ifdef CONFIG_FPU_SHARING
    /*
     * 检查触发 SVC 的线程是否正在使用 FPU 寄存器。
     * EXC_RETURN 的 bit[4] (FTYPE) 指示异常入栈时是否包含
     * 扩展栈帧（FP 寄存器 + FPSCR）。
     *   FTYPE=0 → 包含扩展栈帧（S0-S15 + FPSCR + reserved）
     *   FTYPE=1 → 不包含扩展栈帧（仅基本栈帧）
     */
    tst lr, #_EXC_RETURN_FTYPE_Msk        /* 测试 FTYPE 位 */
    ite eq                                 /* if-then-else */
        moveq r8, #_EXC_HW_SAVED_EXTENDED_SF_SIZE  /* FTYPE=0：r8 = 扩展栈帧大小(72+8=104) */
        movne r8, #_EXC_HW_SAVED_BASIC_SF_SIZE    /* FTYPE=1：r8 = 基本栈帧大小(32) */
#else
    /* 不支持 FPU sharing：始终使用基本栈帧大小 */
    mov r8, #_EXC_HW_SAVED_BASIC_SF_SIZE  /* r8 = 32（基本栈帧：8 寄存器 × 4 字节） */
#endif
    sub ip, #8          /* ip -= 8：为 z_arm_do_syscall 预留存放原始 sp 和返回地址的空间 */
    sub ip, r8          /* ip -= 栈帧大小：跳过异常栈帧区域 */
#endif

    /*
     * 此时寄存器状态：
     *   r0 = 用户 PSP（指向用户栈帧底部）
     *   ip = 特权栈帧目标地址（将要写入特权栈帧的位置）
     *   r8 = 硬件保存的栈帧大小（仅 mainline 有意义）
     *   r2 = CONTROL 寄存器值（之前 msr r2, CONTROL 读取的）
     *   r6 = 系统调用 ID
     */

    /* 保存 callee-saved 寄存器到 MSP（SVC handler 的调用者保存） */
    push {r4-r7}        /* 保存 r4, r5, r6, r7 到 MSP（r6 含 syscall ID） */
    push {r2}           /* 保存 CONTROL 值到 MSP */

#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    /* ---- ARMv6-M / ARMv8-M Baseline 系统调用处理 ---- */
    mov  r2, r0         /* r2 = 用户 PSP（备份，r0 仍保留原始值） */

    /* 检查用户栈帧中是否存在对齐 pad。
     * xPSR 的 bit[9] 为硬件栈对齐自动插入的 pad 标志：
     *   pad=1 → 入栈前 PSP 不是 8 字节对齐的，栈帧中多 4 字节 pad
     *   pad=0 → PSP 已对齐，无额外 pad
     */
    ldr  r1, [r0, #_EXC_HW_SAVED_BASIC_SF_XPSR_OFFSET]  /* r1 = 用户栈帧中的 xPSR */
    movs r3, #1         /* r3 = 1 */
    lsls r3, r3, #9     /* r3 = 1 << 9 = 0x200（pad 位掩码） */
    tst  r1, r3         /* 测试 xPSR 的 pad 位 */
    beq  .L_no_padding  /* 无 pad → 跳过 */
    /* 有 pad：清除 pad 位（特权栈是对齐的，不需要 pad 标志） */
    bics r1, r3         /* r1 的 bit[9] 清零 */
    adds r2, #4         /* r2 += 4：跳过 pad 字节 */
.L_no_padding:
    /* 计算原始 pre-SVC 用户 SP = PSP + 基本栈帧大小（+4 如果有 pad） */
    adds r2, #_EXC_HW_SAVED_BASIC_SF_SIZE  /* r2 = 原始用户 SP */

    mov  r3, ip         /* r3 = 特权栈帧目标地址（ip） */
    str r2, [r3, #0]   /* 特权栈帧偏移 0：存储原始用户 SP */

    /* 存储原始用户 SP 到特权栈帧中 z_arm_do_syscall 期望的偏移处 */
    str r2, [r3, #_EXC_HW_SAVED_BASIC_SF_SIZE]

    /* 将 xPSR（pad 位已清除）写入特权栈帧 */
    str r1, [r3, #_EXC_HW_SAVED_BASIC_SF_XPSR_OFFSET]

    /* 从用户栈帧复制 R0-R3, R12, LR 到特权栈帧 */
    mov r1, r0          /* r1 = 用户栈帧源地址 */
    mov r2, r3          /* r2 = 特权栈帧目标地址 */
    ldmia r1!, {r4-r7}  /* 从用户栈帧加载 R0-R3（r1 自动递增） */
    stmia r2!, {r4-r7}  /* 存储到特权栈帧（r2 自动递增） */
    ldmia r1!, {r4-r5}  /* 从用户栈帧加载 R12, LR */
    stmia r2!, {r4-r5}  /* 存储到特权栈帧 */

    /* 存储原始用户返回地址到特权栈帧中 z_arm_do_syscall 期望的位置 */
    str r5, [r3, #(_EXC_HW_SAVED_BASIC_SF_SIZE+4)]

    /* 将特权栈帧的返回地址替换为 z_arm_do_syscall */
    ldr r1, =z_arm_do_syscall       /* r1 = z_arm_do_syscall 函数地址 */
    str r1, [r3, #_EXC_HW_SAVED_BASIC_SF_RETADDR_OFFSET]  /* 写入特权栈帧的 PC 位置 */

    /* 从用户栈帧中取出原始返回地址（SVC 的下一条指令地址） */
    ldr r1, [r0, #_EXC_HW_SAVED_BASIC_SF_RETADDR_OFFSET]  /* r1 = 用户栈帧中的 PC */
    /* 将原始返回地址存入 r8，z_arm_do_syscall 需要此值来恢复用户态返回 */
    mov r8, r1          /* r8 = SVC 下一条指令的地址 */

#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    /* ---- ARMv7-M / ARMv8-M Mainline 系统调用处理 ---- */
    mov  r2, r0         /* r2 = 用户 PSP（备份） */

    /* 计算原始 pre-SVC 用户 SP（不含 pad）= PSP + 硬件栈帧大小 */
    add r2, r8          /* r2 = PSP + r8（r8 = 栈帧大小） */

    /* 检查用户栈帧中是否存在对齐 pad */
    ldr  r1, [r0, #_EXC_HW_SAVED_BASIC_SF_XPSR_OFFSET]  /* r1 = 用户栈帧中的 xPSR */
    tst r1, #(1<<9)     /* 测试 xPSR bit[9]（pad 标志） */
    beq .L_no_padding   /* 无 pad → 跳过 */
    bics r1, #(1<<9)    /* 有 pad：清除 bit[9]（特权栈对齐，不需要 pad） */
    add  r2, #4         /* r2 += 4：补偿 pad 字节 */
.L_no_padding:
    str r2, [ip, #0]   /* 特权栈帧偏移 0：存储原始用户 SP */

    /* 存储原始用户 SP 到 z_arm_do_syscall 期望的偏移处 */
    str r2, [ip, r8]    /* ip + r8 = 特权栈帧之后的位置 */

    /* 存储 xPSR（pad 位已清除）到特权栈帧 */
    str r1, [ip, #_EXC_HW_SAVED_BASIC_SF_XPSR_OFFSET]

    /* 从用户栈帧复制 R0-R3, R12, LR 到特权栈帧 */
    mov r1, r0          /* r1 = 用户栈帧源地址 */
    mov r2, ip          /* r2 = 特权栈帧目标地址 */
    ldmia r1!, {r4-r7}  /* 加载 R0-R3 */
    stmia r2!, {r4-r7}  /* 存储 R0-R3 */
    ldmia r1!, {r4-r5}  /* 加载 R12, LR */
    stmia r2!, {r4-r5}  /* 存储 R12, LR */

    /* 存储原始用户返回地址到 z_arm_do_syscall 期望的位置 */
    add r8, #4          /* r8 += 4：跳过基本栈帧到达返回地址位置 */
    str r5, [ip, r8]    /* 存储原始返回地址 */

    /* 将特权栈帧的 PC 替换为 z_arm_do_syscall */
    ldr r1, =z_arm_do_syscall       /* r1 = z_arm_do_syscall 地址 */
    str r1, [ip, #_EXC_HW_SAVED_BASIC_SF_RETADDR_OFFSET]  /* 替换特权栈帧的 PC */

    /* 获取原始用户返回地址（SVC 的下一条指令）并存入 r8 */
    ldr r1, [r0, #_EXC_HW_SAVED_BASIC_SF_RETADDR_OFFSET]  /* r1 = 用户栈帧中的 PC */
    mov r8, r1          /* r8 = SVC 下一条指令地址 */

    /* 基本栈帧已复制完毕，现在复制 FPU 上下文（如果存在） */
#ifdef CONFIG_FPU_SHARING
    tst lr, #_EXC_RETURN_FTYPE_Msk        /* 检查 FTYPE：是否有扩展栈帧 */
    bne .L_skip_fp_copy                   /* FTYPE=1 → 无 FPU 上下文 → 跳过 */
    /* FTYPE=0 → 有 FPU 上下文，需要复制 */
    add r1, r0, #32       /* r1 = 用户栈帧中 FPU 区域起始（跳过基本栈帧 32 字节） */
    add r2, ip, #32       /* r2 = 特权栈帧中 FPU 区域起始 */
    vldmia r1!, {s0-s15}  /* 从用户栈帧加载 S0-S15（16 个单精度浮点寄存器） */
    vstmia r2!, {s0-s15}  /* 存储到特权栈帧 */
    /* 复制 FPSCR + reserved（8 字节，使用通用寄存器拷贝） */
    ldmia   r1!, {r4, r5} /* 加载 FPSCR + reserved */
    stmia   r2!, {r4, r5} /* 存储 FPSCR + reserved */
.L_skip_fp_copy:
#endif

#endif

    /* 恢复之前保存的 callee-saved 寄存器 */
    pop {r2}            /* r2 = CONTROL 寄存器值（之前 push 的） */
    pop {r4-r7}         /* 恢复 r4-r7（r6 恢复为 syscall ID） */

    /* ------------------------------------------------
     * 将 PSP 切换到特权栈帧。
     * SVC 异常返回时，硬件将从 PSP 弹出栈帧，
     * 由于 PSP 现在指向特权栈帧，将跳转到
     * z_arm_do_syscall 而非原始用户代码。
     * ------------------------------------------------ */
    msr PSP, ip         /* PSP = ip（特权栈帧地址） */

#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    /* 验证系统调用 ID 是否在合法范围内 */
    ldr r3, =K_SYSCALL_LIMIT  /* r3 = 系统调用 ID 上限 */
    cmp r6, r3          /* r6（syscall ID）< r3（上限）？ */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    /* 验证系统调用 ID 范围（无符号比较） */
    ldr ip, =K_SYSCALL_LIMIT  /* ip = 系统调用 ID 上限 */
    cmp r6, ip          /* 比较 syscall ID 与上限 */
#endif
    /*
     * 系统调用 ID 必须小于上限（无符号比较）。
     * blo = branch if lower（无符号小于）。
     */
    blo .L_valid_syscall_id  /* 合法 ID → 跳转 */

    /* ------------------------------------------------
     * 非法系统调用 ID：将 bad ID 作为 arg1 传递，
     * 并将 call_id 设置为 K_SYSCALL_BAD。
     * z_arm_do_syscall 会据此返回错误。
     * ------------------------------------------------ */
    str r6, [r0]        /* r0 仍指向用户栈帧底部，将 bad ID 存入 R0 位置（arg1） */
    ldr r6, =K_SYSCALL_BAD  /* r6 = K_SYSCALL_BAD（非法系统调用标记） */

    /* 非法系统调用作为合法系统调用处理，ID 设为 K_SYSCALL_BAD */
.L_valid_syscall_id:
    /* ------------------------------------------------
     * 切换当前线程到特权模式。
     *
     * 步骤：
     *   1. 将 thread->mode 中的 nPRIV 位清除（记录为特权态）
     *   2. 清除 CONTROL.nPRIV 位（实际切换到特权态）
     *   3. 执行 ISB 确保后续指令以特权态执行
     * ------------------------------------------------ */
    ldr r0, =_kernel                          /* r0 = _kernel 地址 */
    ldr r0, [r0, #_kernel_offset_to_current]  /* r0 = 当前线程指针 */
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    mov ip, r2         /* ip = r2（备份 CONTROL 值，r2 即将被修改） */
    ldr r1, =_thread_offset_to_mode           /* r1 = mode 字段偏移 */
    ldr r3, [r0, r1]   /* r3 = thread->mode 当前值 */
    movs r2, #1        /* r2 = 0x1（nPRIV 位掩码） */
    bics r3, r2        /* r3 的 nPRIV 位清零（标记为特权态） */
    /* 将特权模式状态存入线程的 mode 变量 */
    str r3, [r0, r1]   /* thread->mode = r3（特权态） */
    mov r2, ip         /* 恢复 CONTROL 值到 r2 */
    dsb                 /* 数据同步屏障：确保 mode 写入对其他核心可见 */
    /* 清除 CONTROL.nPRIV 位，实际切换到特权态 */
    movs r3, #1        /* r3 = 0x1 */
    bics r2, r3        /* r2 的 bit[0] 清零 → 特权态 */
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    ldr r1, [r0, #_thread_offset_to_mode]  /* r1 = thread->mode */
    bic r1, #1         /* 清除 nPRIV 位（标记为特权态） */
    /* 将特权模式状态存入线程的 mode 变量 */
    str r1, [r0, #_thread_offset_to_mode]  /* thread->mode = 特权态 */
    dsb                 /* 数据同步屏障 */
    /* 清除 CONTROL.nPRIV 位，实际切换到特权态 */
    bic r2, #1         /* r2 的 bit[0] 清零 → 特权态 */
#endif
    msr CONTROL, r2    /* CONTROL = r2（nPRIV=0，进入特权态） */

    /*
     * ISB 并非严格必需（此处未修改栈指针），
     * 但 ARM 建议在修改特权级别后执行 ISB，
     * 以避免使用先前特权级预取的指令执行。
     */
    isb                 /* 指令同步屏障：清空预取队列 */

#if defined(CONFIG_BUILTIN_STACK_GUARD)
    /*
     * 设置进程栈指针限制（PSPLIM）。
     * 仅在特权模式下才能写入 PSPLIM。
     * 将限制设为特权栈底部，防止栈溢出到其他内存区域。
     */
    ldr ip, =_kernel                          /* ip = _kernel 地址 */
    ldr ip, [ip, #_kernel_offset_to_current]  /* ip = 当前线程指针 */
    ldr ip, [ip, #_thread_offset_to_priv_stack_start]  /* ip = 特权栈底部 */
    msr PSPLIM, ip     /* PSPLIM = 特权栈底部地址 */
#endif

#ifdef CONFIG_USE_SWITCH
    /*
     * 使用 context switch 实现时，在返回 SVC 之前需要获取调度锁。
     * 通过屏蔽中断来隐式获取锁（与中断锁定级别一致）。
     */
#if defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    mov r0, #_EXC_IRQ_DEFAULT_PRIO  /* r0 = 默认中断优先级掩码 */
    msr BASEPRI, r0     /* 屏蔽低于默认优先级的中断 */
#else
    cpsid i             /* ARMv6-M：全局关闭中断（PRIMASK=1） */
#endif
#endif

    /* ------------------------------------------------
     * 从 SVC 异常返回。
     *
     * bx lr 触发异常返回机制：
     *   - 硬件从 PSP（已指向特权栈帧）弹出栈帧
     *   - 栈帧中的 PC = z_arm_do_syscall
     *   - 因此跳转到 z_arm_do_syscall() 继续执行
     *
     * z_arm_do_syscall() 执行完毕后负责恢复用户栈、
     * 切回用户态、并返回到 SVC 的下一条指令。
     * ------------------------------------------------ */
    bx lr               /* 异常返回 → 弹出特权栈帧 → 跳转 z_arm_do_syscall */
#endif /* CONFIG_USERSPACE */
