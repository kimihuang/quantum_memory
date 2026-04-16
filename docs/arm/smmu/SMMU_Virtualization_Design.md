# ARM SMMU 虚拟化场景应用与设计

## 1. 概述

ARM SMMU 在虚拟化场景中扮演着关键角色，它通过两阶段地址翻译（Two-Stage Translation）机制，使 Hypervisor 能够隔离和控制 Guest OS 对物理内存的访问，同时支持设备直通（Passthrough）和半虚拟化等多种虚拟化模式。

### 1.1 虚拟化中的核心问题

在虚拟化环境中，Guest OS 使用的是**虚拟地址（VA）**，通过自己的页表翻译为**中间物理地址（IPA）**。Guest OS 以为 IPA 就是最终的物理地址（PA），但实际上 Hypervisor 通过第二阶段页表将 IPA 再次翻译为真正的**物理地址（PA）**。

```mermaid
graph LR
    subgraph NoVirt["无虚拟化"]
        VA1["VA"] -->|"Stage 1<br/>(OS 页表)"| PA1["PA"]
    end

    subgraph WithVirt["有虚拟化"]
        VA2["VA"] -->|"Stage 1<br/>(Guest 页表)"| IPA["IPA"] -->|"Stage 2<br/>(Hypervisor 页表)"| PA2["PA"]
    end
```

---

## 2. 两阶段翻译架构

### 2.1 翻译阶段定义

```mermaid
graph TB
    subgraph Stages["SMMU 两阶段翻译"]
        direction LR

        subgraph Stage1["Stage 1 (Guest 管理)"]
            direction TB
            S1_IN["输入: VA<br/>(设备虚拟地址)"]
            S1_PT["Guest 页表<br/>(由 Guest OS 维护)"]
            S1_OUT["输出: IPA<br/>(中间物理地址)"]
            S1_IN --> S1_PT --> S1_OUT
        end

        subgraph Stage2["Stage 2 (Hypervisor 管理)"]
            direction TB
            S2_IN["输入: IPA"]
            S2_PT["Hypervisor 页表<br/>(由 KVM 维护)"]
            S2_OUT["输出: PA<br/>(真实物理地址)"]
            S2_IN --> S2_PT --> S2_OUT
        end

        S1_OUT --> S2_IN
    end
```

### 2.2 翻译模式分类

SMMU 支持四种翻译模式，由 STE 中的 `Config` 字段决定（`arm-smmu-v3.h:204-208`）：

| Config 值 | 模式 | 说明 | 典型用途 |
|-----------|------|------|----------|
| 0b000 | Abort | 中止所有事务 | 安全隔离 |
| 0b100 | S1+S2 Bypass | 两阶段均旁路 | 调试 |
| 0b101 | S1 Translation | 仅 Stage 1 | 非 virtual化 S1 域 |
| 0b110 | S2 Translation | 仅 Stage 2 | 设备直通 |
| 0b111 | S1+S2 Translation | 嵌套翻译 | vIOMMU / 嵌套虚拟化 |

### 2.3 Linux 驱动中的 Domain 类型

Linux 驱动定义了四种 Domain 阶段类型（`arm-smmu-v3.h:702-707`）：

| 类型 | 枚举值 | 说明 | 虚拟化场景 |
|------|--------|------|------------|
| Stage 1 | `ARM_SMMU_DOMAIN_S1` | VA → PA | 非虚拟化 / Host DMA |
| **Stage 2** | `ARM_SMMU_DOMAIN_S2` | **IPA → PA** | **设备直通** |
| **Nested** | `ARM_SMMU_DOMAIN_NESTED` | **VA → IPA → PA** | **vIOMMU** |
| Bypass | `ARM_SMMU_DOMAIN_BYPASS` | 直接透传 | 调试 |

---

## 3. 虚拟化场景详解

### 3.1 场景总览

```mermaid
graph TB
    subgraph Hypervisor["Hypervisor (Host)"]
        KVM["KVM"]
        HOST_SMMU_DRV["Host SMMUv3 驱动"]
        HOST_IOMMU["Host IOMMU 子系统"]
        VFIO["VFIO / vfio-pci"]
    end

    subgraph GuestVM["Guest VM"]
        GUEST_OS["Guest OS (Linux)"]
        GUEST_DRV["Guest SMMU 驱动"]
        GUEST_IOMMU["Guest IOMMU 子系统"]
        GUEST_DEV_DRV["Guest 设备驱动"]
    end

    subgraph Hardware["硬件"]
        SMMU["SMMU (TBU + TCU)"]
        DEV["PCIe 设备<br/>(直通给 Guest)"]
        MEM["物理内存"]
    end

    KVM --> HOST_IOMMU
    HOST_IOMMU --> HOST_SMMU_DRV
    HOST_SMMU_DRV --> SMMU
    VFIO --> HOST_IOMMU

    GUEST_DEV_DRV --> GUEST_IOMMU
    GUEST_IOMMU --> GUEST_DRV
    GUEST_DRV -.->|"IOCTL / virtio"| HOST_SMMU_DRV

    DEV --> SMMU
    SMMU --> MEM
```

