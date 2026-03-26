SMMU 架构知识点速记
==========================

本文档将 ARM SMMUv3 架构规范的核心知识点整理为问答格式，适合文本转语音播放，便于记忆复习。

---

**【问题】SMMU 是什么，它的核心作用是什么？**

【答案】SMMU 是系统内存管理单元，作用类似 CPU 的 MMU，但专门用于 DMA 请求的地址翻译。设备发起 DMA 时，SMMU 将设备的虚拟地址翻译为物理地址，再发送到系统互连。SMMU 仅对 DMA 方向生效，PE 到设备的方向由 PE 的 MMU 管理。翻译目的有两个：隔离和地址映射。记忆口诀：SMMU 是设备的 MMU，只管 DMA 进。

---

**【问题】SMMU 支持几级地址翻译？**

【答案】SMMU 支持两级翻译：Stage 1 将 VA 翻译为 IPA，Stage 2 将 IPA 翻译为 PA。每一级可以独立启用。Stage 1 通常由 OS 使用，用于设备 DMA 隔离；Stage 2 由 Hypervisor 使用，用于将设备 DMA 虚拟化到 Guest VM 地址空间。两级同时启用时称为嵌套翻译。记忆口诀：Stage 1 管 VA 到 IPA，Stage 2 管 IPA 到 PA，嵌套就是两个都开。

---

**【问题】SMMUv3 与 v1/v2 的核心区别是什么？**

【答案】v1/v2 使用寄存器配置上下文，数量有限，无法支持大规模并发。v3 改为基于内存的数据结构，支持大量 Stream 和翻译上下文。v1/v2 的上下文 bank 数量是硬限制，1000 个空闲网卡可能随时触发 DMA，必须全部预留上下文。v3 用内存表替代寄存器，消除了这个瓶颈。记忆口诀：v1/v2 寄存器受限，v3 内存表无限。

---

**【问题】StreamID 是什么，有什么作用？**

【答案】StreamID 是附加在每个 DMA 事务上的标识符，用于区分不同设备。大小为 0 到 32 位，具体由实现定义。StreamID 用于索引 Stream Table 查找对应的 STE。对于 PCIe 系统，Arm 建议 StreamID[15:0] 等于 RequesterID[15:0]，高 16 位区分不同 Root Complex。StreamID 命名空间是每个 SMMU 独立的。记忆口诀：StreamID 识别设备，16 位映射 RequesterID。

---

**【问题】SubstreamID（PASID）是什么？**

【答案】SubstreamID 用于区分同一 StreamID 下的不同子流，最多 20 位，等同于 PCIe PASID。典型场景是一个加速器设备有 8 个上下文，每个映射到不同用户进程。SubstreamID 索引 Context Descriptor 数组选择不同的 Stage 1 翻译上下文，但不影响 Stage 2。仅 Stage 1 实现时才需要支持 SubstreamID。记忆口诀：SubstreamID 等于 PASID，同一设备不同进程用不同翻译。

---

**【问题】Stream Table 的两种格式是什么？**

【答案】两种格式：线性表和两级表。线性表是 STE 的连续数组，由 StreamID 直接索引，所有实现都支持。两级表有一个一级描述符表指向多个二级线性 STE 表，可节省稀疏 StreamID 空间的内存。两级表的分割点 SPLIT 支持 6、8、10 位。超过 64 个 StreamID（6 位）的实现必须支持两级表。记忆口诀：线性直索引，两级省内存，SPLIT 分 6、8、10。

---

**【问题】STE（Stream Table Entry）包含哪些关键配置？**

【答案】STE 是 64 字节，包含：Config 字段控制启用哪些翻译阶段；S2TTB 是 Stage 2 翻译表基地址；S2VMID 是 VMID；S1ContextPtr 指向 CD（Stage 1 上下文描述符）；S1Fmt 和 S1CDMax 控制 CD 格式；STRW 决定 StreamWorld；EATS 控制 ATS 行为；S2R/S2S 控制 Stage 2 故障行为。记忆口诀：STE 管设备，Config 控阶段，S1Ptr 指上下文，S2TTB 指阶段二。