### 3.2 场景一：设备直通（Passthrough）

设备直通是将物理设备直接分配给 Guest VM 使用，Hypervisor 通过 Stage 2 翻译控制设备对物理内存的访问。

```mermaid
sequenceDiagram
    participant QEMU as QEMU / Virt Manager
    participant VFIO as VFIO
    participant IOMMU as Host IOMMU
    participant SMMU as SMMU 硬件
    participant DEV as 直通设备

    QEMU->>VFIO: 分配设备给 VM
    VFIO->>IOMMU: 创建 S2 Domain
    Note over IOMMU: arm_smmu_domain_alloc()<br/>stage = ARM_SMMU_DOMAIN_S2
    IOMMU->>IOMMU: 分配 VMID
    Note over IOMMU: arm_smmu_bitmap_alloc(vmid_map)
    IOMMU->>SMMU: 配置 Stage 2 页表
    Note over SMMU: VTTBR = Guest GPA→HPA 映射表<br/>VTCR = 翻译控制参数<br/>VMID = 分配的 ID
    IOMMU->>SMMU: 写入 STE (Config=S2_TRANS)
    VFIO->>DEV: 使能设备

    Note over DEV: 设备发起 DMA
    DEV->>SMMU: DMA 事务 (IPA)
    SMMU->>SMMU: Stage 2 翻译 IPA → PA
    SMMU->>SMMU: 访问物理内存

    Note over QEMU: VM 退出 / 内存热插拔
    QEMU->>VFIO: 通知 GPA 映射变更
    VFIO->>IOMMU: 更新 Stage 2 页表
    IOMMU->>SMMU: TLB 无效化
    Note over SMMU: CMDQ_OP_TLBI_S2_IPA<br/>或 CMDQ_OP_TLBI_S12_VMALL
```

**Stage 2 配置数据结构**（`arm-smmu-v3.h:601-605`）：

```c
struct arm_smmu_s2_cfg {
    u16     vmid;      // 虚拟机标识符
    u64     vttbr;     // Stage 2 页表基址
    u64     vtcr;      // Stage 2 翻译控制寄存器
};
```

**Stage 2 页表初始化流程**（`arm-smmu-v3.c:2138-2161`）：

```mermaid
flowchart TD
    START["arm_smmu_domain_finalise_s2()"]
    START --> ALLOC_VMID["从 vmid_map 分配 VMID<br/>arm_smmu_bitmap_alloc()"]
    ALLOC_VMID --> EXTRACT["从 pgtbl_cfg 提取 VTCR 参数<br/>tsz, sl, irgn, orgn, sh, tg, ps"]
    EXTRACT --> BUILD["构建 VTCR 编码值<br/>FIELD_PREP(STRTAB_STE_2_VTCR_*, ...)"]
    BUILD --> STORE["存储到 s2_cfg<br/>cfg->vmid, cfg->vttbr, cfg->vtcr"]
    STORE --> DONE["完成"]
```

### 3.3 场景二：vIOMMU（嵌套翻译）

vIOMMU 允许 Guest OS 直接管理自己的 IOMMU（包括 Stage 1 翻译），同时 Hypervisor 通过 Stage 2 保证隔离。这是最完整的虚拟化 IOMMU 方案。

```mermaid
graph TB
    subgraph GuestWorld["Guest 世界"]
        direction TB
        G_OS["Guest OS"]
        G_IOMMU["Guest IOMMU 驱动"]
        G_PT["Guest IOVA 页表<br/>(Stage 1: VA→IPA)"]
        G_DOM["Guest IOMMU Domain<br/>(Stage 1)"]
    end

    subgraph HostWorld["Host 世界"]
        direction TB
        H_KVM["KVM"]
        H_IOMMU["Host SMMU 驱动"]
        H_PT["Host GPA 页表<br/>(Stage 2: IPA→PA)"]
        H_DOM["Host IOMMU Domain<br/>(Nested: S1+S2)"]
    end

    subgraph HW["硬件"]
        DEV["直通设备"]
        SMMU_HW["SMMU"]
        MEM["物理内存"]
    end

    G_OS --> G_IOMMU --> G_DOM --> G_PT
    G_IOMMU -.->|"配置 CD/STE<br/>(Stage 1 参数)"| H_IOMMU

    H_KVM --> H_IOMMU --> H_DOM --> H_PT

    DEV -->|"DMA (VA)"| SMMU_HW
    SMMU_HW -->|"Stage 1 查询 G_PT<br/>VA→IPA"| SMMU_HW
    SMMU_HW -->|"Stage 2 查询 H_PT<br/>IPA→PA"| SMMU_HW
    SMMU_HW --> MEM
```

**嵌套域使能流程**（`arm-smmu-v3.c:2733-2746`）：

```mermaid
flowchart TD
    REQ["请求使能嵌套翻译<br/>arm_smmu_enable_nesting()"]
    REQ --> LOCK["获取 init_mutex"]
    LOCK --> CHECK{"smmu 已关联?"}
    CHECK -->|"是"| REJECT["返回 -EPERM<br/>(已初始化不可更改)"]
    CHECK -->|"否"| SET["设置 stage =<br/>ARM_SMMU_DOMAIN_NESTED"]
    SET --> UNLOCK["释放 init_mutex"]
    UNLOCK --> DONE["完成<br/>(待 attach 时完成实际配置)"]
```

**嵌套模式下 STE 配置**（`arm-smmu-v3.c:1255-1378`）：

当 Domain 阶段为 `NESTED` 时，STE 同时配置 Stage 1 和 Stage 2 参数：

```mermaid
graph LR
    subgraph STE_Nested["STE 嵌套配置"]
        direction TB
        subgraph Word0["STE Word 0"]
            N_CFG["Config = S1+S2 Translation"]
            N_S1CTXPTR["S1 Context Pointer<br/>(指向 CD Table)"]
        end
        subgraph Word1["STE Word 1"]
            N_STRW["STRW = EL2<br/>(E2H 模式)"]
            N_S1C["S1 缓存属性"]
            N_EATS["EATS = TRANS"]
        end
        subgraph Word2["STE Word 2"]
            N_S2VMID["S2VMID"]
            N_VTCR["VTCR<br/>(S2 页表格式)"]
            N_S2PTW["S2PTW = 1"]
            N_S2AA64["S2AA64 = 1"]
            N_S2R["S2R = 1"]
        end
        subgraph Word3["STE Word 3"]
            N_S2TTB["S2TTB<br/>(Stage 2 页表基址)"]
        end
    end
```

### 3.4 场景对比

```mermaid
graph TB
    subgraph Passthrough["设备直通 (S2 Only)"]
        direction TB
        P_DEV["直通设备"]
        P_DMA["DMA: IPA"]
        P_S2["Stage 2: IPA→PA<br/>(Hypervisor 管理)"]
        P_MEM["物理内存"]
        P_DEV --> P_DMA --> P_S2 --> P_MEM
        P_NOTE["Guest 感知: 设备可直接访问<br/>Guest 无需 IOMMU 驱动<br/>性能最优, 隔离依赖 S2"]
    end

    subgraph Viommu["vIOMMU (Nested S1+S2)"]
        direction TB
        V_DEV["直通设备"]
        V_DMA["DMA: VA"]
        V_S1["Stage 1: VA→IPA<br/>(Guest OS 管理)"]
        V_S2["Stage 2: IPA→PA<br/>(Hypervisor 管理)"]
        V_MEM["物理内存"]
        V_DEV --> V_DMA --> V_S1 --> V_S2 --> V_MEM
        V_NOTE["Guest 感知: 完整 IOMMU 控制<br/>Guest 运行 IOMMU 驱动<br/>灵活性最高, 性能有开销"]
    end

    subgraph Emulated["模拟设备 (纯软件)"]
        direction TB
        E_DEV["模拟设备<br/>(virtio)"]
        E_VIRTIO["virtio 数据通路<br/>(经由 Hypervisor)"]
        E_S1["Stage 1: IOVA→PA<br/>(Host S1 Domain)"]
        E_MEM["物理内存"]
        E_DEV --> E_VIRTIO --> E_S1 --> E_MEM
        E_NOTE["Guest 感知: 标准 virtio 设备<br/>无需直通, 可迁移<br/>性能中等"]
    end
```

---

## 4. VMID 管理机制

### 4.1 VMID 在虚拟化中的角色

VMID（Virtual Machine Identifier）用于区分不同 Guest VM 的 Stage 2 地址空间。SMMU 使用 VMID 为每个 VM 维护独立的 TLB 条目，避免 VM 切换时的全局 TLB 刷新。

```mermaid
graph TB
    subgraph VMs["多个虚拟机"]
        VM1["VM 1<br/>VMID=1"]
        VM2["VM 2<br/>VMID=2"]
        VM3["VM 3<br/>VMID=3"]
    end

    subgraph SMMU_TLB["SMMU TLB"]
        subgraph Entry1["TLB 条目 1"]
            T1_IPA["IPA → PA"]
            T1_VMID1["VMID=1"]
        end
        subgraph Entry2["TLB 条目 2"]
            T2_IPA["IPA → PA"]
            T2_VMID2["VMID=2"]
        end
        subgraph Entry3["TLB 条目 3"]
            T3_IPA["IPA → PA"]
            T3_VMID3["VMID=3"]
        end
    end

    VM1 -->|"DMA"| Entry1
    VM2 -->|"DMA"| Entry2
    VM3 -->|"DMA"| Entry3
```

### 4.2 VMID 分配与释放

Linux 驱动使用位图管理 VMID（`arm-smmu-v3.h:664-665`）：