---

**【问题】CD（Context Descriptor）包含哪些关键配置？**

【答案】CD 是 64 字节，包含：TTB0 和 TTB1 是 Stage 1 翻译表基地址；ASID 标识地址空间；TG0/TG1 控制页粒度；T0SZ/T1SZ 控制地址范围；AA64 选择翻译表格式（VMSAv8-32 LPAE 或 VMSAv8-64）；A/R/S 位控制故障处理方式。CD 由 SubstreamID 索引选择，ASID 和 VMID 标记 TLB 条目用于区分地址空间。记忆口诀：CD 管阶段一，TTB 指翻译表，ASID 标空间。

---

**【问题】StreamWorld 有哪些类型？**

【答案】StreamWorld 等同于 PE 的异常级别，包括：NS-EL1（非安全 EL1/EL0）、NS-EL2（非安全 EL2）、NS-EL2-E2H（非安全 EL2 带 E2H）、S-EL2（安全 EL2）、Secure（安全）、EL3、Realm-EL1、Realm-EL2、Realm-EL2-E2H。StreamWorld 决定 TLB 条目如何响应广播 TLB 失效。any-EL2（NS-EL2、S-EL2、Realm-EL2）只用 TTB1 翻译表。记忆口诀：StreamWorld 等于 Exception Level，区分 NS/Secure/Realm。

---

**【问题】SMMU 地址尺寸有哪些概念？**

【答案】三个关键概念：输入地址固定 64 位；IAS 是中间地址大小（Stage 1 输出的 IPA），VMSAv8-32 固定 40 位，VMSAv8-64 等于 OAS；OAS 是输出物理地址大小，由 SMMU_IDR5.OAS 发现。VAS（虚拟地址大小）由 SMMU_IDR5.VAX 决定：0b00 为 49 位，0b01 为 52 位，0b10 为 56 位。地址超出范围会产生 F_ADDR_SIZE 故障。记忆口诀：输入 64 位，VAS 最大 56，OAS 由 IDR5 查。

---

**【问题】SMMU 环形队列的工作原理是什么？**

【答案】队列是 2 的 n 次方大小的环形 FIFO，每个有基地址和 PROD、CONS 两个索引。Command 队列中软件更新 PROD，SMMU 更新 CONS；Event 队列反过来。索引用 19 位加 1 位 wrap 标志。PROD 等于 CONS 且 wrap 相同表示空；PROD 等于 CONS 且 wrap 不同表示满。wrap 位区分满和空，允许所有条目同时有效。初始化时必须设置到一致状态。记忆口诀：环形队列靠 wrap 区分空满，软件写 PROD，硬件写 CONS。

---

**【问题】SMMU 有哪些队列类型？**

【答案】三种队列：Command 队列（输入），软件向 SMMU 发送命令，如 TLB 失效、配置失效等；Event 队列（输出），SMMU 报告故障和事件，每个安全状态一个；PRI 队列（输出，可选），接收 PCIe Page Request Interface 的缺页请求。每个安全状态（NS、Secure、Realm）各有独立的 Command 和 Event 队列。记忆口诀：Command 发命令，Event 报故障，PRI 收缺页请求。

---

**【问题】ATS（Address Translation Service）是什么？**

【答案】ATS 是 PCIe 地址翻译服务，让端点设备在本地 ATC（翻译缓存）中缓存翻译结果，减少 SMMU 访问延迟。设备发起 Translation Request，SMMU 返回物理地址和权限。授权后设备直接发送 Translated 事务绕过 SMMU。翻译变更时必须用 CMD_ATC_INV 命令失效 ATC。ATS 不支持安全 Stream。PRI 依赖 ATS 但 ATS 不需要 PRI。记忆口诀：ATS 缓翻译到设备，省延迟但要管失效。

---

**【问题】PRI（Page Request Interface）是什么？**