```mermaid
flowchart TD
    subgraph Alloc["VMID 分配"]
        A_REQ["创建 S2 Domain"] --> A_CALL["arm_smmu_domain_finalise_s2()"]
        A_CALL --> A_BITMAP["arm_smmu_bitmap_alloc(vmid_map, vmid_bits)"]
        A_BITMAP --> A_CHECK{"有空闲 VMID?"}
        A_CHECK -->|"是"| A_ASSIGN["分配 VMID"]
        A_CHECK -->|"否"| A_FAIL["返回 -ENOSPC"]
    end

    subgraph Free["VMID 释放"]
        F_REQ["销毁 Domain"] --> F_CALL["arm_smmu_domain_free()"]
        F_CALL --> F_FREE["arm_smmu_bitmap_free(vmid_map, vmid)"]
    end

    subgraph Reserved["保留"]
        R_INIT["SMMU 初始化"] --> R_SET["set_bit(0, vmid_map)<br/>VMID 0 用于 Bypass STE"]
    end
```

### 4.3 VMID 位宽

| 条件 | IDR0.VMID16 | VMID 位宽 | 可用 VMID 数量 |
|------|-------------|-----------|---------------|
| 基本配置 | 0 | 8-bit | 256 |
| 扩展配置 | 1 | 16-bit | 65536 |

**Linux 驱动特性检测**（`arm-smmu-v3.c:3513`）：
```c
smmu->vmid_bits = reg & IDR0_VMID16 ? 16 : 8;
```

### 4.4 VMID 与 TLB 无效化

VMID 是 TLB 无效化的关键索引参数：

```mermaid
graph TB
    subgraph TLBI_VMID["基于 VMID 的 TLB 无效化命令"]
        direction TB
        VMALL["CMDQ_OP_TLBI_S12_VMALL<br/>无效化 VMID 下<br/>所有 S1+S2 条目"]
        S2_IPA["CMDQ_OP_TLBI_S2_IPA<br/>按 VMID + IPA<br/>无效化 S2 条目"]
        EL2_ALL["CMDQ_OP_TLBI_EL2_ALL<br/>无效化所有<br/>EL2 上下文条目"]
    end
```

---

## 5. EL2 与 E2H 模式

### 5.1 HYP 特性

`IDR0.HYP` 标志表示 SMMU 支持 Hypervisor 模式（`arm-smmu-v3.h:35`）。当 CPU 运行在 EL2 并启用 E2H（EL2 Host Extension）时，SMMU 的行为会发生变化。

```mermaid
graph TB
    subgraph EL2_Support["HYP / E2H 特性"]
        direction TB
        subgraph Detection["特性检测"]
            D_IDR0["IDR0.HYP = 1<br/>SMMU 支持 Hypervisor"]
            D_CPU["CPU: ARM64_HAS_VIRT_HOST_EXTN<br/>CPU 支持 E2H"]
            D_AND["两者同时满足"]
            D_AND --> D_FEAT["设置 ARM_SMMU_FEAT_E2H"]
        end

        subgraph CR2_E2H["CR2 寄存器配置"]
            C_E2H["CR2.E2H = 1<br/>使能 EL2 Host 模式"]
        end

        subgraph Impact["影响"]
            I_STRW["STE.STRW = EL2<br/>(而非 NSEL1)"]
            I_TLBI["TLBI 使用 EL2 命令<br/>CMD_TLBI_EL2_*"]
            I_RESET["复位时执行<br/>CMD_TLBI_EL2_ALL"]
        end
    end
```

### 5.2 E2H 对 TLB 无效化的影响

当 `ARM_SMMU_FEAT_E2H` 置位时，Linux 驱动使用 EL2 特定的 TLBI 命令：

```mermaid
graph LR
    subgraph NoE2H["无 E2H (标准模式)"]
        NE_TLBI_ASID["CMDQ_OP_TLBI_NH_ASID"]
        NE_TLBI_VA["CMDQ_OP_TLBI_NH_VA"]
    end

    subgraph WithE2H["有 E2H (EL2 Host 模式)"]
        WE_TLBI_ASID["CMDQ_OP_TLBI_EL2_ASID"]
        WE_TLBI_VA["CMDQ_OP_TLBI_EL2_VA"]
    end
```

**Linux 驱动中的条件路由**（`arm-smmu-v3.c:944-954`）：

```c
// ASID 级别无效化
.opcode = smmu->features & ARM_SMMU_FEAT_E2H ?
    CMDQ_OP_TLBI_EL2_ASID : CMDQ_OP_TLBI_NH_ASID,

// VA 级别无效化
.opcode = smmu->features & ARM_SMMU_FEAT_E2H ?
    CMDQ_OP_TLBI_EL2_VA : CMDQ_OP_TLBI_NH_VA,
```

### 5.3 STRW（S2 Stage Reject Write）

STE 中的 STRW 字段控制 Stage 2 的属性交互（`arm-smmu-v3.h:236-238`）：

| STRW 值 | 含义 | 使用场景 |
|---------|------|----------|
| `NSEL1` (0) | Non-secure EL1 | 标准 Stage 1+S2 |
| `EL2` (2) | EL2 Host | E2H 模式 |

在 PCIe 模式下（MMU-600AE/700），无论 STRW 实际值如何，DTI 事务属性始终按 `EL1-S2` 计算。

---

## 6. Stage 2 地址翻译详解

### 6.1 Stage 2 页表格式

Stage 2 使用 ARM LPAE（Large Physical Address Extension）页表格式，由 `arm_64_lpae_s2_cfg` 结构描述：

```mermaid
graph TB
    subgraph VTCR_Params["VTCR 参数"]
        direction TB
        V_T0SZ["S2T0SZ (bits 5:0)<br/>IPA 地址空间大小"]
        V_SL["S2SL0 (bits 7:6)<br/>起始级别"]
        V_IRGN["S2IR0 (bits 9:8)<br/>Inner 缓存属性"]
        V_ORGN["S2OR0 (bits 11:10)<br/>Outer 缓存属性"]
        V_SH["S2SH0 (bits 13:12)<br/>Shareability"]
        V_TG["S2TG (bits 15:14)<br/>粒度 (4K/16K/64K)"]
        V_PS["S2PS (bits 18:16)<br/>PA 大小"]
    end
```

### 6.2 Stage 2 页表遍历

```mermaid
graph TB
    subgraph S2_Walk["Stage 2 页表遍历"]
        IPA_IN["输入: IPA"] --> S2_L0["L0 表<br/>(S2TTB 基址)"]
        S2_L0 -->|"L0 Descriptor"| S2_L1["L1 表"]
        S2_L1 -->|"L1 Descriptor"| S2_L2["L2 表"]
        S2_L2 -->|"L2 Descriptor"| S2_L3["L3 表"]
        S2_L3 -->|"L3 Descriptor"| PA_OUT["输出: PA + 属性"]
    end

    subgraph S2Attrs["Stage 2 输出属性"]
        SA_P["输出 PA"]
        SA_SH["Shareability"]
        SA_MemAttr["内存类型<br/>(Device/Normal)"]
        SA_AP["访问权限<br/>(可被 S1 覆盖)"]
        SA_NS["Non-Secure 标志"]
    end
```

### 6.3 Stage 1 + Stage 2 属性合并

当两个阶段同时启用时，最终的事务属性由两个阶段的输出属性合并决定：

```mermaid
flowchart LR
    subgraph S1_Attrs["Stage 1 输出属性"]
        S1_SH["Shareability"]
        S1_MemAttr["内存类型"]
        S1_AP["访问权限"]
    end

    subgraph S2_Attrs["Stage 2 输出属性"]
        S2_SH["Shareability"]
        S2_MemAttr["内存类型"]
        S2_AP["访问权限"]
        S2_NS["NS 标志"]
    end

    subgraph Final["最终属性"]
        F_SH["合并 Shareability"]
        F_MemAttr["S2 可覆盖 S1<br/>(取决于 FWB)"]
        F_AP["取更严格权限<br/>(S1 ∩ S2)"]
        F_NS["取 S2.NS"]
    end

    S1_SH & S2_SH --> F_SH
    S1_MemAttr & S2_MemAttr --> F_MemAttr
    S1_AP & S2_AP --> F_AP
    S2_NS --> F_NS
```

---

## 7. 虚拟化中的 TLB 管理

### 7.1 VM 切换时的 TLB 行为

```mermaid
sequenceDiagram
    participant KVM as KVM
    participant SMMU_DRV as SMMU 驱动
    participant SMMU as SMMU 硬件

    Note over KVM: 调度切换: VM_A → VM_B

    KVM->>SMMU_DRV: 通知 VM 切换
    SMMU_DRV->>SMMU: CMD_SYNC (保证之前命令完成)

    alt VMID 不同 (VM_A.vmid ≠ VM_B.vmid)
        Note over SMMU: TLB 自动按 VMID 区分<br/>无需全局刷新
        Note over SMMU: 新 VM 的 TLB Miss<br/>触发 Stage 2 页表遍历
    else VMID 相同 (VM 重用)
        SMMU_DRV->>SMMU: CMDQ_OP_TLBI_S12_VMALL
        Note over SMMU: 刷新该 VMID 下所有 TLB 条目
    end
```

### 7.2 Stage 2 TLB 无效化命令路由

Linux 驱动根据 Domain 阶段选择不同的 TLBI 命令（`arm-smmu-v3.c:1856-1944`）：