【答案】PRI 是 PCIe 缺页请求接口，依赖 ATS，允许设备对未驻留的动态分页内存发起 DMA。ATS 请求失败时设备发 PRI Page Request，软件在 PRI 队列收到后调页，然后发 CMD_PRI_RESP 响应。成功则设备重试 ATS，失败则发负响应。PRI 队列大小有限，由 credits 控制。队列满时 SMMU 返回成功响应，设备会重试。记忆口诀：PRI 让设备能缺页 DMA，ATS 失败发请求，软件调页后响应。

---

**【问题】Terminate 故障模型和 Stall 故障模型的区别？**

【答案】Terminate 模型：故障事务立即终止，可 abort（向设备报告错误）或 RAZ/WI（读返回 0，写忽略）。Stage 1 由 CD.A 位控制，Stage 2 只能 abort。Stall 模型：故障事务暂停，等软件用 CMD_RESUME 恢复，适合非 PCIe 设备的缺页 DMA。Stage 1 由 CD.S 位控制，可被 STE.S1STALLD 禁止；Stage 2 由 STE.S2S 位控制。记录故障由 CD.R 和 STE.S2R 控制。记忆口诀：Terminate 直接报错，Stall 挂起等救援。

---

**【问题】HTTU（硬件翻译表更新）是什么？**

【答案】HTTU 是 SMMU 硬件自动更新翻译表中的 Access Flag 和 Dirty 状态。分三种支持级别：不支持、仅 Access Flag、Access Flag 加 Dirty。HTTU 对 ATS 很重要，因为 Translated 事务绕过 SMMU，ATS 请求是唯一更新标志的机会。读请求只更新 Access Flag，读写请求更新两个。有 BBML 支持时可无需 break-before-make 修改映射大小。记忆口诀：HTTU 自动更新 AF 和 Dirty，ATS 场景必须开启。

---

**【问题】SMMU 初始化的标准步骤是什么？**

【答案】推荐五步：一、分配并初始化 Stream Table 及基地址指针；二、分配并初始化 Command 和 Event 队列（基地址、索引）；三、通过 SMMU_CR0.CMDQEN 启用命令处理，启用 EVENTQEN；四、发送命令失效所有缓存配置和 TLB 条目；五、设置 SMMU_CR0.SMMUEN 启用翻译。安全实现可用 SMMU_S_INIT.INV_ALL 直接失效缓存。复位时 SMMU 处于禁用状态，流量旁通。记忆口诀：分配表和队列，启命令，清缓存，最后开翻译。

---

**【问题】SMMU_CR0 控制寄存器有哪些关键位？**

【答案】SMMU_CR0 是主控制寄存器，关键位包括：SMMUEN 启用翻译；CMDQEN 启用 Command 队列处理；EVENTQEN 启用 Event 队列；ATSCHK 控制 Translated 事务是否需要额外检查；PRIQEN 启用 PRI 队列处理；S1E 只启用 Stage 1；S2E 只启用 Stage 2。修改 SMMUEN 需要等 CR0ACK 确认。SMMU_S_CR0 是安全状态的对应寄存器。记忆口诀：CR0 管总开关，EN 开队列，ATSCHK 查 ATS 流量。

---

**【问题】SMMU_IDR0 识别寄存器提供什么信息？**

【答案】IDR0 描述 SMMU 实现的功能特性：S1P/S2P 指示支持哪些翻译阶段；ATS 指示支持 PCIe ATS；PRI 指示支持 PRI；BTM 指示支持广播 TLB 维护；HTTU 指示 HTTU 支持级别；ASID16/VMID16 指示 16 位 ASID/VMID 支持；STALL_MODEL 指示故障模型；TTF 指示翻译表格式；RME_IMPL 指示 RME 支持。记忆口诀：IDR0 是功能清单，查 S1P/S2P/ATS/PRI/BTM。

---

**【问题】配置失效命令有哪些？**