```mermaid
flowchart TD
    TLBI_REQ["需要 TLB 无效化"]

    TLBI_REQ --> CHECK{"Domain 阶段?"}

    CHECK -->|"S1"| S1_PATH["CMD_TLBI_EL2_ASID<br/>或 CMD_TLBI_EL2_VA<br/>(E2H 模式)"]
    CHECK -->|"S2 或 NESTED"| S2_PATH{"无效化范围?"}

    S2_PATH -->|"全部"| S2_ALL["CMDQ_OP_TLBI_S12_VMALL<br/>按 VMID 刷新全部"]
    S2_PATH -->|"指定 IPA"| S2_IPA["CMDQ_OP_TLBI_S2_IPA<br/>按 VMID + IPA 刷新"]

    S1_PATH --> SYNC["CMD_SYNC"]
    S2_ALL --> SYNC
    S2_IPA --> SYNC
    SYNC --> DONE["完成"]
```

---

## 8. 虚拟化故障处理

### 8.1 Stage 2 故障

当 Stage 2 翻译失败时，SMMU 生成事件并写入 Event Queue。在当前 Linux 驱动中，Stage 2 故障被视为致命错误（`arm-smmu-v3.c:1479-1481`）：

```c
/* Stage-2 is always pinned at the moment */
if (evt[1] & EVTQ_1_S2)
    return -EFAULT;
```

```mermaid
graph TB
    subgraph S2_Fault["Stage 2 故障处理"]
        DMA_REQ["设备 DMA 请求 (VA 或 IPA)"]
        S1_OK["Stage 1 成功 (VA→IPA)"]
        S2_FAIL["Stage 2 失败 (IPA→PA)"]
        EVT_Q["Event Queue<br/>F_TRANSLATION / F_PERM<br/>S2=1"]
        IRQ["MSI 中断"]
        DRV["SMMU 驱动"]
        KILL["返回 -EFAULT<br/>终止设备"]

        DMA_REQ --> S1_OK --> S2_FAIL --> EVT_Q --> IRQ --> DRV --> KILL
    end
```

### 8.2 故障事件中的 Stage 标识

Event Queue 条目中 `EVTQ_1_S2` 位（bit 39）标识故障发生在 Stage 2：

| S2 位 | 含义 |
|-------|------|
| 0 | Stage 1 故障 |
| 1 | **Stage 2 故障** |

---

## 9. Secure-EL2 虚拟化

### 9.1 Secure-EL2 支持

MMU-700（SMMUv3.2）引入了 Secure-EL2 支持（`SEL2=1`），允许安全世界也运行 Hypervisor：

```mermaid
graph TB
    subgraph SecureWorld["安全世界"]
        S_EL3["EL3 (Secure Monitor)"]
        S_EL2["Secure EL2<br/>(Secure Hypervisor)"]
        S_EL1["Secure EL1/0<br/>(Secure Guest OS)"]
        S_TBU["Secure TBU"]
        S_TCU["Secure TCU"]
    end

    subgraph NonSecureWorld["非安全世界"]
        NS_EL2["Non-Secure EL2<br/>(Hypervisor)"]
        NS_EL1["Non-Secure EL1/0<br/>(Guest OS)"]
        NS_TBU["Non-Secure TBU"]
        NS_TCU["Non-Secure TCU"]
    end

    S_EL3 <-->|"安全状态切换"| S_EL2
    S_EL2 -->|"两阶段翻译"| S_EL1
    S_EL1 --> S_TBU <--> S_TCU

    NS_EL2 -->|"两阶段翻译"| NS_EL1
    NS_EL1 --> NS_TBU <--> NS_TCU

    S_TCU -.->|"隔离"| NS_TCU
    S_TBU -.->|"隔离"| NS_TBU
```

### 9.2 VMID 分区

| VMID 空间 | 说明 |
|-----------|------|
| Secure VMID | 安全世界使用，独立于 Non-Secure |
| Non-Secure VMID | 非安全世界使用 |
| 两者可以相同值 | 但在 SMMU 内部是独立管理的 |

---

## 10. 虚拟化扩展特性

### 10.1 与虚拟化相关的 SMMU 特性

```mermaid
graph TB
    subgraph Features["虚拟化相关特性"]
        direction TB

        subgraph Core["核心特性"]
            S2P["Stage 2 翻译<br/>(IDR0.S2P)"]
            HYP["Hypervisor 模式<br/>(IDR0.HYP)"]
            VMID16["16-bit VMID<br/>(IDR0.VMID16)"]
            E2H["EL2 Host 扩展<br/>(CR2.E2H)"]
        end

        subgraph Enhanced["增强特性"]
            PTM["Paired TLB Maint.<br/>(CR2.PTM)"]
            RECINVSID["Record Inv. SID<br/>(CR2.RECINVSID)"]
            RIL["Range Inv. (BTM)<br/>(IDR3.RIL)"]
            VMW["VMID Wildcard<br/>(IDR0.VMW)"]
            XNX["EL0/EL1 执行控制<br/>(IDR3.XNX)"]
        end

        subgraph S3P2["SMMUv3.2 独有"]
            SEL2["Secure-EL2<br/>(S_IDR1.SEL2)"]
            HTTU["HW 页表更新<br/>(访问/脏位)"]
            BBML["Break-Before-Make<br/>Level 2"]
        end
    end
```

### 10.2 特性说明

| 特性 | 说明 | 虚拟化价值 |
|------|------|------------|
| **PTM** (Paired TLB Maintenance) | 与 CPU TLB 维护操作配对执行，确保 CPU 侧和 SMMU 侧的 TLB 同步 | 避免 CPU 和 SMMU 的 TLB 不一致 |
| **RECINVSID** (Record Invalidation SID) | 记录触发无效化的 StreamID，帮助调试 | 便于追踪 VM 的 TLB 维护来源 |
| **VMW** (VMID Wildcard) | VMID 通配符匹配，一条命令可同时无效化多个 VMID 的 TLB | 加速 VM 批量切换 |
| **RIL** (Range Invalidaton) | 范围无效化，一次命令覆盖连续地址范围 | 减少大块内存变更时的无效化命令数量 |
| **XNX** (Execute-never at S2) | Stage 2 级别的执行权限控制 | 防止 Guest 设备在 S2 阶段执行非授权内存 |

### 10.3 跨版本虚拟化特性对比

| 特性 | SMMUv2 | MMU-600AE (v3.1) | MMU-700 (v3.2) |
|------|--------|-------------------|-----------------|
| Stage 2 翻译 | Yes | Yes | Yes |
| 嵌套 S1+S2 | Yes | Yes | Yes |
| 8-bit VMID | Yes | Yes | Yes |
| 16-bit VMID | Yes (opt) | Yes | Yes |
| Hyp (EL2) | Yes | Yes | Yes |
| E2H | Yes | Yes | Yes |
| PTM | No | Yes | Yes |
| RECINVSID | No | Yes | Yes |
| VMID Wildcard | No | Yes | Yes |
| Range Inv. (RIL) | No | No | Yes |
| XNX | No | Yes | Yes |
| Secure-EL2 | No | No | Yes |
| HTTU | No | No | Yes |
| DVM 版本 | N/A | DVMv8.1 | DVMv8.4 |

---

## 11. Hypervisor 上下文与 E2HC

### 11.1 SMMUv2 中的 Hypervisor 上下文

在 SMMUv2 中，有两种特殊的上下文 Bank（`arm-smmu-v3.h` 中相关继承）：

```mermaid
graph TB
    subgraph SMMUv2_Banks["SMMUv2 上下文 Bank"]
        direction TB
        STD_CB["标准 Context Bank<br/>(SMMU_CBn)<br/>Guest Stage 1<br/>或 Host Stage 1"]
        HYPC["HYPC Bank<br/>(Hypervisor Context)<br/>匹配 Non-secure EL2<br/>地址翻译体制"]
        E2HC["E2HC Bank<br/>(E2H Context)<br/>使用 CD 中的 ASID<br/>和关联上下文的 VMID"]
    end
```

### 11.2 SMMUv3 中的等效机制

在 SMMUv3 中，上下文 Bank 被 Context Descriptor 替代，Hypervisor 上下文通过以下方式实现：

- **STRW 字段**：设为 `EL2` 表示使用 Hypervisor 上下文
- **E2H 模式**：CR2.E2H=1 使 SMMU 在 EL2 Host 模式下运行
- **独立的 ASID/VMID**：CD 中的 ASID 与 STE 中的 VMID 配合

---

## 12. 虚拟化场景下的设备直通完整流程

### 12.1 端到端流程

```mermaid
flowchart TD
    subgraph Setup["设置阶段"]
        U1["QEMU 启动 VM<br/>配置 -device vfio-pci"]
        U2["VFIO 打开设备<br/>获取设备 FD"]
        U3["VFIO 创建容器<br/>IOMMU Group"]
        U4["VFIO 设置 IOMMU<br/>vfio_iommu_type1_attach_group()"]
        U5["Host IOMMU 创建 S2 Domain<br/>arm_smmu_domain_alloc(S2)"]
        U6["分配 VMID<br/>arm_smmu_bitmap_alloc()"]
        U7["KVM 设置 Stage 2 页表<br/>GPA→HPA 映射"]
        U8["配置 SMMU STE<br/>Config=S2_TRANS<br/>VTTBR/VTCR/VMID"]
        U9["映射设备 IRQ<br/>通过 GIC 直通"]

        U1 --> U2 --> U3 --> U4 --> U5 --> U6 --> U7 --> U8 --> U9
    end

    subgraph Runtime["运行阶段"]
        R1["Guest 设备驱动初始化"]
        R2["设备发起 DMA (GPA)"]
        R3["SMMU Stage 2 翻译<br/>GPA→PA"]
        R4["设备访问物理内存"]

        R1 --> R2 --> R3 --> R4
    end

    subgraph Teardown["销毁阶段"]
        T1["VM 关闭"]
        T2["VFIO 释放设备"]
        T3["Host IOMMU 释放 Domain<br/>TLB 无效化<br/>VMID 释放"]

        T1 --> T2 --> T3
    end
```

### 12.2 GPA 映射变更处理