【答案】配置失效命令操作 Stream Table 和 CD 缓存：CMD_CFGI_STE 失效单个 STE；CMD_CFGI_STE_RANGE 范围失效多个 STE；CMD_CFGI_CD 失效单个 CD；CMD_CFGI_CD_ALL 失效某个 StreamID 的所有 CD；CMD_CFGI_VMS_PIDM 按 VMID 失效；CMD_CFGI_ALL 失效所有配置。配置失效与 TLB 失效是独立的维护操作。失效后需 CMD_SYNC 确保完成。记忆口诀：CFGI 失效配置缓存，STE 范围，CD 全部，VMID 按虚拟机。

---

**【问题】TLB 失效命令有哪些？**

【答案】TLB 失效分三类：按地址失效（CMD_TLBI_VA、CMD_TLBI_VAA），按 ASID 失效（CMD_TLBI_ASID），全局失效（CMD_TLBI_NH_ALL、CMD_TLBI_NSNH_ALL）。SMMUv3.2 新增范围失效（CMD_TLBI_RANGE）和 Level Hint。每个命令包含 StreamWorld、VMID、ASID、TLBI_LEAF 等参数。广播 TLB 维护来自 PE 的 TLBI 指令，SMMU 根据 StreamWorld 匹配并失效 TLB 条目。记忆口诀：TLBI 按地址、ASID 或全局失效，v3.2 加范围失效。

---

**【问题】CMD_SYNC 命令的作用是什么？**

【答案】CMD_SYNC 是同步命令，确保之前的所有命令（包括配置失效、TLB 失效、ATS 失效等）全部完成。软件发出 CMD_SYNC 后轮询或等待 MSI 中断确认完成。这是保证操作顺序的关键机制。CMD_SYNC 可选发送 MSI 中断通知软件完成。ATS 失效超时时，CMD_SYNC 会产生 CERROR_ATC_INV_SYNC 错误。记忆口诀：CMD_SYNC 等所有命令完成，是同步屏障。

---

**【问题】SMMU 安全状态支持是怎样的？**

【答案】SMMU 支持非安全、安全和 Realm 三种安全状态。SMMU_S_* 寄存器控制安全状态。SEC_SID 标志区分安全和非安全 StreamID 命名空间。每个安全状态有独立的 Stream Table、Command 和 Event 队列。安全软件初始化后移交给非安全软件前必须失效所有安全缓存。NSSTALLD 位可禁止非安全软件使用 Stall 模式。Realm 状态用于 RME，有独立的编程接口。记忆口诀：三态 NS/Secure/Realm，各自有独立队列和表。

---

**【问题】SMMUv3.2 引入了哪些重要特性？**

【答案】SMMUv3.2 主要特性：支持 Armv8.4-A，包括 Secure EL2 和 Secure Stage 2 翻译；引入 VMS（Virtual Machine Structure）描述每 VM 配置；BBML 支持无需 break-before-make 修改映射大小；RIL 支持范围 TLB 失效和 Level Hint；STT 支持小翻译表；FWB 支持 Stage 2 强制 Write-Back；MPAM 支持内存资源分区监控。记忆口诀：v3.2 加 Secure EL2、VMS、BBML、范围失效。

---

**【问题】SMMUv3.3 和 v3.4 引入了哪些重要特性？**

【答案】v3.3：E0PD 防止 EL0 访问地址空间一半区域（强制）；PTWNNC 设备内存表访问按 Normal NC 处理（强制）；ECMDQ 增强命令队列接口减少争用（可选）；ATSRECERR 记录 ATS 配置错误事件（可选）。v3.4：LPA2 支持 52 位 VA/PA（4KB/16KB 粒度）；PAN3 增强 PAN；THE 翻译加固；S1PI/S2PIE 权限间接；S2POE 阶段二权限覆盖；D128 支持 128 位描述符和 56 位地址。记忆口诀：v3.3 强制 E0PD 和 PTWNNC，v3.4 大地址和权限间接。

---