当 Guest 内存布局发生变化（如内存分配、释放、 ballooning）时：

```mermaid
sequenceDiagram
    participant GOS as Guest OS
    participant KVM as KVM
    participant VFIO as VFIO / IOMMU
    participant SMMU as SMMU

    GOS->>KVM: 内存操作<br/>(alloc/free/balloon)
    KVM->>KVM: 更新 Stage 2 页表

    KVM->>VFIO: 通知 GPA 映射变更
    VFIO->>SMMU: 更新 Stage 2 IO 页表

    alt 部分更新
        SMMU->>SMMU: CMDQ_OP_TLBI_S2_IPA<br/>(按 IPA 范围无效化)
    else 全量更新
        SMMU->>SMMU: CMDQ_OP_TLBI_S12_VMALL<br/>(按 VMID 全量无效化)
    end

    SMMU->>SMMU: CMDQ_OP_CMD_SYNC<br/>(等待完成)
    SMMU-->>VFIO: 完成
    VFIO-->>KVM: 完成
```

---

## 13. SMMUv3 复位与虚拟化

### 13.1 初始化时的 EL2 TLB 清理

SMMU 初始化时，如果支持 HYP 特性，会先清理 EL2 相关的 TLB 条目（`arm-smmu-v3.c:3336-3343`）：

```c
if (smmu->features & ARM_SMMU_FEAT_HYP) {
    cmd.opcode = CMDQ_OP_TLBI_EL2_ALL;
    arm_smmu_cmdq_issue_cmd_with_sync(smmu, &cmd);
}
cmd.opcode = CMDQ_OP_TLBI_NSNH_ALL;
arm_smmu_cmdq_issue_cmd_with_sync(smmu, &cmd);
```

```mermaid
flowchart TD
    INIT["SMMU 初始化"]
    INIT --> CHECK_HYP{"支持 HYP?"}
    CHECK_HYP -->|"是"| EL2_INV["CMDQ_OP_TLBI_EL2_ALL<br/>清理所有 EL2 TLB 条目"]
    CHECK_HYP -->|"否"| NS_INV
    EL2_INV --> NS_INV["CMDQ_OP_TLBI_NSNH_ALL<br/>清理所有 Non-Secure TLB 条目"]
    NS_INV --> DONE["TLB 干净, 可安全使用"]
```

### 13.2 CR2 寄存器初始化

SMMU 初始化时配置 CR2 寄存器（`arm-smmu-v3.c:3305-3311`）：

```mermaid
graph LR
    subgraph CR2_Init["CR2 初始化"]
        PTM_BIT["CR2.PTM = 1<br/>Paired TLB Maint"]
        RECINVSID_BIT["CR2.RECINVSID = 1<br/>记录无效化 SID"]
        E2H_BIT["CR2.E2H = 1<br/>EL2 Host 模式<br/>(仅当 FEAT_E2H)"]
    end
```

---

## 14. 总结

### 14.1 SMMU 虚拟化核心要点

```mermaid
mindmap
  root((SMMU 虚拟化))
    两阶段翻译
      Stage 1: Guest 管理 VA→IPA
      Stage 2: Hypervisor 管理 IPA→PA
      嵌套模式: S1+S2 同时启用
    VMID 管理
      8/16-bit VMID
      位图分配/释放
      VMID 0 保留给 Bypass
      TLB 按 VMID 自动区分
    EL2 / E2H
      Hypervisor 上下文模式
      STRW 字段控制
      EL2 TLBI 命令
      PTM 配对 TLB 维护
    设备直通
      S2 Domain + VFIO
      GPA→HPA 映射
      中断直通 (GIC)
    vIOMMU
      Nested Domain
      Guest 管理自己的 IOMMU
      CD/STE 由 Guest 配置
    安全扩展
      Secure-EL2 (SMMUv3.2)
      安全/非安全 VMID 分区
      HTTU / BBML / FWB
```

### 14.2 关键数据结构汇总

| 数据结构 | 文件位置 | 虚拟化用途 |
|----------|----------|------------|
| `arm_smmu_s2_cfg` | `arm-smmu-v3.h:601` | Stage 2 配置 (VMID, VTTBR, VTCR) |
| `arm_smmu_domain.stage` | `arm-smmu-v3.h:717` | Domain 阶段 (S2/Nested) |
| `arm_smmu_domain.s2_cfg` | `arm-smmu-v3.h:720` | Domain 中的 S2 配置 |
| `vmid_map` | `arm-smmu-v3.h:665` | VMID 位图分配器 |
| `STRW` 字段 | `arm-smmu-v3.h:236` | EL2 上下文选择 |
| `S2VMID` 字段 | `arm-smmu-v3.h:243` | STE 中的 VMID |
| `VTCR` 字段 | `arm-smmu-v3.h:244` | STE 中的 S2 控制寄存器 |
| `S2TTB` 字段 | `arm-smmu-v3.h:257` | STE 中的 S2 页表基址 |