**【问题】RME（Realm Management Extension）对 SMMU 有什么影响？**

【答案】RME 引入 Granule Protection Check（GPC），SMMU 需要 ROOT_IMPL == 1。GPC 检查物理地址属于哪个安全域。RME DA 支持 Realm 状态设备，有独立编程接口。RME DA 要求 VMSAv8-64 only（TTF=0b10），必须支持 COHACC。DPT（Device Permission Table）提供额外权限检查层。RME SMMU 不支持 EL3 StreamWorld。记忆口诀：RME 加 GPC 检查域归属，RME DA 接入 Realm 设备。

---

**【问题】DPT（Device Permission Table）是什么？**

【答案】DPT 是设备权限表，在 RME DA 场景提供额外权限检查。当 ATS Translated 事务绕过 SMMU 翻译时，DPT 检查输出物理地址的读写执行权限和目标安全域。DPT 结构类似翻译表，可被缓存。检查失败产生 F_TRANSL_FORBIDDEN。ATS 请求和 Translated 事务都会触发 DPT 检查。DPT 由 Realm 安全状态管理。记忆口诀：DPT 在 ATS 绕过时查权限，补翻译缺失的安全检查。

---

**【问题】EATS 字段的取值和含义是什么？**

【答案】EATS（Enable ATS Translated）有四种编码：0b00 禁用 ATS；0b01 Full ATS，翻译返回 PA，Translated 事务绕过 SMMU；0b10 Split-stage ATS，翻译返回 IPA，Translated 事务经过 Stage 2 翻译，保持阶段二隔离（需要 ATSCHK=1）；0b11 Use DPT，使用设备权限表检查。Full ATS 绕过全部翻译，Split-stage ATS 保留阶段二隔离。EATS 变更需要先禁用 ATS 再切换。记忆口诀：0b00 禁，0b01 全绕过，0b10 保留阶段二，0b11 用 DPT。

---

**【问题】SMMU 事务翻译的完整流程是什么？**

【答案】完整流程五步：一、SMMU 全局禁用时直接旁通（属性由 GBPA 决定）；二、全局启用时定位配置：按 StreamID 查 STE，Stage 2 用 STE 中基地址，Stage 1 通过 STE 查 CD（CD 可能经 Stage 2 翻译获取）；三、执行翻译：Stage 1 用 CD 翻译 VA 到 IPA，Stage 2 用 STE 翻译 IPA 到 PA；四、有效配置无故障则转发带输出地址的事务；五、配置无效或翻译故障则终止或暂停事务并记录事件。记忆口诀：查 STE，查 CD，翻阶段一，翻阶段二，成功转发失败报。

---

**【问题】ASID 和 VMID 在 SMMU 中的作用是什么？**

【答案】ASID（16 位最大）标识 Stage 1 地址空间，区分不同进程或 VM 的翻译。VMID（16 位最大）标识 Stage 2 地址空间，区分不同 VM 的 IPA 到 PA 翻译。ASID 存储在 CD 中，VMID 存储在 STE 中。TLB 条目用 {StreamWorld, VMID, ASID, Address} 唯一标识。ASID/VMID 也用于匹配广播 TLB 失效操作。多个 Stream 可共享相同 ASID/VMID 配置以共享 TLB 条目。记忆口诀：ASID 标阶段一空间，VMID 标阶段二空间，一起唯一标识 TLB 条目。

---

**【问题】STE.Config 字段的取值和含义是什么？**

【答案】Config 是 3 位字段：0b000 禁用流（事务终止）；0b100 旁通所有翻译（输入地址直接输出）；0b001 仅 Stage 2；0b010 仅 Stage 1；0b011 Stage 1 + Stage 2（嵌套翻译）。旁通时若地址超过 OAS 则产生 F_ADDR_SIZE。禁用时事务立即终止并记录事件。Config=0b100 时阶段一地址大小故障仍可能发生。记忆口诀：000 禁，100 旁通，001 阶段二，010 阶段一，011 嵌套。
