![image 1](corelink_mmu600ae_trm_101412_0100_07_en_images/imageFile1.png)

### Arm® CoreLink™ MMU-600AE System Memory Management Unit

Revision r1p0

#### Technical Reference Manual

###### Non-Conﬁdential

Copyright © 2018–2020, 2022–2024 Arm Limited (or its aﬃliates). All rights reserved.

###### Issue 07

101412_0100_07_en

| | | | | | | | |
|---|---|---|---|---|---|---|---|
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |


###### Arm® CoreLink™ MMU-600AE System Memory Management Unit Technical Reference Manual

|This document is Non-Conﬁdential.<br><br>Copyright © 2018–2020, 2022–2024 Arm Limited (or its aﬃliates). All rights reserved.<br><br>This document is protected by copyright and other intellectual property rights. Arm only permits use of this document if you have reviewed and accepted Arm's Proprietary Notice found at the end of this document.<br><br>This document (101412_0100_07_en) was issued on 2024-08-16. There might be a later issue at http://developer.arm.com/documentation/101412<br><br>The product revision is r1p0.<br><br>See also: Proprietary Notice | Product and document information | Useful resources|
|---|


###### Start Reading

If you prefer, you can skip to the start of the content.

###### Intended audience

This book is written for system designers, system integrators, and programmers who are designing or programming a System on Chip (SoC) that uses the MMU-600AE.

###### Inclusive language commitment

Arm values inclusive communities. Arm recognizes that we and our industry have used language that can be oﬀensive. Arm strives to lead the industry and create change.

We believe that this document contains no oﬀensive language. To report oﬀensive language in this document, email terms@arm.com.

###### Feedback

Arm welcomes feedback on this product and its documentation. To provide feedback on the product, create a ticket on https://support.developer.arm.com.

To provide feedback on the document, ﬁll the following survey: https://developer.arm.com/ documentation-feedback-survey.

# Contents

###### 1. About the MMU-600AE............................................................................................................................... 9

- 1.1 Compliance.....................................................................................................................................................10
- 1.2 Features...........................................................................................................................................................11
- 1.3 Interfaces........................................................................................................................................................12
- 1.4 Conﬁgurable options....................................................................................................................................13
- 1.5 Documentation..............................................................................................................................................14
- 1.6 Design ﬂow....................................................................................................................................................14
- 1.7 Functional Safety (FuSa)..............................................................................................................................15
- 1.8 Product revisions.......................................................................................................................................... 16


###### 2. Functional description.................................................................................................................................18

- 2.1 Translation Buﬀer Unit................................................................................................................................19

- 2.1.1 TBU direct indexing and MTLB partitioning.......................................................................................21
- 2.1.2 AXI USER bits deﬁned by the MMU-600AE TBU............................................................................23


- 2.2 Translation Control Unit..............................................................................................................................24

- 2.2.1 Distributed Virtual Memory messages.................................................................................................26
- 2.2.2 TCU transaction handling........................................................................................................................27
- 2.2.3 TCU prefetch..............................................................................................................................................28


- 2.3 DTI interconnect...........................................................................................................................................30
- 2.4 Interfaces........................................................................................................................................................31

- 2.4.1 TCU interfaces...........................................................................................................................................31
- 2.4.2 TBU interfaces...........................................................................................................................................35
- 2.4.3 DTI interconnect interfaces....................................................................................................................38


- 2.5 DTI protocol overview.................................................................................................................................41
- 2.6 Performance Monitoring Unit....................................................................................................................42

- 2.6.1 SMMUv3 architectural performance events...................................................................................... 43
- 2.6.2 MMU-600AE TCU events......................................................................................................................44
- 2.6.3 MMU-600AE TBU events......................................................................................................................46
- 2.6.4 SMMUv3 PMU register architectural options....................................................................................47
- 2.6.5 PMU snapshot interface..........................................................................................................................48


- 2.7 Reliability, Availability, and Serviceability................................................................................................48
- 2.8 Quality of Service.........................................................................................................................................49


- 2.9 Error responses.............................................................................................................................................49
- 2.10 Conversion between ACE-Lite and Armv8 attributes.......................................................................49

- 2.10.1 Attribute handling...................................................................................................................................50

2.11 Constraints and limitations of use.........................................................................................................52

- 2.11.1 SMMUv3 support...................................................................................................................................53




- 2.11.2 AMBA support........................................................................................................................................ 56
- 3. Programmer's model....................................................................................................................................60


- 3.1 SMMU architectural registers....................................................................................................................61

- 3.2 MMU-600AE memory map.......................................................................................................................65
- 3.3 TCU and TBU PMU identiﬁcation registers...........................................................................................67

- 3.3.1 TCU PMU component and peripheral ID registers...........................................................................67
- 3.3.2 TBU PMU component and peripheral ID registers...........................................................................68


- 3.4 TCU identiﬁcation registers........................................................................................................................69

3.4.1 TCU component and peripheral ID registers..................................................................................... 70

- 3.5 TCU microarchitectural registers.............................................................................................................. 71

3.5.1 TCU_CTRL, TCU Control register......................................................................................................... 71

- 3.5.2 TCU_QOS, TCU Quality of Service register.......................................................................................73
- 3.5.3 TCU_CFG, TCU Conﬁguration Information register......................................................................... 75
- 3.5.4 TCU_STATUS, TCU Status Information register................................................................................76
- 3.5.5 TCU_SCR, TCU Secure Control register..............................................................................................77
- 3.5.6 TCU_NODE_CTRLn, TCU Node Control registers............................................................................78
- 3.5.7 TCU_NODE_STATUSn, TCU Node Status registers.........................................................................80


- 3.6 TCU RAS registers........................................................................................................................................81

3.6.1 TCU_ERRFR, TCU Error Feature register............................................................................................81

- 3.6.2 TCU_ERRCTLR, TCU Error Control register.......................................................................................82
- 3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register................................................ 83
- 3.6.4 TCU_ERRGEN, TCU Error Generation register..................................................................................85


- 3.7 TBU identiﬁcation registers........................................................................................................................87

3.7.1 TBU component and peripheral ID registers..................................................................................... 87

- 3.8 TBU microarchitectural registers...............................................................................................................88

3.8.1 TBU_CTRL, TBU Control register......................................................................................................... 88

- 3.8.2 TBU_SCR, TBU Secure Control register..............................................................................................89

3.9 TBU RAS registers........................................................................................................................................91

- 3.9.1 TBU_ERRFR, TBU Error Feature register............................................................................................91




- 3.9.2 TBU_ERRCTLR, TBU Error Control register.......................................................................................92


- 3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register................................................ 93
- 3.9.4 TBU_ERRGEN, TBU Error Generation register..................................................................................94


###### 4. Functional Safety..........................................................................................................................................97

- 4.1 FuSa I/Os.....................................................................................................................................................100

- 4.1.1 Non-architected FuSa ports.................................................................................................................100
- 4.1.2 Q-Channel FuSa ports...........................................................................................................................102
- 4.1.3 AMBA interface FuSa ports.................................................................................................................102


- 4.2 Clocks and resets.......................................................................................................................................102
- 4.3 DFT protection...........................................................................................................................................104


- 4.3.1 MBIST........................................................................................................................................................104
- 4.3.2 ATPG Scan...............................................................................................................................................104
- 4.3.3 LBIST..........................................................................................................................................................105
- 4.4 Fault Management Unit............................................................................................................................105

- 4.4.1 Error signaling to the FMU..................................................................................................................106
- 4.4.2 Error signaling from the FMU.............................................................................................................107
- 4.4.3 Error Record table..................................................................................................................................108
- 4.4.4 Safety Mechanism table........................................................................................................................109
- 4.4.5 Software interaction...............................................................................................................................113
- 4.4.6 Lock and key mechanism......................................................................................................................116
- 4.4.7 Ping mechanism......................................................................................................................................117
- 4.4.8 Correctable Error enable...................................................................................................................... 119
- 4.4.9 Fault Detection and Control Programmer's View........................................................................... 120


- 4.5 Lock-step protection..................................................................................................................................140

4.5.1 Comparators.............................................................................................................................................141 4.5.2 Non-resettable ﬂops..............................................................................................................................142 4.5.3 Reset..........................................................................................................................................................142 4.5.4 Error injection..........................................................................................................................................142

- 4.6 RAM protection..........................................................................................................................................143

4.6.1 RAM fault correction.............................................................................................................................144

- 4.6.2 RAM fault reporting...............................................................................................................................144
- 4.6.3 RAM fault control...................................................................................................................................145
- 4.6.4 RAM fault severity................................................................................................................................. 145
- 4.6.5 Address protection.................................................................................................................................145


- 4.7 External interface protection...................................................................................................................146


###### 4.7.1 ACE-Lite interface parity protection..................................................................................................148

- 4.7.2 AXI4-Stream interface parity protection...........................................................................................148
- 4.7.3 APB interface parity protection.......................................................................................................... 149
- 4.7.4 F-Channel................................................................................................................................................. 149
- 4.7.5 Interrupt output protection..................................................................................................................152
- 4.7.6 Tie-oﬀ input protection.........................................................................................................................152
- 4.7.7 Connecting to unsafe interfaces.........................................................................................................152
- 4.8 Integrating the TCU, TBU, LPD, PCIe ATS, and DTI AXI4-Stream interconnect..........................153
- 4.9 Q-Channel protection...............................................................................................................................154

- 4.9.1 Q-Channel signaling...............................................................................................................................156
- 4.9.2 Q-Channel acceptance..........................................................................................................................157
- 4.9.3 Q-Channel denial....................................................................................................................................157
- 4.9.4 _chk signal timing................................................................................................................................... 157
- 4.9.5 Transient faults........................................................................................................................................158
- 4.9.6 Stuck-at faults..........................................................................................................................................160
- 4.9.7 Disabling Q-Channel Safety Mechanisms.........................................................................................161


- 4.10 Systematic fault watchdog protection................................................................................................161


###### A. Signal descriptions.....................................................................................................................................162

- A.1 Clock and reset signals for MMU-600AE............................................................................................162
- A.2 TCU QTW/DVM interface signals for MMU-600AE........................................................................162
- A.3 TCU programming interface signals for MMU-600AE..................................................................... 164
- A.4 TCU SYSCO interface signals for MMU-600AE................................................................................165
- A.5 TCU PMU snapshot interface signals for MMU-600AE..................................................................165
- A.6 TCU LPI_PD interface signals for MMU-600AE................................................................................166
- A.7 TCU LPI_CG interface signals for MMU-600AE................................................................................166
- A.8 TCU DTI interface signals for MMU-600AE.......................................................................................166
- A.9 TCU APB FMU interface signals for MMU-600AE...........................................................................167
- A.10 TCU interrupt signals for MMU-600AE............................................................................................ 168
- A.11 TCU event interface signal for MMU-600AE.................................................................................. 169
- A.12 TCU tie-oﬀ signals for MMU-600AE.................................................................................................170
- A.13 TCU and TBU test and debug signals for MMU-600AE...............................................................171
- A.14 TBU TBS interface signals for MMU-600AE................................................................................... 171
- A.15 TBU TBM interface signals for MMU-600AE..................................................................................173
- A.16 TBU PMU snapshot interface signals for MMU-600AE................................................................175
- A.17 TBU LPI_PD interface signals for MMU-600AE..............................................................................175
- A.18 TBU LPI_CG interface signals for MMU-600AE............................................................................. 176


- A.19 TBU DTI interface signals for MMU-600AE....................................................................................176
- A.20 TBU interrupt signals for MMU-600AE............................................................................................ 177
- A.21 TBU tie-oﬀ signals for MMU-600AE.................................................................................................177
- A.22 DTI interconnect switch DN_Sn interface signals...........................................................................179
- A.23 DTI interconnect switch UP_Sn interface signals............................................................................179
- A.24 DTI interconnect switch DN_M interface signals............................................................................180
- A.25 DTI interconnect switch UP_M interface signals.............................................................................180
- A.26 DTI interconnect sizer LPI_CG interface signals..............................................................................181
- A.27 DTI interconnect sizer DN_S interface signals.................................................................................181
- A.28 DTI interconnect sizer UP_S interface signals..................................................................................181
- A.29 DTI interconnect sizer DN_M interface signals............................................................................... 182
- A.30 DTI interconnect sizer UP_M interface signals................................................................................182
- A.31 DTI interconnect register slice LPI_CG interface signals............................................................... 183
- A.32 DTI interconnect register slice DN_S interface signals.................................................................. 183
- A.33 DTI interconnect register slice UP_S interface signals................................................................... 183
- A.34 DTI interconnect register slice DN_M interface signals.................................................................184
- A.35 DTI interconnect register slice UP_M interface signals..................................................................184


###### B. Software initialization examples............................................................................................................ 185

- B.1 Initializing the SMMU...............................................................................................................................185

- B.1.1 Allocating the Command queue.........................................................................................................185
- B.1.2 Allocating the Event queue................................................................................................................. 186
- B.1.3 Conﬁguring the Stream table.............................................................................................................. 186
- B.1.4 Initializing the Command queue.........................................................................................................187
- B.1.5 Initializing the Event queue................................................................................................................. 187
- B.1.6 Invalidating TLBs and conﬁguration caches.....................................................................................187
- B.1.7 Creating a basic Context Descriptor................................................................................................. 188
- B.1.8 Creating a Stream Table Entry............................................................................................................ 189


- B.2 Enabling the SMMU..................................................................................................................................190


###### Proprietary Notice.........................................................................................................................................191

Product and document information......................................................................................................... 193 Product status...................................................................................................................................................193 Revision history................................................................................................................................................ 193 Conventions.......................................................................................................................................................197

###### Useful resources............................................................................................................................................ 199

## 1. About the MMU-600AE

MMU-600AE is a Functional Safety (FuSa) variant of the MMU-600 System-level Memory Management Unit (SMMU) that translates an input address to an output address. This translation is based on address mapping and memory attribute information that is available in MMU-600AE internal registers and translation tables.

MMU-600AE implements the Arm® SMMU architecture version 3.1, SMMUv3.1, as the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1 deﬁnes.

An address translation from an input address to an output address is described as a stage of address translation. MMU-600AE can perform:

- • Stage 1 translations that translate an input virtual address (VA) to an output physical address (PA) or intermediate physical address (IPA).
- • Stage 2 translations that translate an input IPA to an output PA.
- • Combined stage 1 and stage 2 translations that translate an input VA to an IPA, and then translate that IPA to an output PA. MMU-600AE performs translation table walks for each stage of the translation.


In addition to translating an input address to an output address, a stage of address translation also deﬁnes the memory attributes of the output address. With a two-stage translation, the stage

###### 2 translation can modify the attributes that the stage 1 translation deﬁnes. A stage of addresstranslation can be disabled or bypassed, and MMU-600AE can deﬁne memory attributes fordisabled and bypassed stages of translation.

MMU-600AE uses inputs from the requester to identify a context. Conﬁguration tables in memory deﬁne how MMU-600AE is to translate each context, such as which translation tables to use.

MMU-600AE can cache the result of a translation table lookup in a Translation Lookaside Buﬀer (TLB). It can also cache conﬁguration tables in a conﬁguration cache.

MMU-600AE contains the following key components:

- • Translation Buﬀer Units (TBUs) that use a TLB to cache translation tables.
- • A Translation Control Unit (TCU) that controls and manages address translations.
- • Distributed Translation Interface (DTI) interconnect components that connect multiple TBUs to the TCU.


##### 1.1 Compliance

Compliance indicates how an Arm® product, in this case MMU-600AE, adheres to architecture speciﬁcations, protocol speciﬁcations, and relevant external standards.

This Technical Reference Manual (TRM) complements architecture reference manuals, architecture speciﬁcations, protocol speciﬁcations, and relevant external standards. It does not duplicate information from these sources.

MMU-600AE complies with, or implements, the following speciﬁcations: Arm architecture

MMU-600AE implements parts of the Armv8 Virtual Memory System Architecture (VMSA), as the Arm® Architecture Reference Manual Armv8, for A-proﬁle architecture deﬁnes. The SMMUv3 architecture describes the parts of VMSA that apply to the MMU-600AE.

SMMU architecture MMU-600AE implements the SMMUv3.1 architecture, as the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1 deﬁnes. For more information, see 2.11.1 SMMUv3 support on page 53.

AMBA Distributed Translation Interface protocol MMU-600AE implements the Distributed Translation Interface (DTI) protocol, as the AMBA® DTI Protocol Speciﬁcation deﬁnes.

The DTI interfaces use an AXI4‑Stream interface, as the AMBA® 4 AXI4‑Stream Protocol Speciﬁcation deﬁnes.

For more information, see 2.5 DTI protocol overview on page 41.

AMBA ACE5-Lite and AMBA AXI5 protocol MMU-600AE complies with the AMBA® ACE5‑Lite protocol. For more information about the protocol, see the AMBA® AXI and ACE Protocol Speciﬁcation. For more information about how the MMU-600AE uses the protocol, see 2.11.2 AMBA support on page 55.

AMBA APB protocol MMU-600AE complies with the AMBA APB4 protocol, as the AMBA® APB Protocol Speciﬁcation deﬁnes.

AMBA Interface Parity MMU-600AE complies with the AMBA Interface Parity Speciﬁcation, as the AMBA® APB Protocol Speciﬁcation deﬁnes.

##### 1.2 Features

MMU-600AE features include architecture compliance, AMBA® interfaces, ﬂexible integration, and high performance translation.

- • Compliance with the SMMUv3.1 architecture:

- ◦ Support for stage 1 translation, stage 2 translation, and stage 1 followed by stage 2 translation
- ◦ Support for Armv8 AArch32 and AArch64 translation table formats
- ◦ Support for 4KB, 16KB, and 64KB granule sizes in AArch64 format
- ◦ Support for PCI Express (PCIe) integration, including Address Translation Services (ATS) and Process Address Space IDs (PASIDs)
- ◦ Support for Page Request Interface (PRI), as SMMUv3 deﬁnes. PRI is an optional PCIe ATS extension that enables support for unpinned memory in PCIe.
- ◦ Support for ACE5‑Lite atomic transactions in the TBU
- ◦ A requester can be stalled while a processor handles translation faults, enabling software support for demand paging
- ◦ Conﬁguration tables in memory can support millions of active translation contexts
- ◦ Queues in memory perform MMU-600AE management. There is no requirement to stall a processor when it accesses the MMU-600AE.
- ◦ Support for Generic Interrupt Controller (GIC) integration, with Message Signaled Interrupts (MSIs) supported for common interrupt types
- ◦ A Performance Monitoring Unit (PMU) in each TBU and TCU that enables MMU-600AE performance to be investigated
- ◦ Reliability, Serviceability, and Availability (RAS) features for cache corruption detection and correction


- • Support for AMBA® interfaces, including:


- ◦ ACE5‑Lite TBU transaction interfaces that support cache stash transactions, deallocating transactions, and cache maintenance
- ◦ Option to disable cache maintenance operations on a TBU, a sideband channel protection feature
- ◦ An architected AXI5 extension that communicates per‑transaction translation stream information
- ◦ An ACE5‑Lite+Distributed Virtual Memory (DVM) TCU table walk interface that enables Armv8.2 processors to perform shared TLB invalidate operations without accessing the MMU-600AE directly
- ◦ An ACE5 low-power extension that enables the TCU to subscribe to DVM TLB invalidate requests when powering up and powering down without reprogramming the DTI interconnect
- ◦ AMBA® DTI communication between the TCU and TBUs, enabling a requester to request translations and implement TBU functionality internally


- ◦ Support for the AMBA® Low‑Power Interface (LPI) Q‑Channel so that standard controllers can control power and clock gating
- ◦ AXI5 WAKEUP signaling on all interfaces, including DTI and APB interfaces


- • Support for ﬂexible integration:

- ◦ You can place a conﬁgurable number of TBUs close to each requester being translated
- ◦ Communication between TBU and TCU over AXI4‑Stream is supported using the supplied DTI interconnect components, or any other AXI4‑Stream interconnect
- ◦ DTI interconnect components support hierarchical topologies and control the tradeoﬀ between the number of wires and the DTI bandwidth


- • Support for high‑performance translation:


- ◦ Scalable conﬁgurable micro TLB and Main TLB (MTLB) in the TBU can reduce the number of translation requests to the TCU
- ◦ TBU direct indexing and MTLB partitioning enable the use of MTLB entries to be managed outside the TBU, improving real‑time translation performance
- ◦ Optimization enables storage of all architecturally‑deﬁned page and block sizes, including contiguous page and block entries, as a single entry in the TBU and TCU TLBs
- ◦ Per‑TBU prioritization in the TCU enables high‑priority transaction streams to be translated before low‑priority streams
- ◦ TCU prefetch of translation tables, which can be enabled on a per‑context basis, improves translation performance for real‑time requesters that access memory linearly
- ◦ Hit‑Under‑Miss (HUM) support in the TBU enables transactions with diﬀerent AXI IDs to be propagated out of order, when a translation is available
- ◦ TBU detects multiple transactions that require the same translation so that only one TBU request to the TCU is required
- ◦ TCU detects multiple translations that require the same table in memory so that only one TCU memory request is required
- ◦ Multi‑level, multi‑stage walk caches in the TCU reduce translation cost by performing only part of the table walk process on a miss
- ◦ A conﬁgurable number of concurrent translations in the TBU and TCU promotes high translation throughput


##### 1.3 Interfaces

The majority of interfaces in MMU-600AE are common to both the TCU and TBU, but each component also has some additional interfaces.

The TCU and TBU both support the following common interfaces:

- • DTI
- • Tie-oﬀs
- • Interrupts


- • PMU snapshot
- • Test and debug
- • LPI clock gating
- • LPI powerdown The TCU also supports the following interfaces:
- • Programming
- • System coherency
- • Queue and Table Walk (QTW)/DVM The TBU also supports the following interfaces:
- • Transaction completer (TBS)
- • Transaction requester (TBM) For more information about the MMU-600AE interfaces, see 2.4 Interfaces on page 31.


##### 1.4 Conﬁgurable options

MMU-600AE is highly conﬁgurable and provides conﬁguration options for each of the main blocks. For the TCU, you can conﬁgure the following:

- • Size of each cache
- • Data width of the QTW/DVM interface
- • Number of translations that can be performed at the same time
- • Number of translation requests that can be accepted from all DTI requesters For the TBU, you can conﬁgure the following:
- • Write data buﬀer depth
- • Size of each cache
- • Number of transactions that can be translated at the same time
- • Number of outstanding read and write transactions that the TBM interface supports
- • Width of data, ID, user, StreamID, and SubstreamID signals on the TBS and TBM interfaces


Depths are speciﬁed as a discrete number of entries.

You can also conﬁgure the DTI interconnect components to meet your system requirements.

##### 1.5 Documentation

MMU-600AE documentation includes the Technical Reference Manual (TRM) and Conﬁguration and Integration Manual (CIM). Both documents are useful to the design ﬂow when incorporating MMU-600AE in an overall System-on-Chip (SoC) design.

Technical Reference Manual The TRM describes the functionality and the eﬀects of functional options on the behavior of MMU-600AE. It is required at all stages of the design ﬂow, see 1.6 Design ﬂow on page

14. The choices that are made in the design ﬂow can mean that some behaviors that are described in the TRM are not relevant. If you are programming MMU-600AE, then contact:

- • The implementer to determine:

- ◦ The build conﬁguration of the implementation
- ◦ The integration, if any, that was performed before implementing MMU-600AE


- • The integrator to determine the pin conﬁguration of the device that you are using.


Conﬁguration and Integration Manual

The CIM describes:

- • The available build conﬁguration options and related issues in selecting them.
- • How to integrate MMU-600AE into an SoC. The CIM also describes the pins that the integrator must tie oﬀ to conﬁgure the macrocells for the required integration.
- • The processes to sign oﬀ on the conﬁguration, integration, and implementation of the design.


The CIM is a conﬁdential book that is only available to licensees.

##### 1.6 Design ﬂow

The design ﬂow is a set of processes that must be followed before an MMU-600AE can be used in a product. The processes are implementation, integration, and programming.

MMU-600AE is delivered as synthesizable RTL. Before it can be used in a product, it must go through the following processes:

Implementation The implementer conﬁgures and synthesizes the RTL to produce a hard macrocell. This process might include integrating RAMs into the design.

Integration The integrator connects the implemented design into an SoC. Integration includes connecting the design to a memory system and peripherals.

Programming The system programmer develops the software to conﬁgure and initialize the MMU-600AE, and tests the required application software.

Each process is separate, and can include implementation and integration choices that aﬀect the behavior and features of the MMU-600AE.

Awareness of compliance to standards and speciﬁcations also aids the design ﬂow. For a summary of MMU-600AE compliance, see 1.1 Compliance on page 9.

The operation of the ﬁnal device depends on: Build conﬁguration

The implementer chooses the options that aﬀect how the RTL source ﬁles are preprocessed. These options usually include or exclude logic that aﬀects one or more of the following:

- • Area
- • Maximum frequency
- • Features of the resulting macrocell


Conﬁguration inputs The integrator conﬁgures some features of the MMU-600AE by tying inputs to speciﬁc values. These conﬁgurations aﬀect the start-up behavior before any software conﬁguration is made.

Software conﬁguration The programmer conﬁgures the MMU-600AE by programming particular values into registers. This conﬁguration aﬀects the behavior of the MMU-600AE.

For an overview of the TCU and TBU conﬁgurable options, see 1.4 Conﬁgurable options on page 13.

##### 1.7 Functional Safety (FuSa)

MMU-600AE is a version of MMU-600, using the same code base, with Functional Safety (FuSa) detection and correction features added. The original MMU-600 logic and functionality are unchanged.

MMU-600AE provides built-in Safety Mechanisms. Functionality includes:

- • Description of the Fault Management Unit (FMU) Programmers View (PV) and the APB interface used to program Safety Mechanisms, inject errors, read fault status information, and clear fault status Error Recovery Interrupt (ERI) and Fault Handling Interrupt (FHI) interrupts.
- • Additional FuSa conﬁguration options.
- • Safety Mechanism descriptions and conﬁguration.
- • FuSa I/Os, reset, and clocking.


- 4. Functional Safety on page 97 describes the functionality in detail.


##### 1.8 Product revisions

The functionality of the MMU-600AE may change between product revisions.

###### r0p0

First release.

###### r1p0

- Table 1-1: Changes introduced in release r1p0


|Aﬀected component or feature|New or updated functionality|Functionality description|
|---|---|---|
|TCU/FMU|New|The TCU now supports a maximum of 62 TBUs which can be conﬁgured using the TCUCFG_NUM_TBU parameter. r0p0 limited this to 14. The IP-XACT ﬁles are also updated to reﬂect this change. The actual number of TBUs connected to the TCU and sending fault information is provided by the TCUCFG_FUSA_TBU_FAULT_WIRE_COUNT parameter. This parameter also now supports a maximum value of 62. The register in the FMU moves to a 64KB page size when FMU TCUCFG_<br><br>FUSA_TBU_FAULT_WIRE_COUNT > 54. Otherwise, they continue to use the 4KB page size as in the previous release.|
|TCU/FMU|New|The number of Fault channels supported is increased to 255. This is conﬁgured using the TCUCFG_FUSA_FCHAN_COUNT parameter.|
|TCU/FMU|Updated|The FMU KEY and LOCK behavior is extended to allow for reordering of split 32 bit write access to the FMU which originated as 64 bit writes. For more information, see 4.4.6 Lock and key mechanism on page 115.|
|TCU/FMU|Updated|The behavior of the FMU in sending err messages to the TBU has changed. Now the TCU does not exchange error messages with the TBU if the TBU is powered down. Error messages to the TBU are generated by writing to one of the following registers:<br><br>• FMU_SMEN<br>• FMU_SMINJERR<br>• FMU_PINGNOW<br>• ERR<n>STATUS corresponding to a TBU error record<br>|


|Aﬀected component or feature|New or updated functionality|Functionality description|
|---|---|---|
|TCU|Updated|IP-XACT changes:<br><br>• Fixed reset value of registers in the IPXACT ﬁle.<br>• Modiﬁed how the reset value from tieoﬀs are represented in IP-XACT.<br>|


## 2. Functional description

The major functional blocks of MMU-600AE are the TBU, TCU, and DTI interconnect. The following ﬁgure shows an example system that uses the MMU-600AE.

###### Figure 2-1: Example system with the MMU-600AE

Fully coherent requesters

Processor Processor GPU

Other requester

PCIe requester with ATS

I/O coherent requesters

CoreLink MMU-600AE

TBU TBU

DTI-TBU DTI-TBU

DTI-ATS

DTI interconnect

DTI

TCU

CoreLink Cache Coherent Interconnect

Completers

Memory system Peripheral Peripheral

MMU-600AE contains the following key components: Translation Buﬀer Unit (TBU)

The TBU contains Translation Lookaside Buﬀers (TLBs) that cache translation tables. MMU-600AE implements at least one TBU for each connected requester, and these TBUs are local to the corresponding requester. See 2.1 Translation Buﬀer Unit on page 19.

Translation Control Unit (TCU) The TCU controls and manages the address translations. MMU-600AE implements a single TCU. In MMU-600AE-based systems, the AMBA® DTI protocol deﬁnes the standard for communicating with the TCU. See 2.2 Translation Control Unit on page 24.

DTI interconnect The DTI interconnect connects multiple TBUs to the TCU. See 2.3 DTI interconnect on page 30.

When an MMU-600AE TBU receives a transaction on the TBS interface, it looks for a matching translation in its TLBs. If the TBU has a matching translation, the TBU uses the cached translation to translate the transaction and outputs the transaction on the TBM interface. If the TBU does

not have a matching cached translation, it requests a new translation from the TCU using the DTI interface.

When the TCU receives a DTI translation request, it uses the QTW interface to perform:

- • Conﬁguration table walks, which return conﬁguration information for the translation context.
- • Translation table walks, that return translation information that is speciﬁc to the transaction address.


The TCU contains caches that reduce the number of conﬁguration and translation table walks that are to be performed. Sometimes no walks are required.

When the TBU receives the translation from the TCU, it stores it in its TLBs. If the translation was successful, the TBU uses it to translate the transaction, otherwise it terminates it.

A processor controls the TCU by:

- • Writing commands to a Command queue in memory.
- • Receiving events from an Event queue in memory.
- • Writing to its conﬁguration registers using the programming interface.


See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version

- 3.0, and version 3.1 for more information about the following:


- • Translation
- • How software communicates with the TCU


##### 2.1 Translation Buﬀer Unit

A typical SMMUv3-based system includes multiple Translation Buﬀer Units (TBUs). Each TBU is located close to the component for which it provides address translation.

A TBU intercepts transactions and provides the required translation from a Translation Lookaside Buﬀer (TLB) if possible. If a TLB does not contain the required translation, the TBU requests translations from the TCU and then caches the translation in one of the TLBs.

The following ﬁgure shows the TBU.

###### Figure 2-2: MMU-600AE TBU

ACE-Lite

MMU-600AE TBU

|Completer interface|
|---|


|Write data buffer|
|---|


|Micro TLB|
|---|


|PMU|
|---|


|Main TLB|
|---|


DTI interface

DTI over AXI4-Stream

|Clock and power control|
|---|


Q-Channel

|Translation manager|
|---|


|Transaction tracker|
|---|


Requester interface

ACE-Lite

The TBU consists of: Requester and completer interfaces

These interfaces manage the TBS and TBM interfaces.

Micro TLB The TBU compares incoming transactions with translations that are cached in the micro TLB before looking in the Main TLB (MTLB). The micro TLB is a fully associative TLB that provides conﬁguration cache and TLB functionality. You can use a tie‑oﬀ signal to conﬁgure the cache replacement policy as either round‑robin or Pseudo Least Recently Used (PLRU).

Main TLB

Each TBU includes an optional MTLB that caches translation table walk entries from:

- • Stage 1 translations
- • Stage 2 translations
- • Stage 1 combined with stage 2 translations


The MTLB is a conﬁgurable four‑way set associative cache structure that uses a random cache replacement policy. If multiple translation sizes are in use, a single transaction might require multiple lookups. Lookups are pipelined to permit a sustained rate of one lookup per cycle. TBU direct indexing enables MMU-600AE to manage MTLB entries externally to the TBU. Direct indexing improves the predictability of TBU performance, for bus requesters that have real-time performance requirements.

Translation manager The translation manager manages translation requests that are in progress. Each transaction occupies a translation slot until it is propagated downstream through the requester interface. All transactions are hazard-checked to reduce the possibility of duplicate translation requests being sent to the TCU. There is no restriction on the ordering of transactions with diﬀerent AXI IDs. Transactions with diﬀerent AXI IDs can be propagated downstream out‑of‑order. All transactions with a given AXI ID value must remain ordered. The translation manager propagates such transactions when the translation is ready, provided no other transaction with the same AXI ID is already waiting. For more information about AXI transaction identiﬁers, see the AMBA® AXI and ACE Protocol Speciﬁcation.

Write data buﬀer The optional write data buﬀer enables write transactions with diﬀerent AXI IDs to progress through the TBU out‑of‑order. It reorders the data to match the downstream transaction order.

###### PMU

The PMU counts TBU performance-related events. Clock and power control

The TBU has its own clock and power control, that the Q‑Channel provides.

DTI interface The requester DTI interface uses the DTI protocol, typically over AXI4‑Stream, to enable the TBU to communicate with a completer component. For MMU-600AE, the completer component is the TCU. Although you can implement DTI over diﬀerent transport protocols, the MMU-600AE interfaces use AXI4‑Stream.

Transaction tracker The transaction trackers manage outstanding read and write transactions, permitting invalidation and synchronization to take place without stalling the AXI interfaces.

###### Related information

TBU direct indexing and MTLB partitioning on page 21 SMMU architectural registers on page 61

###### 2.1.1 TBU direct indexing and MTLB partitioning

TBU direct indexing can help your system to meet real-time translation requirements by enabling MMU-600AE to manage Main TLB (MTLB) entries externally to the TBU.

Direct indexing enables real‑time translation requirements to be met, as follows:

- • It can be guaranteed that diﬀerent streams do not overwrite prefetched entries
- • The MTLB can be partitioned into diﬀerent sets of entries that diﬀerent streams use


If you conﬁgure your system to not use direct indexing, you can select MTLB partitioning. MTLB partitioning has similar behavior, but only the most signiﬁcant TLB index bits are provided, and the other bits are generated internally.

Direct indexing is enabled for a TBU when TBUCFG_DIRECT_IDX = 1. When TBUCFG_DIRECT_IDX = 1, or when an MTLB is partitioned, the width of the AxUSER signals on the TBS interface is extended to convey the indexing information that is required for TBU direct indexing or MTLB partitioning.

The table lists the extended bits in the order MSB ﬁrst.

- Table 2-1: Extended aruser_s and awuser_s bits for MTLB partitioning


|Field name|Width|Description|
|---|---|---|
|mtlbidx|When direct indexing is enabled, the width of this ﬁeld is log2(TBUCFG_MTLB_DEPTH) - 2. When direct indexing is not enabled, the width of this ﬁeld is 0.|MTLB index|
|mtlbway|When direct indexing is enabled, the width of this ﬁeld is 2.<br><br>When direct indexing is not enabled, the width of this ﬁeld is 0.|MTLB way|
|mtlbpart|log2(TBUCFG_MTLB_PARTS)|MTLB partition|
|-|TBUCFG_AWUSER_WIDTH for awuser_s.<br><br>TBUCFG_ARUSER_WIDTH for aruser_s.|Regular AxUSER signals|


If an MTLB is partitioned:

- • The MTLB size is multiplied by TBUCFG_MTLB_PARTS
- • The mtlbpart ﬁeld deﬁnes the log2(TBUCFG_MTLB_PARTS) most signiﬁcant index bits When direct indexing is enabled for a TBU:
- • Lookups and updates to the MTLB use the mtlbidx ﬁeld
- • Updates to the MTLB use the way that mtlbway speciﬁes
- • Lookups to the MTLB operate on all ways simultaneously


To maintain system performance, Arm® recommends that DVM invalidation is disabled on TBUs on which direct indexing is enabled. Disable DVM invalidation by setting the appropriate TCU_NODE_CTRLn.DIS_DVM bit. See 3.5.6 TCU_NODE_CTRLn, TCU Node Control registers on page 78.

###### 2.1.2 AXI USER bits deﬁned by the MMU-600AE TBU

The TBU TBM interface AxUSER signals, aruser_m and awuser_m, have 13 bits more than TBUCFG_AxUSER_WIDTH deﬁnes. These extra bits are output in higher-order bits of aruser_m and awuser_m. The following table shows the MMU-600AE-deﬁned aruser_m and awuser_m bits, where w represents the AXI USER bus width that TBUCFG_AxUSER_WIDTH deﬁnes.

- Table 2-2: MMU-600AE deﬁned aruser_m and awuser_m bits


|Bit position|Value|
|---|---|
|[w+12]|Outer Cacheable|
|[w+11:w+8]|The Stream Table Entry (STE) deﬁnes the attributes|
|[w+7:w+4]|The IMPLEMENTATION DEFINED stage 2 hardware attributes|
|[w+3:w]|The IMPLEMENTATION DEFINED stage 1 hardware attributes|


Bits[119:116] of the STE are IMPLEMENTATION DEFINED in SMMUv3. When the TCU sends a DTI translation response message to a TBU, it outputs these bits in the DTI_TBU_TRANS_RESP.CTXTATTR ﬁeld. The MMU-600AE TBU outputs these bits as STE‑deﬁned attributes.

The TCU DTI_TBU_TRANS_RESP response also includes S1HWATTR[3:0] and S2HWATTR[3:0] ﬁelds. These ﬁelds provide the IMPLEMENTATION DEFINED hardware attributes for each stage of translation. The TBU reports these ﬁelds using awuser_m and aruser_m.

The S1HWATTR and S2HWATTR ﬁelds are calculated as follows:

- S1HWATTR S1HWATTR[n] is equal to bit[n+59] of the stage 1 translation table ﬁnal-level descriptor when both of the following conditions apply:

- • SMMUv3 permits the bit to have an IMPLEMENTATION DEFINED hardware use
- • SMMUv3 does not permit bit[n+59] of the stage 2 translation table ﬁnal‑level descriptor to have an IMPLEMENTATION DEFINED hardware use


- Otherwise, S1HWATTR[n] = 0.

S2HWATTR S2HWATTR[n] is equal to bit[n+59] of the stage 2 translation table ﬁnal-level descriptor when SMMUv3 permits that bit to have an IMPLEMENTATION DEFINED hardware use.

- Otherwise, S2HWATTR[n] = 0.




Arm recommends that systems always use the value of S1HWATTR[n] | S2HWATTR[n], that is:

- • The value of the corresponding stage 2 ﬁnal-level descriptor bit, if it is enabled for hardware use and stage 2 translation is enabled
- • The value of the corresponding stage 1 ﬁnal-level descriptor bit, if it is enabled for hardware use and stage 1 translation is enabled
- • Otherwise, 0


###### Related information

Requester interface memory type attribute handling on page 51

##### 2.2 Translation Control Unit

A typical SMMUv3‑based system includes a single Translation Control Unit (TCU). The TCU is usually the largest block in the MMU-600AE system, and performs several roles.

The TCU performs the following:

- • Manages the memory queues
- • Performs translation table walks
- • Performs conﬁguration table walks
- • Implements backup caching structures
- • Implements the SMMU programmers model The following ﬁgure shows the TCU. Figure 2-3: MMU-600AE TCU


MMU-600AE TCU

|Translation request buffer|
|---|


|Configuration cache|
|---|


|DTI interface|
|---|


|Walk caches and TLB<br><br>|S1L0|
|---|
<br><br>|S1L1|
|---|
<br><br>|S1L2|
|---|
<br><br>|S1L3|
|---|
<br><br>|S2L0|
|---|
<br><br>|S2L1|
|---|
<br><br>|S2L2|
|---|
<br><br>|S2L3|
|---|
|
|---|


RAM-based logic

DTI over AXI4-Stream

|PMU|
|---|


|Translation manager|
|---|


|Clock and power control|
|---|


|Queue manager|
|---|


Q-Channel

|Register file|
|---|


QTW/DVM interface

APB

ACE-Lite + DVM

The TCU consists of: Walk caches

The TCU includes separate four-way set-associative walk caches to store results of translation table walks. During MMU-600AE conﬁguration, the cache line entries are split to create separate walk caches that are reserved for:

- • Stage 1 level 0 table entries
- • Stage 1 level 1 table and block entries
- • Stage 1 level 2 table and block entries
- • Stage 1 level 3 table entries
- • Stage 2 level 0 table entries
- • Stage 2 level 1 table and block entries
- • Stage 2 level 2 table and block entries
- • Stage 2 level 3 table entries


To enable and disable the walk cache for a particular stage and level of translation, use the TCU_CTRL register. If an error occurs for a cache line entry, the TCU_ERRSTATUS register identiﬁes the aﬀected entry.

The walk cache is useful in cases where a translation request results in a miss in other TCU caches. A subsequent hit in the walk cache requires only a single memory access to complete the translation table walk and fetch the required descriptor.

Conﬁguration cache The conﬁguration caches are 4-way set-associative cache structures that store conﬁguration information. Each entry stores the Context Descriptor (CD) and Stream Table Entry (STE) contents for a translation context.

The conﬁguration cache does not cache the contents of intermediate conﬁguration tables.

Translation manager The translation manager manages translation requests that are in progress. All translation table walks and conﬁguration table walks are hazard-checked to reduce the possibility of multiple transactions requesting duplicate walks.

Translation request buﬀer The translation request buﬀer stores translation requests from TBUs when all translation manager slots are full. The translation request buﬀer supports more slots than the translation manager. When correctly conﬁgured, this buﬀer has enough space to store all translation requests that TBUs can issue simultaneously. This buﬀer therefore prevents the DTI interface from becoming blocked.

###### PMU

The PMU counts TCU performance‑related events. Clock and power control

The TCU has its own clock and power control, that the Q‑Channel provides.

Queue manager The queue manager manages all SMMUv3 Command queues and Event queues that are stored in memory.

QTW/DVM interface The Queue and Table Walk (QTW)/Distributed Virtual Memory (DVM) interface is an ACELite+DVM requester interface.

Register ﬁle The register ﬁle implements the SMMUv3 programmers model, as the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1 deﬁnes.

DTI interface The completer DTI interface uses the DTI protocol, typically over AXI4‑Stream, to enable the TCU to communicate with a requester component. For MMU-600AE, the requester component is either a TBU or a PCIe Root Complex that implements ATS.

###### Related information

Interfaces on page 31 TCU transaction handling on page 27 TCU prefetch on page 28 SMMU architectural registers on page 61

###### 2.2.1 Distributed Virtual Memory messages

The QTW/DVM interface supports Distributed Virtual Memory (DVM) messages. MMU-600AE supports DVMv8.1.

The interface supports DVM transactions of message types TLB Invalidate and Synchronization. The interface accepts all other DVM transaction message types, and sends a snoop response, but otherwise ignores such transactions.

Tie the sup_btm input signal HIGH when the system supports Broadcast TLB Maintenance. When Broadcast TLB Maintenance is supported, you can use SMMU_CR2 and SMMU_S_CR2 to control how the SMMU handles TLB Invalidate operations as follows:

- SMMU_CR2.PTM = 0 Non‑secure TLB Invalidate operations are applied to the TLBs.
- SMMU_CR2.PTM = 1 Non‑secure TLB Invalidate operations have no eﬀect.


- SMMU_S_CR2.PTM = 0 Secure TLB Invalidate operations are applied to the TLBs.
- SMMU_S_CR2.PTM = 1 Secure TLB Invalidate operations have no eﬀect.


When sup_btm is tied HIGH, the reset value of SMMU_CR2.PTM and SMMU_S_CR2.PTM is 1.

Although TLB Invalidate operations have no eﬀect when PTM = 1, the QTW/DVM interface still returns the appropriate response.

The QTW/DVM interface might receive DVM Sync transactions without receiving a DVM TLB Invalidate transaction, or when the PTM bits have masked a TLB Invalidate. If no DVM TLB Invalidate operations have occurred since the most recent DVM Sync transaction, subsequent DVM Sync transactions result in an immediate DVM Complete transaction. This behavior ensures that the TCU does not aﬀect system DVM performance unless TLB Invalidate operations are performed.

The DTI interface allocates the access permissions and shareability of DVM Complete transactions as follows:

- • ARPROT = 0b000, indicating Unprivileged, Secure, Data access
- • ARDOMAIN = 0b01, indicating Inner Shareable


For a DVM Operation or DVM Sync request on the AC channel, the snoop response signal CRRESP[4:0] is always set to 0b00000.

###### 2.2.2 TCU transaction handling

The transaction width, burst length, and transfer size that the TCU supports depend on the transaction type.

The following table shows the TCU support for read transactions.

- Table 2-3: TCU support for read transactions


|Transaction type|Transaction width, bits|ARID[n:1]|ARID[0]|
|---|---|---|---|
|Stage 1 Stream table lookup|64|PTW slot number|1|
|Stream table lookup|256|PTW slot number|1|
|Translation table lookup|64|PTW slot number|1|
|Command queue read|128|All 0|0|
|DVM Complete|-|All 1|0|


DVM Complete transactions are always one beat of full data width. Command queue reads and DVM Complete transactions are independent of translation slots. Therefore, the maximum number of read transactions that the TCU can issue at any time is TCUCFG_PTW_SLOTS + 2.

The following table shows the TCU support for write transactions.

- Table 2-4: TCU support for write transactions


|Transaction type|Transaction width, bits|AWID|
|---|---|---|
|Event queue write|256|0|
|PRI queue write|128|0|
|Message Signaled Interrupt (MSI)|32|0|


Only one write transaction can be outstanding at a time. All read and write transactions are aligned to the transaction size.

###### 2.2.3 TCU prefetch

The TCU can prefetch translations on a per‑context basis to improve translation performance for real‑time requesters that access memory linearly. If TCU prefetch is enabled, a second translation request occurs after the original request. This second translation request is regarded as the prefetch because it is an advance request of the next translation that is expected to be requested. This second request is Speculative and is used to allocate into the caches of the TCU.

Software can enable TCU prefetch for a particular translation context by programming the Stream Table Entry (STE). Bits [121:120] are IMPLEMENTATION DEFINED in the SMMUv3 architecture. See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

MMU-600AE uses these bits for the PF ﬁeld as follows: PF, bits [121:120]

This ﬁeld determines whether prefetch is enabled or disabled for the translation context that this STE deﬁnes as follows:

0b00 Prefetching disabled 0b01 Reserved

- 0b10 Forward prefetching
- 0b11 Backward prefetching


Prefetching disabled

TCU prefetch does not occur. Reserved

Reserved values must not be used.

Forward prefetching The address to be prefetched is the ﬁrst address following the end of the translation range, as DTI_TBU_TRANS_RESP.TRANS_RNG/DTI_ATS_TRANS_RESP.TRANS_RNG indicates.

Backward prefetching The address to be prefetched is the last address before the beginning of the translation range, as DTI_TBU_TRANS_RESP.TRANS_RNG/DTI_ATS_TRANS_RESP.TRANS_RNG indicates.

Whenever a miss occurs in the MicroTLB and Main TLB of the TBU, the TBU sends a translation request to the TCU. If the STE for the translation is programmed to enable prefetch, a prefetch may be done after the original request is complete:

- 1. The TCU completes the original translation request. The STE.PF ﬁeld indicates whether prefetch is enabled.
- 2. If prefetch is enabled, the next translation request is issued to the same TCU translation slot. This prefetch request is Speculative, and only allocates into the TCU walk caches.
- 3. A translation response for the prefetch is not returned to the TBU.


When the TCU handles each incoming translation request from the TBU, translation table walks might or might not occur depending on whether there is a hit in each level of walk cache that is looked up. Translation table walks also might or might not occur for the subsequent prefetch request. The number of memory accesses that are performed for this prefetch are unrelated to the number of memory accesses that are performed for the original translation request.

Consider the following examples:

- 1. An incoming translation request might hit in the lowest level of walk cache, but the subsequent prefetch request might still require at least one translation table walk to memory.
- 2. The original translation request might require multiple translation table walks, but the subsequent prefetch request might hit in the lowest level of walk cache and not require any memory accesses. If the prefetch request hits in the lowest level of walk cache, then the walk caches are not updated and no memory accesses are performed.


The walk cache uses a round‑robin replacement policy.

The prefetch can only occur when the original request is complete, even if translation table walks are required. The prefetch must wait for completion because it uses the same translation slot as the original request. If the TCU receives a non‑speculative request for the next translation, it can handle this request in a separate translation slot before the ﬁrst prefetch is initiated. Therefore, TCU prefetch only results in a performance advantage if the prefetch can complete before the next translation is initiated. This occurs when the number of cycles between sequential translation requests from the TBU is greater than the number of cycles that is taken for the TCU to complete the original translation request and to complete the subsequent prefetch.

Even if TCU prefetch is enabled, a prefetch does not occur if one of the following caused the original request:

- • A Speculative translation request. That is, DTI_TBU_TRANS_REQ.PERM[1:0] = 2'b11, if a TBU receives a StashOnceShared, StashOnceUnique, or StashTranslation transaction.


- • A translation request for an atomic transaction that provides a data response. That is, DTI_TBU_TRANS_REQ.PERM[1:0] = 2'b10, if a TBU receives an AtomicLoad, AtomicSwap, or AtomicCompare transaction. If the original translation request returns one the following, prefetch also does not occur:
- • Fault response
- • Global bypass response
- • Stream bypass response


Prefetch applies to both ATS and non‑ATS translation requests.

##### 2.3 DTI interconnect

MMU-600AE TBU and TCUs use a DTI interface to communicate. The DTI interconnect enables the DTI interface to use the AXI4‑Stream transport protocol.

The DTI interconnect can connect any components that conform to the AXI4‑Stream protocol, as the AMBA® DTI Protocol Speciﬁcation deﬁnes.

The DTI interconnect contains internal components that are hierarchically composable, that is, they can be connected in diﬀerent ways to suit your system requirements. For example, within an MMU-600AE system, you can use the switch component to combine the DTI interfaces of multiple TBUs into a single DTI interface. You can then connect the combined DTI interface to another DTI interconnect that is closer to the TCU.

The DTI interconnect includes switch, sizer, and register slice components. Switch

The switch connects multiple DTI requesters, such as TBUs, to a DTI completer such as a TCU. The switch implements the following parallel networks:

- • For TBU to TCU traﬃc, a network that connects multiple AXI4‑Stream completer interfaces to a single AXI4‑Stream requester interface
- • For TCU to TBU traﬃc, a network that connects a single AXI4‑Stream completer interface to multiple AXI4‑Stream requester interfaces


The switch does not store any data, and therefore does not require a Q‑Channel clock‑gating interface.

###### Sizer

The sizer connects channels that have diﬀerent data widths, enabling diﬀerent tradeoﬀs of bandwidth to area. The sizer supports conversion between any of the supported AXI4‑Stream data widths:

- • 1 byte
- • 4 bytes
- • 10 bytes
- • 20 bytes The sizer includes a Q‑Channel interface to provide clock‑gating control.


Register slice Use the register slice to improve timing. The register slice includes a Q‑Channel interface to provide clock‑gating control.

The MMU-600AE DTI interconnect components do not include a component to connect diﬀerent clock and power domains. You can connect DTI interfaces in diﬀerent clock and power domains by using the Bidirectional AXI4-Stream (BAS) conﬁguration of the ADB-400 AMBA® Domain Bridge.

##### 2.4 Interfaces

MMU-600AE includes interfaces for each of the TCU, TBU, and DTI interconnect components. The DTI interconnect consists of switch, sizer, and register slice components that you can connect separately, and these components therefore have their own interfaces. The PMU snapshot interface is common to both TCU and TBU.

###### 2.4.1 TCU interfaces

The MMU-600AE TCU includes several requester and completer interfaces. The following ﬁgure shows the TCU interfaces.

###### Figure 2-4: TCU interfaces

|MMU-600AE TCU|MMU-600AE TCU|
|---|---|
|ACE-Lit<br><br>Coherency connection signaling|e+DVM APB4<br><br>|


Clock and reset

Q-Channel DTI LPI_CG Q-Channel

LPI_PD

SYSCO QTW/DVM

PROG

###### 2.4.1.1 TCU Queue and Table Walk/Distributed Virtual Memory interface

The Queue and Table Walk/Distributed Virtual Memory (QTW/DVM) interface for the MMU-600AE TCU is an ACE-Lite+DVM requester interface.

The QTW/DVM interface issues the following transaction types:

- • ReadNoSnoop
- • WriteNoSnoop
- • ReadOnce
- • WriteUnique
- • DVM Complete


The QTW/DVM interface uses the write address transaction ID signal awid_qtw, and the read address transaction ID signal, arid_qtw. The value of awid_qtw is always 0, and the value of arid_qtw depends on the transaction type. The following table shows the possible values of arid_qtw.

- Table 2-5: Possible arid_qtw values


|Transaction type|arid_qtw[n:1]|arid_qtw[0]|
|---|---|---|
|Translation table walk|Indicates the slot that is requesting the translation table walk|1|
|Command queue read|All bits = 0|0|
|DVM Complete|All bits = 1|0|


For a full list of QTW/DVM interface signals, see A.2 TCU QTW/DVM interface signals for MMU-600AE on page 162.

To support 16-bit Virtual Machine IDentiﬁers (VMIDs), the interface provides DVMv8.1 support. For information about DVM messages, see 2.2.1 Distributed Virtual Memory messages on page 26.

The interface also supports some other AXI5 extensions, as indicated in 2.11.2.4 AXI5 support on page 58.

The interface does not issue cache maintenance operations or exclusive accesses.

###### Related information

Error responses on page 49

###### 2.4.1.2 TCU PROG interface

The PROG interface is an AMBA APB4 completer interface. It enables software to program the MMU-600AE internal registers and read the Performance Monitoring Unit (PMU) registers and the Debug registers.

This interface runs synchronously with the other TCU interfaces. The applicable address width for this interface depends on the value of TCUCFG_NUM_TBU:

- • When TCUCFG_NUM_TBU = 14, the address width is 21 bits
- • When TCUCFG_NUM_TBU = 62, the address width is 23 bits Transactions are Read-As-Zero, Writes Ignored (RAZ/WI) when any of the following apply:
- • An unimplemented register is accessed
- • PSTRB[3:0] is not 0b1111 for write transfers
- • PPROT[1] is not set to 0 for Secure register accesses


For more information, see the AMBA® APB Protocol Speciﬁcation.

Related information

TCU programming interface signals for MMU-600AE on page 164

###### 2.4.1.3 TCU LPI_PD interface

This Q‑Channel completer interface manages Low Power Interface (LPI) powerdown for the MMU-600AE TCU.

For more information, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

###### Related information

TCU LPI_PD interface signals for MMU-600AE on page 165

###### 2.4.1.4 TCU LPI_CG interface

This Q‑Channel completer interface enables Low Power Interface (LPI) clock gating for the MMU-600AE TCU.

For more information, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

Related information

TCU LPI_CG interface signals for MMU-600AE on page 166

###### 2.4.1.5 TCU DTI interface

The DTI interface manages communication between the MMU-600AE TBUs and the TCU, using the DTI protocol. The DTI protocol can be conveyed over diﬀerent transport layer mediums, including AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled).

The TCU includes a completer DTI interface and each TBU includes a requester DTI interface. To permit bidirectional communication, each DTI interface includes:

Requester interface

One AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled) requester interface Completer interface

One AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled) completer interface

For more information, see the AMBA® DTI Protocol Speciﬁcation and the AMBA® 4 AXI4‑Stream Protocol Speciﬁcation.

Related information

DTI protocol overview on page 41 TCU DTI interface signals for MMU-600AE on page 166

###### 2.4.1.6 TCU interrupt interfacesThis interface provides global, per-context, and performance interrupts.

Related information

TCU interrupt signals for MMU-600AE on page 167

###### 2.4.1.7 TCU SYSCO interface

MMU-600AE provides a hardware system coherency interface. This requester interface permits the TCU to remove itself from a coherency domain in response to an LPI request.

The SYSCO interface uses the syscoreq and syscoack handshake signals to enter or exit a coherency domain.

If the sup_btm signal is tied LOW, the syscoreq signal is always driven LOW and syscoack is ignored.

For more information about the signals used by this interface, see A.4 TCU SYSCO interface signals for MMU-600AE on page 164.

###### 2.4.1.8 TCU tie-oﬀ signals

The TCU tie-oﬀ signals enable you to initialize various operating parameters on exit from reset state.

At reset, the value of each tie-oﬀ signal controls the respective bits in the SMMU_IDR0 Register.

###### Related information

TCU tie-oﬀ signals for MMU-600AE on page 170

- 2.4.2 TBU interfaces Each MMU-600AE TBU includes several requester and completer interfaces. The following ﬁgure shows the TBU interfaces:


###### Figure 2-5: TBU interfaces

TBS

###### ACE-Lite

|MMU-600AE TBU|MMU-600AE TBU|DTI|
|---|---|---|
|MMU-600AE TBU|MMU-600AE TBU| |
|ACE-|Lite|Lite|


Clock and reset

Q-Channel LPI_CG Q-Channel

LPI_PD

TBM

###### 2.4.2.1 TBU TBS interface

The transaction completer (TBS) interface is an ACE5-Lite interface on which the MMU-600AE TBU receives incoming untranslated memory accesses.

This interface supports a 64‑bit address width. The interface implements optional signals to support the following AXI5 extensions:

- • Wakeup_Signals
- • Untranslated_Transactions
- • Cache_Stash_Transactions
- • DeAllocation_Transactions


- For a full list of interface signals, see A.14 TBU TBS interface signals for MMU-600AE on page 171. The TBS interface supports ACE Exclusive accesses.


If a transaction is terminated in the TBU, the transaction tracker returns the transaction with the user‑deﬁned AXI RUSER and BUSER bits set to 0.

Related information

Error responses on page 49

###### 2.4.2.2 TBU TBM interface

The TBM transaction requester interface is an ACE5‑Lite interface on which the MMU-600AE TBU sends outgoing translated memory accesses.

The AXI ID of a transaction on this interface is the same as the AXI ID of the corresponding transaction on the TBS interface.

This interface supports a 48‑bit address width, and TBUCFG_DATA_WIDTH deﬁnes the data width. This interface can issue read and write transactions until the outstanding transaction limit is reached. MMU-600AE provides parameters that permit you to conﬁgure:

- • The outstanding read transactions limit
- • The outstanding write transactions limit
- • The total outstanding read and write transactions limit. The interface implements optional signals to support the following AXI5 extensions:
- • Wakeup_Signals
- • Untranslated_Transactions
- • Cache_Stash_Transactions


- • DeAllocation_Transactions


- For a full list of interface signals, see A.15 TBU TBM interface signals for MMU-600AE on page 173.


When receiving an SLVERR or DECERR response to a downstream transaction, the TBM interface propagates the same response to the TBS interface.

The TBM interface supports ACE Exclusive accesses. For more information about MMU-600AE ACE-Lite implementation, see 2.11.2 AMBA support on page 55.

###### 2.4.2.3 TBU LPI_PD interface

This Q‑Channel completer interface manages Lower Power Interface (LPI) powerdown for the MMU-600AE TBU.

- For a list interface signals, see A.17 TBU LPI_PD interface signals for MMU-600AE on page 175.

For more information, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

2.4.2.4 TBU LPI_CG interface

This Q‑Channel completer interface enables Lower Power Interface (LPI) clock gating for the MMU-600AE TBU.

- For a list interface signals, see A.18 TBU LPI_CG interface signals for MMU-600AE on page 176.


For more information, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

###### 2.4.2.5 TBU DTI interface

The TBU DTI interface enables the MMU-600AE TBU to request translations from the TCU. This interface uses the DTI‑TBU protocol for communication between the TBU and the TCU.

The TCU includes a completer DTI interface and each TBU includes a requester DTI interface. To permit bidirectional communication, each DTI interface includes:

###### Requester interface

One AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled) requester interface Completer interface

One AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled) completer interface

For more information, see the AMBA® DTI Protocol Speciﬁcation and the AMBA® 4 AXI4‑Stream Protocol Speciﬁcation.

###### Related information

DTI protocol overview on page 41 TBU DTI interface signals for MMU-600AE on page 176

- 2.4.2.6 TBU interrupt interfaces This interface provides global, per-context, and performance interrupts.

Related information

TBU interrupt signals for MMU-600AE on page 177

- 2.4.2.7 TBU tie-oﬀ signals


The TBU tie‑oﬀ signals enable you to initialize various operating parameters on exit from reset state.

###### Related information

TBU tie-oﬀ signals for MMU-600AE on page 177

###### 2.4.3 DTI interconnect interfaces

The MMU-600AE DTI interconnect includes interfaces for each of the switch, sizer, and register slice components.

Switch The switch interfaces connect to multiple DTI requesters, such as TBUs, and to a DTI completer such as a TCU.

- See 2.4.3.1 DTI interconnect switch interfaces on page 38 for more information.

Sizer

The sizer interfaces are for connecting to channels that have diﬀerent data widths. The sizer also has a Q-Channel clock gating interface.

- See 2.4.3.2 DTI interconnect sizer interfaces on page 39 for more information for more information.


Register slice Use the register slice to improve timing. The register slice also has a Q-Channel clock gating interface.

See 2.4.3.3 DTI interconnect register slice interfaces on page 40 for more information for more information.

###### 2.4.3.1 DTI interconnect switch interfaces

The MMU-600AE DTI interconnect switch component includes dedicated interfaces. The following ﬁgure shows the DTI interconnect switch interfaces.

###### Figure 2-6: DTI interconnect switch interfaces

| | | | | |
|---|---|---|---|---|
|DTI interconnect switch<br><br>DN_M<br><br>DN_S0<br><br>UP_M<br><br>UP_S0 DN_S1 UP_S1 DN_Sn UP_Sn|DTI interconnect switch<br><br>DN_M<br><br>DN_S0<br><br>UP_M<br><br>UP_S0 DN_S1 UP_S1 DN_Sn UP_Sn|DTI interconnect switch<br><br>DN_M<br><br>DN_S0<br><br>UP_M<br><br>UP_S0 DN_S1 UP_S1 DN_Sn UP_Sn|DTI interconnect switch<br><br>DN_M<br><br>DN_S0<br><br>UP_M<br><br>UP_S0 DN_S1 UP_S1 DN_Sn UP_Sn|DTI interconnect switch<br><br>DN_M<br><br>DN_S0<br><br>UP_M<br><br>UP_S0 DN_S1 UP_S1 DN_Sn UP_Sn|
| | | | | |


The following table provides more information about the switch interfaces.

- Table 2-6: DTI interconnect switch interfaces


|Interface|Interface type|Protocol|Description|
|---|---|---|---|
|DN_Sn|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer downstream interface. One DN_Sn interface is present for each completer interface.|
|UP_Sn|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer upstream interface. One UP_Sn interface is present for each completer interface.|
|DN_M|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester downstream interface|
|UP_M|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester upstream interface|


The interconnect switch does not store any data, and therefore does not require a Q‑Channel clock gating interface.

###### 2.4.3.2 DTI interconnect sizer interfaces

The MMU-600AE DTI interconnect sizer component includes dedicated interfaces. The following ﬁgure shows the DTI interconnect sizer interfaces.

###### Figure 2-7: DTI interconnect sizer interfaces

| | | |
|---|---|---|
|DTI interconnect sizer UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|DTI interconnect sizer UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|DTI interconnect sizer UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|
| | | |


The following table provides more information about the sizer interfaces.

- Table 2-7: DTI interconnect sizer interfaces


|Interface|Interface type|Protocol|Description|
|---|---|---|---|
|LPI_CG|Completer|Q‑Channel|Clock gating interface|
|DN_S|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer downstream interface|
|UP_S|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer upstream interface|
|DN_M|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester downstream interface|
|UP_M|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester upstream interface|


###### 2.4.3.3 DTI interconnect register slice interfaces

The MMU-600AE DTI interconnect register slice component includes dedicated interfaces. The following ﬁgure shows the DTI interconnect register slice interfaces.

###### Figure 2-8: DTI interconnect register slice interfaces

| | | |
|---|---|---|
|DTI interconnect register slice UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|DTI interconnect register slice UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|DTI interconnect register slice UP_M<br><br>DN_S<br><br>DN_M<br><br>LPI_CG UP_S|
| | | |


The following table provides more information about the register slice interfaces.

- Table 2-8: DTI interconnect register slice interfaces


|Interface|Interface type|Protocol|Description|
|---|---|---|---|
|LPI_CG|Completer|Q‑Channel|Clock gating interface|
|DN_S|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer downstream interface|
|UP_S|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Completer upstream interface|
|DN_M|Requester|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester downstream interface|
|UP_M|Completer|AXI4‑Stream (with AXI5 Wakeup_Signal added and enabled)|Requester upstream interface|


##### 2.5 DTI protocol overview

In an MMU-600AE-based system, the AMBA® DTI protocol deﬁnes the standard for communicating with a TCU.

The AMBA® DTI protocol includes both:

- • DTI-TBU protocol, for communication between a TBU and a TCU
- • DTI-ATS protocol, for communication between a PCIe Root Complex and a TCU

The DTI protocol is a point-to-point protocol. Each channel consists of a link, a DTI requester, and a DTI completer. The DTI requesters in the respective protocols are:

- • The TBU, in the DTI-TBU protocol
- • The PCIe Root Complex, in the DTI-ATS protocol The DTI completer in both DTI-TBU and DTI-ATS is the TCU.


- DTI requesters and completers communicate using deﬁned DTI messages. The DTI protocol deﬁnes the following message groups:
- • Page request
- • Register access
- • Translation request
- • Connection and disconnection
- • Invalidation and synchronization


The DTI_TBU_CONDIS_REQ message initiates a TBU connection or disconnection handshake. The TBU uses this message to connect to the TCU. During connection, the TBU can specify the number of requested translation tokens.

The TBU uses the TOK_TRANS_REQ ﬁeld to request translation tokens. The max_tok_trans signal deﬁnes the number of translation tokens that the TBU requests.

The TBU uses the TOK_INV_GNT ﬁeld to grant invalidation tokens. The TBU grants only one invalidation token, and the TCU is only capable of issuing one invalidate message at a time.

A DTI requester uses a DTI_TBU_CONDIS_REQ or a DTI_ATS_CONDIS_REQ message to initiate a connection handshake. If the requester provides a TID value that is greater than the maximum supported TID that TCUCFG_NUM_TBU deﬁnes, the completer sends a Connect Deny message.

A translation request to the TCU where StreamID ≥ 224 results in a fault and an SMMUv3 C_BAD_STREAMID event. If the TBU receives an invalidation request where StreamID ≥ 224, any comparisons with a StreamID value fail. No TLB entries are invalidated, but other eﬀects that do not consider the supplied StreamID occur as normal.

- • The TBU never generates translation requests with StreamID ≥ 224
- • The TCU never generates invalidation requests with StreamID ≥ 224


See the AMBA® DTI Protocol Speciﬁcation for more information.

##### 2.6 Performance Monitoring Unit

MMU-600AE includes a PMU for the TCU and a PMU for each TBU. The PMU events and counters indicate the runtime performance of MMU-600AE.

MMU-600AE includes logic to gather various statistics on the operation of the MMU during runtime, using events and counters. These events, which the SMMUv3 architecture deﬁnes, provide useful information about the behavior of the MMU. You can use this information when debugging or proﬁling traﬃc.

###### 2.6.1 SMMUv3 architectural performance events

The MMU-600AE TCU and TBU implement performance events that the SMMUv3 Performance Monitor extension deﬁnes.

The SMMU_PMCG_SMR0 register can ﬁlter some events so that only events with a particular StreamID are counted. This event ﬁltering includes:

- • Speculative transactions and translations
- • Transactions and translations that result in a terminated transaction or a translation fault The following table shows the architecturally deﬁned MMU-600AE TCU performance events.


- Table 2-9: SMMUv3 performance events for the TCU

|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|Clock cycle|0x0|No|Counts clock cycles.<br><br>Cycles where the clock is gated after a clock Q-Channel handshake are not counted.|
|Transaction|0x1|Yes|Counts translation requests that originate from a DTI-TBU or DTI-ATS requester|
|TLB miss caused by incoming transaction or translation request|0x2|Yes|Counts translation requests where the translation walks new translation table entries|
|Conﬁguration cache miss caused by transaction or translation request|0x3|Yes|Counts translation requests where the translation walks new conﬁguration table entries|
|Translation table walk access|0x4|Yes|Counts translation table walk accesses|
|Conﬁguration structure access|0x5|Yes|Counts conﬁguration table walk accesses|
|PCIe ATS Translation Request received|0x6|Yes|Counts translation requests that originate from a DTI-ATS requester|


The following table shows the architecturally deﬁned MMU-600AE TBU performance events.

- Table 2-10: SMMUv3 performance events for the TBU


|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|Clock cycle|0x0|No|Counts clock cycles.<br><br>Cycles where the clock is gated after a clock Q-Channel handshake are not counted.|
|Transaction|0x1|Yes|Counts transactions that are issued on the TBM interface|
|TLB miss caused by incoming transaction or translation request|0x2|Yes|Counts non-speculative translation requests that are issued to the TCU|
|PCIe ATS Translation Request received|0x7|Yes|Counts ATS-translated transactions that are issued on the TBM interface|


See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1 for more information.

###### 2.6.2 MMU-600AE TCU events

The MMU-600AE PMU can be conﬁgured to monitor a range of IMPLEMENTATION DEFINED TCU performance events.

The SMMU_PMCG_SMR0 register can ﬁlter some TCU performance events so that only events with a particular StreamID are counted. This event ﬁltering includes:

- • Speculative transactions and translations
- • Transactions and translations that result in a terminated transaction or a translation fault The following table shows the TCU performance events.


- Table 2-11: MMU-600AE TCU performance events


|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|S1L0WC lookup|0x80|Yes|Counts translation requests that access the S1L0WC walk cache|
|S1L0WC miss|0x81|Yes|Counts translation requests that access the S1L0WC walk cache and do not result in a hit|
|S1L1WC lookup|0x82|Yes|Counts translation requests that access the S1L1WC walk cache|
|S1L1WC miss|0x83|Yes|Counts translation requests that access the S1L1WC walk cache and do not result in a hit|
|S1L2WC lookup|0x84|Yes|Counts translation requests that access the S1L2WC walk cache|
|S1L2WC miss|0x85|Yes|Counts translation requests that access the S1L2WC walk cache and do not result in a hit|
|S1L3WC lookup|0x86|Yes|Counts translation requests that access the S1L3WC walk cache|
|S1L3WC miss|0x87|Yes|Counts translation requests that access the S1L3WC walk cache and do not result in a hit|
|S2L0WC lookup|0x88|Yes|Counts translation requests that access the S2L0WC walk cache|
|S2L0WC miss|0x89|Yes|Counts translation requests that access the S2L0WC walk cache and do not result in a hit|
|S2L1WC lookup|0x8A|Yes|Counts translation requests that access the S2L1WC walk cache|
|S2L1WC miss|0x8B|Yes|Counts translation requests that access the S2L1WC walk cache and do not result in a hit|
|S2L2WC lookup|0x8C|Yes|Counts translation requests that access the S2L2WC walk cache|
|S2L2WC miss|0x8D|Yes|Counts translation requests that access the S2L2WC walk cache and do not result in a hit|
|S2L3WC lookup|0x8E|Yes|Counts translation requests that access the S2L3WC walk cache|
|S2L3WC miss|0x8F|Yes|Counts translation requests that access the S2L3WC walk cache and do not result in a hit|


|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|WC read|0x90|Yes|Counts reads from the walk cache RAMs, excluding reads that are caused by invalidation requests<br><br>Note: A single walk cache lookup might result in multiple RAM reads. This behavior permits contiguous entries to be located.|
|Buﬀered translation|0x91|Yes|Counts translations written to the translation request buﬀer because all translation slots are full.|
|CC lookup|0x92|Yes|Counts lookups into the conﬁguration cache|
|CC read|0x93|Yes|Counts reads from the conﬁguration cache RAMs, excluding reads that are caused by invalidation requests<br><br>Note: A single cache lookup might result in multiple RAM reads. This behavior permits contiguous entries to be located.|
|CC miss|0x94|Yes|Counts lookups into the conﬁguration cache that result in a miss|
|Speculative translation|0xA0|Yes|Counts translation requests that are marked as speculative|
|S1L0WC error|0xC0|No|RAS corrected error in S1L0 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S1L1WC error|0xC1|No|RAS corrected error in S1L1 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S1L2WC error|0xC2|No|RAS corrected error in S1L2 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S1L3WC error|0xC3|No|RAS corrected error in S1L3 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S2L0WC error|0xC4|No|RAS corrected error in S2L0 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S2L1WC error|0xC5|No|RAS corrected error in S2L1 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S2L2WC error|0xC6|No|RAS corrected error in S2L2 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|S2L3WC error|0xC7|No|RAS corrected error in S2L3 walk cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|Conﬁguration cache error|0xC8|No|RAS corrected error in conﬁguration cache.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|


A single DTI translation request might correspond to multiple translation request events in either of the following circumstances:

- • A translation results in a stall fault event and is restarted.
- • If a translation results in a stall fault event because of the Event queue being full, the translation is retried when an Event queue slot becomes available.


###### 2.6.3 MMU-600AE TBU events

The MMU-600AE PMU can be conﬁgured to monitor a range of IMPLEMENTATION DEFINED TBU performance events.

The SMMU_PMCG_SMR0 register can ﬁlter the TBU performance events so that only events with a particular StreamID are counted. This event ﬁltering includes:

- • Speculative transactions and translations
- • Transactions and translations that result in a terminated transaction or a translation fault The following table shows the TBU performance events.


- Table 2-12: MMU-600AE TBU performance events


|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|Main TLB lookup|0x80|Yes|Counts Main TLB lookups|
|Main TLB miss|0x81|Yes|Counts translation requests that miss in the Main TLB|
|Main TLB read|0x82|Yes|Counts once per access to the Main TLB RAMs, excluding reads that invalidation requests cause<br><br>Note: A transaction might access the Main TLB multiple times to look for diﬀerent page sizes.|
|Micro TLB lookup|0x83|Yes|Counts micro TLB lookups|
|Micro TLB miss|0x84|Yes|Counts translation requests that miss in the micro TLB|
|Slots full|0x85|No|Counts once per cycle when all slots are occupied and not ready to issue transactions downstream.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|Out of translation tokens|0x86|No|Counts once per cycle when a translation request cannot be issued because all translation tokens are in use.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|
|Write data buﬀer full|0x87|No|Counts once per cycle when a transaction is blocked because the write data buﬀer is full.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|


|Event|Event ID|SMMU_PMCG_SMR0 ﬁlterable|Description|
|---|---|---|---|
|Translation request|0x88|Yes|Counts translation requests, including both speculative and non-speculative requests|
|Write data uses write data buﬀer|0x89|Yes|Counts transactions with write data that is stored in the write data buﬀer|
|Write data bypasses write data buﬀer|0x8A|Yes|Counts transactions with write data that bypasses the write data buﬀer|
|MakeInvalid downgrade|0x8B|Yes|Counts when either:<br><br>• A MakeInvalid transaction on the TBS interface is output as CleanInvalid on the TBM interface<br>• A ReadOnceMakeInvalid transaction on the TBS interface is output as ReadOnceCleanInvalid on the TBM interface<br>|
|Stash fail|0x8C|Yes|Counts when either.<br><br>• A WriteUniquePtlStash or WriteUniqueFullStash transaction on TBS is output as a WriteNoSnoop or WriteUnique transaction on the TBM interface<br>• A StashOnceShared or StashOnceUnique transaction on the TBS interface has a valid translation, but is terminated in the TBU<br><br><br>Note:<br><br>A StashOnceShared or StashOnceUnique transaction that is terminated because of a StreamDisable or GlobalDisable translation response does not cause this event to count.|
|Main TLB error|0xC0|No|RAS corrected error in Main TLB.<br><br>This Secure event is visible only when the SMMU_PMCG_SCR.SO bit is set to 1.|


###### 2.6.4 SMMUv3 PMU register architectural options

The SMMUv3 architecture deﬁnes the Performance Monitor Counter Group (PMCG) conﬁguration register, SMMU_PMCG_CFGR. An MMU-600AE implementation assumes ﬁxed values for SMMU_PMCG_CFGR, and these values deﬁne behavioral aspects of the implementation.

The following table shows the SMMU_PMCG_CFGR register options that the MMU-600AE TCU and TBU use.

- Table 2-13: MMU-600AE SMMU_PMCG_CFGR register architectural options


|Field|Default value|Description for default value|
|---|---|---|
|SID_FILTER_TYPE|1|A single StreamID ﬁlter applies to all PMCG counters|
|CAPTURE|1|Capture of counter values into SVRn registers is supported|
|MSI|0|The counter group does not support Message Signaled Interrupts (MSIs)|
|RELOC_CTRS|1|The PMCG registers are relocated to page 1 of the PMU address map|
|SIZE|0x31|The counter group implements 32-bit counters|
|NCTR|0x3|The counter group includes 4 counters|


###### Related information

MMU-600AE memory map on page 65

###### 2.6.5 PMU snapshot interface

The Performance Monitoring Unit (PMU) snapshot interface is included on the TCU and on each TBU. You can use this asynchronous interface to initiate a PMU snapshot. A simultaneous snapshot of each counter register is created and copied to the respective SMMU_PMCG_SVRn register.

The PMU snapshot sequence is a 4-phase handshake. Both pmusnapshot_req and pmusnapshot_ack are LOW after reset. A snapshot occurs on the rising edge of pmusnapshot_req, and is equivalent to writing the value 1 to SMMU_PMCG_CAPR.CAPTURE.

The pmusnapshot_req signal is sampled using synchronizing registers. A register drives pmusnapshot_ack so that the connected component can sample the signal asynchronously.

###### Related information

Reliability, Availability, and Serviceability on page 48 TCU PMU snapshot interface signals for MMU-600AE on page 165 TBU PMU snapshot interface signals for MMU-600AE on page 175

##### 2.7 Reliability, Availability, and Serviceability

Reliability, Availability, and Serviceability (RAS) features enable cache corruption to be detected and corrected, optionally generating interrupts into the system. All MMU-600AE RAM-based caches support RAS error detection and correction.

The RAS Extension registers permit software to monitor the following caches for errors:

- • TBU Main TLB (MTLB)
- • TCU conﬁguration cache
- • TCU translation table walk cache


Within a coherent system, these caches are always clean, and there is no requirement to correct data on these caches. Any incorrect data is discarded and refetched. From an RAS standpoint, discarding and refetching counts as a corrected error.

For more information, see the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

###### Related information

TCU_ERRFR, TCU Error Feature register on page 81 TCU_ERRCTLR, TCU Error Control register on page 82 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register on page 83 TBU_ERRFR, TBU Error Feature register on page 91 TBU_ERRCTLR, TBU Error Control register on page 92

TBU_ERRSTATUS, TBU Error Record Primary Syndrome register on page 93

##### 2.8 Quality of Service

You can program the TCU with a priority level for each TBU interface. The priority level is applied to every translation from that TBU interface.

The TCU uses this priority level to:

- • Arbitrate between translations that are waiting in the translation request buﬀer when translation manager slots become available
- • Arbitrate between translation manager slots when they access the caches and perform conﬁguration table walks and translation table walks
- • Determine the AXI AxQOS value for translation table walks and conﬁguration table walks that the TCU issues on the QTW/DVM interface


The arbiters contain starvation avoidance mechanisms to prevent transactions from being stalled indeﬁnitely.

The TBU does not implement any prioritization between transactions. We recommend that bus requesters with diﬀerent Quality of Service (QoS) requirements use separate TBUs for translation. For information about the TCU QoS register, see 3.5.2 TCU_QOS, TCU Quality of Service register on page 73. See the TCU Node Control register information in 3.5.6 TCU_NODE_CTRLn, TCU Node Control registers on page 78 for information about translation priority control.

##### 2.9 Error responses

AMBA deﬁnes external AXI completer error, SLVERR, and external AXI decode error, DECERR, and TRANSFAULT. The MMU-600AE error response behavior depends on the interface.

The TCU ACE‑Lite interface treats SLVERR and DECERR identically, as an abort. When terminating a transaction, the TBS interface generates an OKAY, SLVERR, or TRANSFAULT response depending on the reason for the termination. If the TBU TBM interface receives a DECERR or SLVERR response to a downstream transaction, it propagates the same abort type to the TBS interface.

##### 2.10 Conversion between ACE-Lite and Armv8 attributes

The SMMUv3 architecture deﬁnes attributes in terms of the Arm®v8 architecture. The MMU-600AE components are therefore required to perform conversion between ACE‑Lite and Arm®v8 attributes.

The TBU must convert:

- • ACE‑Lite attributes to Arm®v8 attributes when it receives transactions on the Transaction completer (TBS) interface
- • Arm®v8 attributes to ACE‑Lite attributes when it outputs transactions on the Transaction requester (TBM) interface


The TCU must convert Arm®v8 attributes to ACE‑Lite attributes when it outputs transactions on the QTW/DVM interface.

For information about Arm®v8 attributes, see the Arm® Architecture Reference Manual Armv8, for Aproﬁle architecture.

###### 2.10.1 Attribute handling

Attribute handling in MMU-600AE depends on the context. When translation is enabled and a PCIe Root Complex issues transactions to a TBU, the following apply, depending on the type of transaction: Untranslated (non‑ATS) transaction

The SMMU applies attributes that a combination of the input attributes, STE overrides, and translation table descriptors determine.

Fully‑translated (full ATS) transaction

MMU-600AE generates ﬁxed attributes.

See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture versions 3.0 and version 3.2 for information about the preceding transactions and their attributes.

TBUs that are connected to a PCIe Root Complex must have the pcie_mode input signal tied HIGH, as the table in A.21 TBU tie-oﬀ signals for MMU-600AE on page 177 describes.

###### 2.10.1.1 Completer interface memory type attribute handling

The AxCACHE and AxDOMAIN signals contain the memory attributes that apply to the TBS interface.

The following table shows the ACE‑Lite to Armv8 attribute conversions that the TBU TBS interface performs.

- Table 2-14: MMU-600AE ACE-Lite to Armv8 memory attribute conversions


|AxCACHE attribute|AxDOMAIN attribute|Armv8 memory type|Armv8 Shareability|
|---|---|---|---|
|Device Non‑buﬀerable|System|Device‑nGnRnE|Outer Shareable|


|AxCACHE attribute|AxDOMAIN attribute|Armv8 memory type|Armv8 Shareability|
|---|---|---|---|
|Device Buﬀerable|System|Device‑nGnRE|Outer Shareable|
|Normal Non‑cacheable Buﬀerable<br><br>Normal Non‑cacheable Non‑buﬀerable<br><br>Write‑Through No Allocate<br><br>Write‑Through Read‑Allocate<br><br>Write‑Through Write‑Allocate<br><br>Write‑Through Read and Write‑Allocate|Any|Normal Inner Non‑cacheable Outer Non‑cacheable|Outer Shareable|
|Write‑Back No Allocate<br><br>Write‑Back Read‑Allocate<br><br>Write‑Back Write‑Allocate<br><br>Write‑Back Read‑Allocate Write‑Allocate|Non‑shareable<br><br>Inner Shareable<br><br>Outer Shareable|Normal Inner Write‑Back Outer Write‑Back|Non‑shareable<br><br>Non‑shareable<br><br>Outer Shareable|


- • Write-Back transactions are always treated as non-transient
- • The Armv8‑A Read‑Allocate and Write‑Allocate hints are the same as the hints that the AxCACHE Write‑Back type provides
- • The TBU TBS interface converts instruction writes into data writes, that is, it treats awprot_s[2] as 0


###### 2.10.1.2 Requester interface memory type attribute handling

The memory attributes that apply to the TBM and the QTW/DVM interfaces are contained in the AxCACHE and AxDOMAIN signals.

In addition, the TBU TBM interface can use the AxLOCK signal to indicate an Exclusive access. The QTW/DVM interface does not use the AxLOCK signal.

On the TBU TBM interface, a bit on AxUSER indicates whether the memory type before the conversion is Outer Cacheable.

The following table shows the Armv8 to ACE‑Lite attribute conversions that the requester interfaces perform.

###### Table 2-15: MMU-600AE Armv8 to ACE-Lite memory attribute conversions

|Armv8 memory attribute|AxCACHE attribute|AxDOMAIN attribute|AxLOCK attribute|AxUSER Outer Cacheable|
|---|---|---|---|---|
|Device-nGnRnE|Device Non‑buﬀerable|System|As Transaction completer (TBS) AxLOCK value|0|
|Device-GRE<br><br>Device-nGRE<br><br>Device-nGnRE|Device Buﬀerable|System|As TBS AxLOCK value|0|
|Normal Inner Non‑cacheable Outer Non‑cacheable<br><br>Normal Inner Write‑Through Outer Non‑cacheable<br><br>Normal Inner Write‑Back Outer Non‑cacheable|Normal Non‑cacheable Buﬀerable|System|As TBS AxLOCK value|0|
|Normal Inner Non‑cacheable Outer Write‑Through<br><br>Normal Inner Write‑Through Outer Write‑Through<br><br>Normal Inner Write‑Back Outer Write‑Through<br><br>Normal Inner Non‑cacheable Outer Write‑Back<br><br>Normal Inner Write‑Through Outer Write‑Back|Normal Non‑cacheable Buﬀerable|System|As TBS AxLOCK value|1|
|Normal Inner Write-Back Outer Write‑Back|Write‑Back No Allocate<br><br>Write‑Back Read‑Allocate<br><br>Write‑Back Write‑Allocate<br><br>Write‑Back Read and Write‑Allocate|If AxBURST == FIXED, Non‑shareable.<br><br>If AxBURST != FIXED, the attribute reﬂects the Armv8 Shareability:<br><br>• Non‑shareable<br>• Inner Shareable<br>• Outer Shareable<br>|0|1|


##### 2.11 Constraints and limitations of use

Certain usage constraints and limitations apply to the MMU-600AE. Unless otherwise speciﬁed, an IMPLEMENTATION DEFINED ﬁeld in a structure that the MMU-600AE:

- • Generates is 0


- • Reads is ignored


###### 2.11.1 SMMUv3 support

MMU-600AE does not implement, or require, certain SMMUv3 functionality. The SMMUv3 architectural registers include a set of ID registers that indicate the SMMUv3 features that the MMU-600AE implements. The following table shows the SMMUv3 ID register values that the MMU-600AE uses.

- Table 2-16: MMU-600AE SMMUv3 ID register architectural options


|Register|Field|Value|Conﬁgurable|Description for value|
|---|---|---|---|---|
|SMMU_IDR0|S2P|1|N|Stage 2 translations are supported.|
|SMMU_IDR0|S1P|1|N|Stage 1 translations are supported.|
|SMMU_IDR0|TTF|0b11|N|Both AArch32 Long-descriptor and AArch64 translation tables are supported.|
|SMMU_IDR0|COHACC|sup_cohacc|Y|Coherent access to translations, structure, and queues is supported.|
|SMMU_IDR0|BTM|sup_btm|Y|Broadcast TLB maintenance is supported.|
|SMMU_IDR0|HTTU[1:0]|0b00|N|Updates of the Dirty state and Access ﬂag are not supported.|
|SMMU_IDR0|DORMHINT|0|N|Dormant hint is not supported.|
|SMMU_IDR0|HYP|1|N|Hypervisor stage 1 context is supported.|
|SMMU_IDR0|ATS|1|N|PCIe Root Complex ATS is supported.|
|SMMU_IDR0|NS1ATS|1|N|Stage 1-only ATS is not supported.|
|SMMU_IDR0|ASID16|1|N|16-bit ASID is supported.|
|SMMU_IDR0|MSI|1|N|Message Signaled Interrupts (MSIs) are supported.|
|SMMU_IDR0|SEV|sup_sev|Y|SMMU and system support for the generation of events.|
|SMMU_IDR0|ATOS|0|N|Address translation operations are not supported.|
|SMMU_IDR0|PRI|1|N|PCIe Page Request Interface (PRI) is supported.|
|SMMU_IDR0|VMW|1|N|VMID wildcard-matching is supported for TLB invalidation.|
|SMMU_IDR0|VMID16|1|N|16-bit VMIDs are supported.|
|SMMU_IDR0|CD2L|1|N|2-level Context Descriptor (CD) tables are supported.|
|SMMU_IDR0|VATOS|0|N|Virtual ATOS page interface is not supported.|
|SMMU_IDR0|TTENDIAN|0b00|N|Mixed-endian translation walks are supported.|
|SMMU_IDR0|STALL_MODEL|{0, SMMU_S_CR0.NSSTALLD}|N|Stall model and Terminate model are both supported, unless the Secure world disables Non-secure stalling.|
|SMMU_IDR0|TERM_MODEL|0|N|Terminated transactions can terminate with either RAZ/ WI behavior or abort.|
|SMMU_IDR0|ST_LEVEL|0b01|N|2-level Stream tables are supported.|
|SMMU_IDR1|SIDSIZE|0b11000|N|24-bit StreamIDs are supported.|
|SMMU_IDR1|SSIDSIZE|0b10100|N|20-bit SubstreamIDs are supported.|
|SMMU_IDR1|PRIQS|0b10011|N|219 PRI queue entries are supported.|


|Register|Field|Value|Conﬁgurable|Description for value|
|---|---|---|---|---|
| |EVENTQS|0b10011|N|219 Event queue entries are supported.|
| |CMDQS|0b10011|N|219 Command queue entries are supported.|
| |ATTR_PERMS_OVR|1|N|Incoming permission attributes can be overridden.|
| |ATTR_TYPES_OVR|1|N|Incoming memory attributes can be overridden.|
| |REL|0|N|Base addresses are not ﬁxed.|
| |QUEUES_PRESET|0|N|The queue base addresses are not ﬁxed.|
| |TABLES_PRESET|0|N|The table base addresses are not ﬁxed.|
|SMMU_IDR2|BA_VATOS|0|N|No VATOS page is present.|
|SMMU_IDR3|HAD|1|N|Hierarchical Attribute Disable is supported.|
|SMMU_IDR3|PBHA|1|N|Page‑based hardware attributes are supported.|
|SMMU_IDR3|XNX|1|N|EL0/EL1 execute control distinction at stage 2 is supported for both AArch64 and AArch32 stage 2 translation tables.|
|SMMU_IDR3|PPS|1|N|If the request has a Process Address Space ID (PASID), the PASID is included in PRI queue overﬂow autogenerated responses. The STE.PPAR ﬁeld is not checked and is treated as 1.|
|SMMU_IDR4|IMPDEF|0|N|No IMPLEMENTATION DEFINED features apply.|
|SMMU_IDR5|OAS|sup_oas|Y|The size of the physical address that is output from the SMMU.|
|SMMU_IDR5|GRAN4K|1|N|4KB translation granule is supported.|
|SMMU_IDR5|GRAN16K|1|N|16KB translation granule is supported.|
|SMMU_IDR5|GRAN64K|1|N|64KB translation granule is supported.|
|SMMU_IDR5|VAX|0b00|N|Virtual addresses of 48 bits per CD.TTBx are supported.|
|SMMU_IDR5|STALL_MAX|TCUCFG_XLATE_SLOTS|N|Maximum number of outstanding stalled transactions that the SMMU supports.|
|SMMU_IIDR|Implementer|0x43B|N|Arm implementation.|
|SMMU_IIDR|Revision|MAX[0x0, ecorevnum]|Y (ecorevnum is conﬁgurable)|Minor revision is p0.<br><br>Note: ecorevnum is not conﬁgurable.|
|SMMU_IIDR|Variant|1|N|Product variant, or major revision is r1.|
|SMMU_IIDR|ProductID|0x485|N|Arm ID.|
|SMMU_AIDR|ArchMinorRev|0b0001|N|Architecture minor revision is SMMUv3.1.|
|SMMU_AIDR|ArchMajorRev|0b0000|N|Architecture major revision is SMMUv3.|
|SMMU_S_IDR0|MSI|1|N|Secure MSIs are supported.|
|SMMU_S_IDR0|STALL_MODEL|0b00|N|Stall model and Terminate model are both supported.|
|SMMU_S_IDR1|S_SIDSIZE|0b11000|N|24‑bit Secure StreamIDs are supported.|
|SMMU_S_IDR1|SECURE_IMPL|1|N|Security implemented.|
|SMMU_S_IDR3|SAMS|1|N|Secure Address Translation Services (ATS) maintenance is not implemented.|


In an MMU-600AE-based system, the SFM_ERR global error cannot occur, because Service Failure Mode (SFM) is not required.

MMU-600AE accepts but does not act on the following SMMUv3 Prefetch commands: CMD_PREFETCH_CONFIG

Prefetch conﬁguration. This command prefetches the required conﬁguration for a StreamID.

CMD_PREFETCH_ADDR Prefetch address. This command prefetches conﬁguration and TLB entries for an address range.

MMU-600AE does not generate any of the following SMMUv3 events, because they are not required:

###### F_UUT

Unsupported Upstream Transaction. F_TLB_CONFLICT

TLB conﬂict.

###### F_CFG_CONFLICT

Conﬁguration cache conﬂict. E_PAGE_REQUEST

Speculative page request hint. IMPDEF_EVENTn

IMPLEMENTATION DEFINED event allocation.

F_TLB_CONFLICT and F_CFG_CONFLICT are not required because the MMU-600AE caches include logic to ensure that only one entry can match at a time. If multiple cache entries match a transaction or translation request, only one entry is used and the others are ignored.

MMU-600AE never merges events. The STE.MEV ﬁeld is ignored. The TBU ignores the STE.ALLOCCFG ﬁeld that the TCU communicates to the TBU in the ALLOCCFG ﬁeld of the DTI_TBU_TRANS_RESP message. The TCU sup_oas[2:0] signal must not be set to 0b110. If this value is used, the TCU treats it as 0b101, that is, 48 bits. The TBU supports a 48‑bit PA size. The MMU-600AE TBU and TCU cannot be used with other components that implement DTI and are conﬁgured for a 52‑bit PA size.

###### Related information

SMMU architectural registers on page 61

###### 2.11.2 AMBA support

Certain behavior applies to how the MMU-600AE implements its ACE-Lite interfaces.

###### 2.11.2.1 TBU support for ACE-Lite transactions

The MMU-600AE TBU supports many ACE-Lite transaction types, and handles these transactions in certain ways. Typically, when propagating downstream transactions on the TBU TBM interface, the MMU-600AE uses the same transaction type that the upstream requester presents to the TBU TBS interface.

If the shareability domain of a downstream WriteLineUnique transaction is not Inner Shareable or Outer Shareable, the MMU-600AE outputs the transaction as WriteNoSnoop. That is, AWSNOOP

= 0b0000. The AWDOMAIN signal indicates the shareability domain of write transactions.

###### 2.11.2.2 Transactions that can result in a translation fault

In an MMU-600AE system, some transactions can result in a translation fault, and certain behavior is associated with such transactions.

MMU-600AE treats the following transactions as ordinary reads when calculating translation faults:

- • CleanShared.
- • CleanInvalid.
- • MakeInvalid.
- • CleanSharedPersist.
- • ReadOnceMakeInvalid.
- • ReadOnceCleanInvalid.

Therefore, these transactions might require either read permission or execute permission at the appropriate privilege level.

MMU-600AE treats the following transactions as ordinary writes when calculating translation faults:

- • WriteUniquePtlStash.
- • WriteUniqueFullStash. Therefore, these transactions require write permission at the appropriate privilege level.


CleanShared, CleanInvalid, MakeInvalid, and CleanSharedPersist transactions do not have a memory type. The input transaction and output transaction memory type and allocation hints are ignored and replaced by Normal, Inner Write-Back, Outer Write-Back, Read Allocate, Write Allocate. This behavior means that the ARDOMAIN output on the TBM interface is never System Shareable for these transactions, because they are never Non-cacheable or Device.

MMU-600AE treats transactions that pass the translation fault check as follows:

MakeInvalid transactions MMU-600AE converts MakeInvalid transactions to CleanInvalid transactions, unless the translation also grants write permission and Destructive Read Enable (DRE) permission.

ReadOnceMakeInvalid and ReadOnceCleanInvalid transactions MMU-600AE outputs ReadOnceMakeInvalid transactions as ReadOnceCleanInvalid transactions, unless the translation also granted write permission and DRE permission. If the ﬁnal transaction attributes on the TBU TBM interface are not Inner Shareable WriteBack or Outer Shareable Write-Back, MMU-600AE converts ReadOnceMakeInvalid and ReadOnceCleanInvalid transactions into ordinary reads.

WriteUniquePtlStash and WriteUniqueFullStash transactions If they pass the translation fault check, MMU-600AE converts WriteUniquePtlStash and WriteUniqueFullStash transactions to ordinary write transactions if either:

- • The translation did not grant Directed Cache Prefetch (DCP) permission.
- • The ﬁnal transaction attributes on the TBU TBM interface are not Inner Shareable or Outer Shareable Write-Back. If such a conversion occurs, AWSTASH* is driven as 0.


###### 2.11.2.3 Transactions that cannot result in a translation fault

In an MMU-600AE system, certain transactions cannot result in a translation fault, and certain behavior is associated with such transactions.

The following transactions never result in a translation fault:

- • StashOnceShared
- • StashOnceUnique
- • StashTranslation

If any of these transactions require a translation request to the TCU, MMU-600AE issues a Speculative translation request on the DTI interconnect. StashOnceShared and StashOnceUnique transactions are terminated in the TBU, with a BRESP value of OKAY, when any of the following cases apply:

- • The translation did not grant Directed Cache Prefetch (DCP) permission
- • The ﬁnal transaction attributes on the TBM interface are not Inner Shareable or Outer Shareable Write‑Back
- • The translation did not grant any of read, write, or execute permission at the appropriate privilege level


Only one of these permissions is required for the stash transaction to be permitted.

A BRESP value of OKAY indicates transaction success. MMU-600AE always generates this value when a StashOnceShared or a StashOnceUnique transaction is terminated in the TBU. This behavior applies even when a StreamDisable or GlobalDisable translation response causes the transaction to be terminated.

MMU-600AE never propagates StashTranslation transactions downstream, and uses StashTranslation only to prefetch Main TLB contents. MMU-600AE always terminates StashTranslation transactions with a BRESP value of OKAY, even if no translation could be stored in the Main TLB.

The TBU ignores AWPROT[0] and AWPROT[2] for StashTranslation transactions, because they do not aﬀect Speculative translation requests.

A StashTranslation transaction can be used to prefetch translations into the Main TLB of the MMU-600AE. However, for this prefetching to be useful, any subsequent transactions that intend to take advantage of the translations that have been prefetched into the Main TLB must use the same StreamID as the original prefetch. The StreamID identiﬁes a translation context. Using a diﬀerent StreamID for a subsequent transaction means that this subsequent transaction uses a diﬀerent translation context to the translation that has been prefetched into the Main TLB and might lead to a TLB miss.

###### 2.11.2.4 AXI5 support

The AXI5 protocol includes extensions that are not included in previous AXI versions. The Arm® AMBA® AXI and ACE Protocol Speciﬁcation, AXI3, AXI4, AXI5, ACE and ACE5 deﬁnes these extensions. The following table shows whether individual TCU and TBU interfaces support the AXI5 extensions.

- Table 2-17: TCU and TBU interface support for AXI5 extensions


|AXI5 extension|QTW/DVM|TBU TBS|TBU TBM|
|---|---|---|---|
|DVM_v8.1|Yes|-|-|
|Wakeup_Signals|Yes|Yes|Yes|
|Atomic_Transactions|-|Yes|Yes|
|Coherency_Connection_Signals|Yes|-|-|
|Cache_Stash_Transactions| |Yes|Yes|
|DeAllocation_Transactions|-|Yes|Yes|
|Untranslated_Transactions|-|Yes|Yes|
|Poison|-|-|-|
|Check_Type|-|-|-|
|QoS_Accept| | | |


|AXI5 extension|QTW/DVM|TBU TBS|TBU TBM|
|---|---|---|---|
|Trace_Signals|-|-|-|
|Loopback_Signals|-|-|-|
|NSAccess_Identiﬁers| | | |
|Persist_CMO|-|Yes|Yes|


## 3. Programmer's model

The programmer's model provides general information about the MMU-600AE register properties.

The information in this Programmer's model chapter applies to all the MMU-600AE registers. The complete set of MMU-600AE registers includes the main MMU-600 registers, that this Programmer's model chapter describes, and the MMU-600AE-speciﬁc Fault Detection and Control

(FDC) registers, that 4.4.9 Fault Detection and Control Programmer's View on page 119 describes:

- • The base address is not ﬁxed, and can be diﬀerent for any particular system implementation. The oﬀset of each register from the base address is ﬁxed.
- • Access type is described as follows:

RW Read and write. RO Read‑only. WO Write‑only. RAZ Read‑As‑Zero. WI Writes ignored.

- • Do not attempt to access reserved or unused address locations. Reading these locations results in RAZ and writing to these locations results in WI.
- • Unless otherwise stated in the accompanying text:

- ◦ Do not modify UNDEFINED register bits.
- ◦ Ignore UNDEFINED register bits on reads.
- ◦ All register bits are reset to 0 by a system or Cold reset.


- • Bit positions that are described as reserved are:


- ◦ In an RW register, RAZ/WI.
- ◦ In an RO register, RAZ.
- ◦ In a WO register, WI.


The main MMU-600 registers are accessed using the PROG APB4 completer interface on the TCU, and cannot be accessed directly through any other completer interfaces.

Some registers are 64 bits, but the PROG APB4 interface is 32 bits. Because software accesses 64‑bit registers 32 bits at a time, such accesses are not guaranteed to be 64‑bit atomic. This behavior does not cause problems for software, because the SMMUv3 architecture does not require 64‑bit atomic access to any registers.

The programmer's model contains separate TBU and TCU regions for internal control, RAS, and identiﬁcation registers. Accesses to unmapped or reserved registers are RAZ/WI. Non-secure accesses to Secure registers are RAZ/WI. MMU-600AE implements the identiﬁcation register scheme that the SMMUv3 architecture deﬁnes.

MMU-600AE implements all the Performance Monitor Counter Group (PMCG) registers that the SMMUv3 architecture deﬁnes, except for:

- • SMMU_PMCG_IRQ_CFG0
- • SMMU_PMCG_IRQ_CFG1
- • SMMU_PMCG_IRQ_CFG2
- • SMMU_PMCG_IRQ_STATUS

MMU-600AE does not implement the following SMMUv3 architectural registers, and accesses to these locations are RAZ/WI:

- • SMMU_IDR4
- • SMMU_STATUSR
- • SMMU_AGBPA
- • SMMU_GATOS_*
- • SMMU_S_IDR4
- • SMMU_S_AGBPA
- • SMMU_S_GATOS_*
- • SMMU_VATOS_*


For more information about the SMMU architectural registers, see the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

For information about the MMU-600AE Fault Detection and Control (FDC) registers, see 4.4.9 Fault Detection and Control Programmer's View on page 119.

##### 3.1 SMMU architectural registers

MMU-600AE implements many of the SMMU architectural registers, as deﬁned by the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

The following table lists the SMMUv3 architectural registers that the MMU-600AE implements.

All writable register ﬁelds reset to 0 unless the SMMU architecture speciﬁes otherwise.

- Table 3-1: SMMUv3 architectural registers


|Register|Name|Description|
|---|---|---|
|SMMU_S_IDR0 - SMMU_S_IDR3|SMMU Secure feature Identiﬁcation Registers|Provides information about the Secure features that the SMMU implementation supports.|
|SMMU_S_CR0|Secure global Control Register 0|Provides global conﬁguration of the Secure SMMU.|


|Register|Name|Description|
|---|---|---|
|SMMU_S_CR0ACK|Secure global Control Register 0 update Acknowledge|Provides acknowledgment of completion of updates to SMMU_S_CR0.|
|SMMU_S_CR1<br>SMMU_S_CR2<br>|Secure global Control Registers|Provides the controls for Secure table and queue access attributes.|
|SMMU_S_INIT|Secure Initialization control register|Provides a control to invalidate all Secure SMMU caching on system initialization.|
|SMMU_S_GBPA|Secure Global Bypass Attribute register|Controls the global bypass attributes that are used for transactions from Secure streams when the MMU is disabled.|
|SMMU_S_IRQ_CTRL|Secure Interrupt Control register|Contains enables for SMMU interrupts.|
|SMMU_S_IRQ_CTRLACK|Secure Interrupt Control register update Acknowledge|Provides acknowledgment of the completion of updates to SMMU_S_IRQ_CTRL.|
|SMMU_S_GERROR|Secure Global Error status register|Provides information on Secure global programming interface errors.|
|SMMU_S_GERRORN|Secure Global Error Acknowledgment register|Contains the acknowledgment ﬁelds for SMMU_S_GERROR errors.|
|SMMU_S_GERROR_IRQ_CFG0 SMMU_S_GERROR_IRQ_CFG2|Secure Global Error IRQ Conﬁguration register|Contains the Secure MSI address conﬁguration for the GERROR IRQ.|
|SMMU_S_STRTAB_BASE|Secure Stream Table Base address register|Contains the base address and attributes for the Secure Stream table.|
|SMMU_S_STRTAB_BASE_CFG|Secure Stream Table Base Conﬁguration register|Contains conﬁguration ﬁelds for the Secure Stream table.|
|SMMU_S_CMDQ_BASE|Secure Command queue Base address register|Contains the base address and attributes for the Secure Command queue.|
|SMMU_S_CMDQ_PROD|Secure Command queue Producer index register|Contains the Secure Command queue index for writes by the producer.|
|SMMU_S_CMDQ_CONS|Secure Command queue Consumer index register|Contains the Secure Command queue index for reads by the consumer.|
|SMMU_S_EVENTQ_BASE|Secure Event queue Base address register|Contains the base address and attributes for the Secure Event queue.|
|SMMU_S_EVENTQ_PROD|Secure Event queue Producer index register|Contains the Secure Event queue index for writes by the producer.|
|SMMU_S_EVENTQ_CONS|Secure Event queue Consumer index register|Contains the Secure Event queue index for reads by the consumer.|
|SMMU_S_EVENTQ_IRQ_CFG0 SMMU_S_EVENTQ_IRQ_CFG2|Secure Event queue IRQ Conﬁguration registers|Contains the MSI address conﬁguration for the Secure Event queue IRQ.|
|SMMU_IDR0 - SMMU_IDR3<br><br>SMMU_IDR5|SMMU feature Identiﬁcation Registers|Provides information about the features that the SMMU implementation supports.|
|SMMU_IIDR|Implementation Identiﬁcation Register|Provides implementer, part, and revision information for the SMMU implementation.|
|SMMU_AIDR|Architecture Identiﬁcation Register|Identiﬁes the SMMU architecture version to which the implementation conforms.|
|SMMU_CR0|Non‑secure global Control Register 0|Provides the controls for the global conﬁguration of the Non‑secure SMMU.|


|Register|Name|Description|
|---|---|---|
|SMMU_CR0ACK|Non‑secure global Control Register 0 update Acknowledge register|Provides acknowledgment of completion of updates to SMMU_CR0.|
|SMMU_CR1|Non‑secure global Control Register 1|Provides the controls for Non‑secure table and queue access attributes.|
|SMMU_CR2|Non‑secure global Control Register 2|Provides the controls for the conﬁguration of the global Non‑secure features.|
|SMMU_GBPA|Non‑secure Global Bypass Attribute register|Controls the global bypass attributes that are used for transactions from Non‑secure streams when the MMU is disabled.|
|SMMU_IRQ_CTRL|Non‑secure Interrupt Control register|Provides IRQ enable ﬂags for edge‑triggered wired outputs, if implemented, and MSI writes, if implemented.|
|SMMU_IRQ_CTRLACK|Non‑secure Interrupt Control register update Acknowledge register|Provides acknowledgment of the completion of updates to SMMU_IRQ_CTRL.|
|SMMU_GERROR|Non‑secure Global Error status register|Provides information about Non-secure global programming interface errors.|
|SMMU_GERRORN|Non‑secure Global Error acknowledgment register|Contains the acknowledgment ﬁelds for SMMU_GERROR errors.|
|SMMU_GERROR_IRQ_CFG0|Non‑secure Global Error IRQ Conﬁguration register 0|Contains the MSI address conﬁguration for the GERROR IRQ.|
|SMMU_GERROR_IRQ_CFG1|Non‑secure Global Error IRQ Conﬁguration register 1|Contains the MSI payload conﬁguration for the GERROR IRQ.|
|SMMU_GERROR_IRQ_CFG2|Non‑secure Global Error IRQ Conﬁguration register 2|Contains the MSI attribute conﬁguration for the GERROR IRQ.|
|SMMU_STRTAB_BASE|Non‑secure Stream Table Base address register|Contains the base address and attributes for the Non-secure Stream table.|
|SMMU_STRTAB_BASE_CFG|Non‑secure Stream Table Conﬁguration register|Contains conﬁguration ﬁelds for the Non‑secure Stream table.|
|SMMU_CMDQ_BASE|Non‑secure Command queue Base address register|Contains the base address and attributes for the Non‑secure Command queue.|
|SMMU_CMDQ_PROD|Non‑secure Command queue Producer index register|Contains the Non‑secure Command queue index for writes by the producer.|
|SMMU_CMDQ_CONS|Non‑secure Command queue Consumer index register|Contains the Non‑secure Command queue index for reads by the consumer.|
|SMMU_EVENTQ_BASE|Non‑secure Event queue Base address register|Contains the base address and attributes for the Non‑secure Event queue.|
|SMMU_EVENTQ_PROD|Non‑secure Event queue Producer index register|Contains the Non‑secure Event queue index for writes by the producer.|
|SMMU_EVENTQ_CONS|Non‑secure Event queue Consumer index register|Contains the Non‑secure Event queue index for reads by the consumer.|
|SMMU_EVENTQ_IRQ_CFG0|Non‑secure Event queue IRQ Conﬁguration register 0|Contains the MSI address conﬁguration for the Event queue IRQ.|
|SMMU_EVENTQ_IRQ_CFG1|Non‑secure Event queue IRQ Conﬁguration register 1|Contains the MSI payload conﬁguration for the Event queue IRQ.|
|SMMU_EVENTQ_IRQ_CFG2|Non‑secure Event queue IRQ Conﬁguration register 2|Contains the MSI attribute conﬁguration for the Event queue IRQ.|


|Register|Name|Description|
|---|---|---|
|SMMU_PRIQ_BASE|Non‑secure PRI queue Base address register|Contains the base address and attributes for the Non‑secure PRI queue.|
|SMMU_PRIQ_PROD|Non‑secure PRI queue Producer index register|Contains the Non‑secure PRI queue index for writes by the producer.|
|SMMU_PRIQ_CONS|Non‑secure PRI queue Consumer index register|Contains the Non‑secure PRI queue index for reads by the consumer.|
|SMMU_PRIQ_IRQ_CFG0|Non‑secure PRI queue IRQ Conﬁguration register 0|Contains the MSI address conﬁguration for the PRI queue IRQ.|
|SMMU_PRIQ_IRQ_CFG1|Non‑secure PRI queue IRQ Conﬁguration register 1|Contains the MSI payload conﬁguration for the PRI queue IRQ.|
|SMMU_PRIQ_IRQ_CFG2|Non‑secure PRI queue IRQ Conﬁguration register 2|Contains the MSI attribute conﬁguration for the PRI queue IRQ.|


###### MMU-600AE implements an SMMUv3 Performance Monitor Counter Group (PMCG) in the TCU and in each TBU. The following table lists the registers that MMU-600AE implements in each PMCG.

- Table 3-2: SMMUv3 PMCG registers


|Register|Name|Description|
|---|---|---|
|SMMU_PMCG_EVCNTR0 SMMU_PMCG_EVCNTR3|SMMU PMCG Event Counter registers|Contains the values of the event counters.|
|SMMU_PMCG_EVTYPER0 SMMU_PMCG_EVTYPER3|SMMU PMCG Event Type conﬁguration registers|Conﬁgures the events that the corresponding counter counts.|
|SMMU_PMCG_SVR0 SMMU_PMCG_SVR3|SMMU PMCG Shadow Value Registers|Contains the shadow value of the corresponding event counter.|
|SMMU_PMCG_SMR0|SMMU PMCG Stream Match ﬁlter Register|Conﬁgures the stream match ﬁlter for the corresponding event counter.|
|SMMU_PMCG_CNTENSET0|SMMU PMCG Counter Enable Set register|Provides the set mechanism for the counter enables.|
|SMMU_PMCG_CNTENCLR0|SMMU PMCG Counter Enable Clear register|Provides the clear mechanism for the counter enables.|
|SMMU_PMCG_INTENSET0|SMMU PMCG Interrupt contribution Enable Set register|Provides the set mechanism for the counter interrupt contribution enables.|
|SMMU_PMCG_INTENCLR0|SMMU PMCG Interrupt contribution Enable Clear register|Provides the clear mechanism for the counter interrupt enables.|
|SMMU_PMCG_OVSCLR0|SMMU PMCG Overﬂow Status Clear register|Provides the clear mechanism for the overﬂow status bits and provides read access to the overﬂow status bit values.|
|SMMU_PMCG_OVSSET0|SMMU PMCG Overﬂow Status Set register|Provides the set mechanism for the overﬂow status bits and provides read access to the overﬂow status bit values.|
|SMMU_PMCG_CAPR|SMMU PMCG Counter shadow value Capture Register|Controls the counter shadow value capture mechanism.|
|SMMU_PMCG_SCR|SMMU PMCG Secure Control Register|Secure Control Register.|
|SMMU_PMCG_CFGR|SMMU PMCG Conﬁguration information Register|Provides information about the PMCG implementation.|


|Register|Name|Description|
|---|---|---|
|SMMU_PMCG_CR|SMMU PMCG Control Register|Contains the Performance Monitor control ﬂags.|
|SMMU_PMCG_CEID0 SMMU_PMCG_CEID1|SMMU PMCG Common Event ID registers|Contains the lower and upper 64 bits of the Common Event identiﬁcation bitmap.|
|SMMU_PMCG_IRQ_CTRL|SMMU PMCG IRQ enable register|Contains the Performance Monitors IRQ enable.|
|SMMU_PMCG_IRQ_CTRLACK|SMMU PMCG IRQ enable Acknowledge register|Provides acknowledgment of the completion of updates to SMMU_PMCG_IRQ_CTRL.|
|SMMU_PMCG_AIDR|SMMU PMCG Architecture Identiﬁcation Register|Provides the Performance Monitor Architecture Identiﬁcation.|
|SMMU_PMCG_ID_REGS|ID registers|IMPLEMENTATION DEFINED.|
|SMMU_PMCG_PMAUTHSTATUS|PMU Authentication Status register|Performance Monitor authentication status.|
|SMMU_PMCG_PMDEVARCH|PMU Device Architecture register|Performance Monitor architecture identiﬁer.|
|SMMU_PMCG_PMDEVTYPE|PMU Device Type register|Performance Monitor device type.|


##### 3.2 MMU-600AE memory map

The MMU-600AE memory map that this section describes, in combination with the Fault Management Unit (FMU) memory map, contain all the MMU-600AE registers.

The MMU-600AE FMU has its own memory map which can have a diﬀerent base address from the main memory map. For more information about the FMU address map, see 4.4.9.1 FMU Programmer's View registers on page 121.

The following table shows the MMU-600AE memory map with the maximum number of implemented TBUs.

- Table 3-3: MMU-600AE memory map


|Address range|Description|
|---|---|
|0x000000 0x03FFFC|TCU registers.|
|0x040000 - 0x05FFFC<br><br>0x060000 - 0x07FFFC<br><br>0x080000 - 0x09FFFC<br><br>.<br><br>.<br><br>.<br><br>0x7C0000 - 0x7DFFFC<br><br>0x7E0000 - 0x7FFFFC|TBU0 registers.<br>TBU1 registers<br>TBU2 registers.<br><br><br>.<br><br>.<br><br>.<br><br>TBU60 registers.<br>TBU61 registers.<br>|


All TBU and TCU register addresses in this manual are described relative to the base address for that component.

The following table shows the MMU-600AE TCU memory map.

- Table 3-4: MMU-600AE TCU memory map

|Address|Description|
|---|---|
|0x00000 - 0x0FFFC|TCU registers, page 0, including:<br><br>• SMMUv3 registers, page 0.<br>• TCU Performance Monitor Counter Group (PMCG) registers, page 0, starting at oﬀset 0x02000.<br>• TCU microarchitectural registers.<br>|
|0x10000 - 0x1FFFC|TCU registers, page 1.<br><br>This address range contains the SMMUv3 registers, page 1.|
|0x20000 - 0x2FFFC|TCU registers, page 2.<br><br>This address range contains the TCU PMCG registers, page 1, starting at oﬀset 0x22000.|
|0x30000 - 0x3FFFC|Reserved.|


The following table shows the MMU-600AE TBU memory map.

- Table 3-5: MMU-600AE TBU memory map


|Address|Description|
|---|---|
|0x00000 - 0x0FFFC|TBU registers, page 0, including:<br><br>• TBU PMCG registers, page 0, starting at oﬀset 0x02000.<br>• TBU microarchitectural registers.<br>|
|0x10000 0x1FFFC|TBU registers, page 1.<br><br>This address range contains the TBU PMCG registers, page 1, starting at oﬀset 0x12000.|


For more information about the FMU address map, that in combination with the memory maps in this section, form the full MMU-600AE memory map, see 4.4.9.1 FMU Programmer's View registers on page 121.

##### 3.3 TCU and TBU PMU identiﬁcation registers

The MMU-600AE TCU and TBU use the same Performance Management Unit (PMU) identiﬁcation registers.

- Table 3-6: TCU and TBU PMU identiﬁcation register summary

|Oﬀset|Name|Type|Width|Description|
|---|---|---|---|---|
|0x02FB8|SMMU_PMCG_PMAUTHSTATUS|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FD0|SMMU_PMCG_PIDR4|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FD4|SMMU_PMCG_PIDR5|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FD8|SMMU_PMCG_PIDR6|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FDC|SMMU_PMCG_PIDR7|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FE0|SMMU_PMCG_PIDR0|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FE4|SMMU_PMCG_PIDR1|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FE8|SMMU_PMCG_PIDR2|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FEC|SMMU_PMCG_PIDR3|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FF0|SMMU_PMCG_CIDR0|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FF4|SMMU_PMCG_CIDR1|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FF8|SMMU_PMCG_CIDR2|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|
|0x02FFC|SMMU_PMCG_CIDR3|RO|32-bit|3.3.1 TCU PMU component and peripheral ID registers on page 67<br>3.3.2 TBU PMU component and peripheral ID registers on page 68<br>|


3.3.1 TCU PMU component and peripheral ID registers

The component and peripheral identity registers comply with the format that Arm CoreLink™ and CoreSight™ components use, and that the SMMUv3 architecture recommends. They provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

These registers are all read‑only. Each ﬁeld deﬁnes a single byte in the least signiﬁcant 8 bits, and the most signiﬁcant 24 bits are reserved. The least signiﬁcant 8 bits of the four Component ID registers form a single 32‑bit conceptual ID register. In a similar way, the deﬁned ﬁelds of the eight Peripheral ID registers form a conceptual 64‑bit ID register.

- Table 3-7: TCU PMU Component and Peripheral ID registers bit descriptions


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PMCG_PMAUTHSTATUS|0x02FB8|[7:0]|0x00|No authentication interface is implemented.|
|SMMU_PMCG_PIDR4|0x02FD0|[7:4]|0x0|4KB region count.|
|SMMU_PMCG_PIDR4|0x02FD0|[3:0]|0x4|JEP106 continuation code for Arm.|
|SMMU_PMCG_PIDR5|0x02FD4|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR6|0x02FD8|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR7|0x02FDC|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR0|0x02FE0|[7:0]|0x87|Part number[7:0].|


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PMCG_PIDR1|0x02FE4|[7:4]|0xB|JEP106 ID code[3:0] for Arm.|
|SMMU_PMCG_PIDR1|0x02FE4|[3:0]|0x4|Part number[11:8].|
|SMMU_PMCG_PIDR2|0x02FE8|[7:4]|0x1|MMU-600AE revision.<br><br>The value 0x1 indicates major product revision r1.|
|SMMU_PMCG_PIDR2|0x02FE8|[3]|0b1|The component uses a manufacturer identity code that JEDEC allocates, according to the JEP106 speciﬁcation.|
|SMMU_PMCG_PIDR2|0x02FE8|[2:0]|0b011|JEP106 ID code[6:4] for Arm.|
|SMMU_PMCG_PIDR3|0x02FEC|[7:4]|MAX[0x0,ecorevnum]|MMU-600AE minor revision.<br><br>The value 0x0 indicates minor product revision p0.|
|SMMU_PMCG_PIDR3|0x02FEC|[3:0]|0x0|CMOD. This ﬁeld is not used.|
|SMMU_PMCG_CIDR0|0x02FF0|[7:0]|0x0D|Preamble.|
|SMMU_PMCG_CIDR1|0x02FF4|[7:0]|0x90|Preamble.|
|SMMU_PMCG_CIDR2|0x02FF8|[7:0]|0x05|Preamble.|
|SMMU_PMCG_CIDR3|0x02FFC|[7:0]|0xB1|Preamble.|


###### 3.3.2 TBU PMU component and peripheral ID registers

The component and peripheral identity registers comply with the format that Arm CoreLink™ and CoreSight™ components use, and recommended in the SMMUv3 architecture. They provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

These registers are all read‑only. Each ﬁeld deﬁnes a single byte in the least signiﬁcant 8 bits, and the most signiﬁcant 24 bits are reserved. The least signiﬁcant 8 bits of the four Component ID registers form a single 32‑bit conceptual ID register. In a similar way, the deﬁned ﬁelds of the eight Peripheral ID registers form a conceptual 64‑bit ID register.

- Table 3-8: TBU PMU Component and Peripheral ID registers bit descriptions


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PMCG_PMAUTHSTATUS|0x02FB8|[7:0]|0x00|No authentication interface is implemented.|
|SMMU_PMCG_PIDR4|0x02FD0|[7:4]|0x0|4KB region count.|
|SMMU_PMCG_PIDR4|0x02FD0|[3:0]|0x4|JEP106 continuation code for Arm.|
|SMMU_PMCG_PIDR5|0x02FD4|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR6|0x02FD8|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR7|0x02FDC|[7:0]|0x00|Reserved.|
|SMMU_PMCG_PIDR0|0x02FE0|[7:0]|0x86|Part number[7:0].|
|SMMU_PMCG_PIDR1|0x02FE4|[7:4]|0xB|JEP106 ID code[3:0] for Arm.|
|SMMU_PMCG_PIDR1|0x02FE4|[3:0]|0x4|Part number[11:8].|


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PMCG_PIDR2|0x02FE8|[7:4]|0x1|MMU-600AE major revision.<br><br>The value 0x1 indicates major product revision r1.|
|SMMU_PMCG_PIDR2|0x02FE8|[3]|0b1|The component uses a manufacturer identity code that JEDEC allocates, according to the JEP106 speciﬁcation.|
|SMMU_PMCG_PIDR2|0x02FE8|[2:0]|0b011|JEP106 ID code[6:4] for Arm.|
|SMMU_PMCG_PIDR3|0x02FEC|[7:4]|MAX[0x0,ecorevnum]|MMU-600AE minor revision.<br><br>The value 0x0 indicates minor product revision p0.|
|SMMU_PMCG_PIDR3|0x02FEC|[3:0]|0x0|CMOD. This ﬁeld is not used.|
|SMMU_PMCG_CIDR0|0x02FF0|[7:0]|0x0D|Preamble.|
|SMMU_PMCG_CIDR1|0x02FF4|[7:0]|0x90|Preamble.|
|SMMU_PMCG_CIDR2|0x02FF8|[7:0]|0x05|Preamble.|
|SMMU_PMCG_CIDR3|0x02FFC|[7:0]|0xB1|Preamble.|


##### 3.4 TCU identiﬁcation registers

The TCU identiﬁcation registers provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

- Table 3-9: TCU identiﬁcation register summary


|Oﬀset|Name|Type|Width|Description|
|---|---|---|---|---|
|0x00FD0|SMMU_PIDR4|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FD4|SMMU_PIDR5|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FD8|SMMU_PIDR6|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FDC|SMMU_PIDR7|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FE0|SMMU_PIDR0|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FE4|SMMU_PIDR1|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FE8|SMMU_PIDR2|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FEC|SMMU_PIDR3|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FF0|SMMU_CIDR0|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FF4|SMMU_CIDR1|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FF8|SMMU_CIDR2|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|
|0x00FFC|SMMU_CIDR3|RO|32-bit|3.4.1 TCU component and peripheral ID registers on page 69|


###### 3.4.1 TCU component and peripheral ID registers

The component and peripheral identity registers comply with the format that the Arm CoreLink™ and CoreSight™ components use, and recommended in the SMMUv3 architecture. They provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

These registers are all read‑only. Each ﬁeld deﬁnes a single byte in the least signiﬁcant 8 bits, and the most signiﬁcant 24 bits are reserved. The least signiﬁcant 8 bits of the four Component ID registers form a single 32‑bit conceptual ID register. In a similar way, the deﬁned ﬁelds of the eight Peripheral ID registers form a conceptual 64‑bit ID register.

- Table 3-10: TCU Component and Peripheral ID registers bit descriptions


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PIDR4|0x00FD0|[7:4]|0x0|4KB region count.|
|SMMU_PIDR4|0x00FD0|[3:0]|0x4|JEP106 continuation code for Arm.|
|SMMU_PIDR5|0x00FD4|[7:0]|0x00|Reserved.|
|SMMU_PIDR6|0x00FD8|[7:0]|0x00|Reserved.|
|SMMU_PIDR7|0x00FDC|[7:0]|0x00|Reserved.|
|SMMU_PIDR0|0x00FE0|[7:0]|0x87|Part number[7:0].|
|SMMU_PIDR1|0x00FE4|[7:4]|0xB|JEP106 ID code[3:0] for Arm.|
|SMMU_PIDR1|0x00FE4|[3:0]|0x4|Part number[11:8].|
|SMMU_PIDR2|0x00FE8|[7:4]|0x1|MMU-600AE major revision.<br><br>The value 0x1 indicates major product revision r1.|
|SMMU_PIDR2|0x00FE8|[3]|0b1|The component uses a manufacturer identity code that JEDEC allocates, according to the JEP106 speciﬁcation.|
|SMMU_PIDR2|0x00FE8|[2:0]|0b011|JEP106 ID code[6:4] for Arm.|
|SMMU_PIDR3|0x00FEC|[7:4]|MAX[0x0,ecorevnum]|MMU-600AE minor revision.<br><br>The value 0x0 indicates minor product revision p0.|
|SMMU_PIDR3|0x00FEC|[3:0]|0x0|CMOD. This ﬁeld is not used.|
|SMMU_CIDR0|0x00FF0|[7:0]|0x0D|Preamble.|
|SMMU_CIDR1|0x00FF4|[7:0]|0xF0|Preamble.|
|SMMU_CIDR2|0x00FF8|[7:0]|0x05|Preamble.|
|SMMU_CIDR3|0x00FFC|[7:0]|0xB1|Preamble.|


##### 3.5 TCU microarchitectural registers

You can set the MMU-600AE TCU microarchitectural registers at boot time to optimize TCU behavior for your system. We recommend that you use the default values for most systems.

- Table 3-11: TCU microarchitectural register summary


|Oﬀset|Name|Type|Reset|Width|Description|
|---|---|---|---|---|---|
|0x08E00|TCU_CTRL|RW|0x00000000|32-bit|3.5.1 TCU_CTRL, TCU Control register on page 71|
|0x08E04|TCU_QOS|RW|0x00000000|32-bit|3.5.2 TCU_QOS, TCU Quality of Service register on page 73|
|0x08E08|TCU_CFG|RO|See register bit assignments.|32-bit|3.5.3 TCU_CFG, TCU Conﬁguration Information register on page 75|
|0x08E10|TCU_STATUS|RO|0x00000000|32-bit|3.5.4 TCU_STATUS, TCU Status Information register on page 76|
|0x08E18|TCU_SCR|RW|See register bit assignments.|32-bit|3.5.5 TCU_SCR, TCU Secure Control register on page 76|
|0x09000 0x093FC|TCU_NODE_CTRLn|RW|0x00000000|32-bit|3.5.6 TCU_NODE_CTRLn, TCU Node Control registers on page 78|
|0x09400 0x097FC|TCU_NODE_STATUSn|RO|0x00000000|32-bit|3.5.7 TCU_NODE_STATUSn, TCU Node Status registers on page 79|


###### 3.5.1 TCU_CTRL, TCU Control register

The TCU Control register disables MMU-600AE TCU features. If the hit rate of the individual walk cache is too low, you can disable individual walk caches to improve performance in some systems. Do not modify the AUX bits unless we direct you to do so.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x08E00 Type

RW

Reset value

0x00000000

###### Usage constraints

When TCU_SCR.NS_UARCH = 0, Non‑secure accesses to this register are RAZ/WI. Writes to this register are possible only when both SMMU_CR0.SMMUEN = 0 and SMMU_S_CR0.SMMUEN = 0. Writes at other times are ignored. After modifying this register, software must issue an INV_ALL operation using the SMMU_S_INIT register, before it sets SMMUEN to 1. Failure to issue an INV_ALL operation results in UNPREDICTABLE behavior.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-1: TCU_CTRL register bit assignments

|31| |20|19 18 16|19 18 16|15 14 13 12|15 14 13 12|15 14 13 12|15 14 13 12|11 10 9 8|11 10 9 8|11 10 9 8|11 10 9 8|7|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|AUX[31:20]|AUX[31:20]|AUX[31:20]| | | | | | | | | | |AUX[7:0]|AUX[7:0]|


DONT_HASH_ASID AUX[18:16]

WCS1L0_DIS WCS1L1_DIS

- WCS2L2_DIS

- WCS2L3_DIS


- WCS1L2_DIS

- WCS1L3_DIS

- WCS2L0_DIS


- WCS2L1_DIS


###### Table 3-12: TCU_CTRL register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:20]|AUX[31:20]|Leave each of these bits as 0.|
|[19]|ASID_VMID_HASH|0 Only the input address is used for walk cache indexing. Transactions with the same input address use the same walk cache index. Walk cache utilization is poor if such transactions are common.<br>1 ASID, VMID, and input address are used for walk cache indexing. Transactions with the same VMID and input address, but diﬀerent ASID, use diﬀerent walk cache indexes. This improves walk cache utilization if diﬀerent ASIDs are used for the same input address and VMID, but invalidation performance is worse for invalidations that do not provide an ASID because the whole cache must be walked instead of invalidating based on a speciﬁc index.<br>|
|[18:16]|AUX[18:16]|Leave each of these bits as 0.|
|[15]|WCS2L3_DIS|Walk cache disable:<br><br>0 Stage 2 level 3 walk cache is enabled.<br>1 Stage 2 level 3 walk cache is disabled.<br>|
|[14]|WCS2L2_DIS|Walk cache disable:<br><br>0 Stage 2 level 2 walk cache is enabled.<br>1 Stage 2 level 2 walk cache is disabled.<br>|


|Bits|Name|Description|
|---|---|---|
|[13]|WCS2L1_DIS|Walk cache disable:<br><br>0 Stage 2 level 1 walk cache is enabled.<br>1 Stage 2 level 1 walk cache is disabled.<br>|
|[12]|WCS2L0_DIS|Walk cache disable:<br><br>0 Stage 2 level 0 walk cache is enabled.<br>1 Stage 2 level 0 walk cache is disabled.<br>|
|[11]|WCS1L3_DIS|Walk cache disable:<br><br>0 Stage 1 level 3 walk cache is enabled.<br>1 Stage 1 level 3 walk cache is disabled.<br>|
|[10]|WCS1L2_DIS|Walk cache disable:<br><br>0 Stage 1 level 2 walk cache is enabled.<br>1 Stage 1 level 2 walk cache is disabled.<br>|
|[9]|WCS1L1_DIS|Walk cache disable:<br><br>0 Stage 1 level 1 walk cache is enabled.<br>1 Stage 1 level 1 walk cache is disabled.<br>|
|[8]|WCS1L0_DIS|Walk cache disable:<br><br>0 Stage 1 level 0 walk cache is enabled.<br>1 Stage 1 level 0 walk cache is disabled.<br>|
|[7:0]|AUX[7:0]|Leave each of these bits as 0.|


###### 3.5.2 TCU_QOS, TCU Quality of Service register

The TCU Quality of Service (QoS) register speciﬁes AxQOS values for each transaction type that is issued on the QTW/DVM interface. MMU-600AE does not use this value internally, but a downstream interconnect can use the value to control how it prioritizes transactions.

The AxQOS value that is associated with each transaction does not take account of other transactions that are blocked behind it. For example, although higher priority translations are normally progressed before lower priority translations, a low‑priority translation table walk might prevent the TCU from issuing a translation table walk with a higher priority.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

###### Width

32

###### Functional group

See Table 3-11: TCU microarchitectural register summary on page 71.

Address oﬀset

0x08E04 Type

RW

Reset value

0x00000000

###### Usage constraints

When TCU_SCR.NS_UARCH = 0, Non-secure accesses to this register are RAZ/WI. Writes to this register are possible only when both SMMU_CR0.SMMUEN = 0 and SMMU_S_CR0.SMMUEN = 0. Writes at other times are ignored. After modifying this register, software must issue an INV_ALL operation using the SMMU_S_INIT register, before it sets SMMUEN to 1. Failure to issue an INV_ALL operation results in UNPREDICTABLE behavior.

###### Bit descriptions

###### The following ﬁgure shows the bit assignments. Figure 3-2: TCU_QOS register bit assignments

|31 28|27 24|23 20|19 16|15 12|11 8|7 4|3 0|
|---|---|---|---|---|---|---|---|
|Reserved| | | | | | | |


QOS_DVMSYNC

QOS_MSI

- QOS_PTW0

QOS_QUEUE

- QOS_PTW1
- QOS_PTW2
- QOS_PTW3


###### Table 3-13: TCU_QOS register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:28]| |Reserved.|
|[27:24]|QOS_DVMSYNC|The AxQOS value that is used for DVM Sync Completion messages.|
|[23:20]|QOS_MSI|The AxQOS value that is used for MSIs.|
|[19:16]|QOS_QUEUE|The AxQOS value that is used for queue accesses.|
|[15:12]|QOS_PTW3|The AxQOS value that is used for translation table walks for translations where TCU_NODE_CTRLn.PRIORITY = 3 for the requesting node.|
|[11:8]|QOS_PTW2|The AxQOS value that is used for translation table walks for translations where TCU_NODE_CTRLn.PRIORITY = 2 for the requesting node.|
|[7:4]|QOS_PTW1|The AxQOS value that is used for translation table walks for translations where TCU_NODE_CTRLn.PRIORITY = 1 for the requesting node.|


|Bits|Name|Description|
|---|---|---|
|[3:0]|QOS_PTW0|The AxQOS value that is used for translation table walks for translations where TCU_NODE_CTRLn.PRIORITY = 0 for the requesting node.|


###### 3.5.3 TCU_CFG, TCU Conﬁguration Information register

Use the TCU Conﬁguration Information register to set the number of shared translation slots in the MMU-600AE TCU.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x08E08 Type

RO

Reset value

See register bit assignments.

###### Usage constraints

When TCU_SCR.NS_UARCH = 0, Non‑secure accesses to this register are RAZ.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-3: TCU_CFG register bit assignments

|31| | |16|15| |4|3 0|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|XLATE_SLOTS|XLATE_SLOTS|XLATE_SLOTS|Reserved|


- Table 3-14: TCU_CFG register bit descriptions


|Bits|Name|Description|
|---|---|---|
|[31:16]|-|Reserved.|
|[15:4]|XLATE_SLOTS|The number of translation slots that are available for sharing between all nodes.<br><br>The reset value of this ﬁeld is TCUCFG_XLATE_SLOTS.|
|[3:0]|-|Reserved.|


###### 3.5.4 TCU_STATUS, TCU Status Information register

The TCU Status Information register counts the number of allocated translation slots in the MMU-600AE TCU.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x08E10 Type

RO

Reset value

0x00000000

###### Usage constraints

When TCU_SCR.NS_UARCH = 0, Non‑secure accesses to this register are RAZ.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-4: TCU_STATUS register bit assignments

|31| | |16|15| |4|3 0|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|GNT_XLATE_SLOTS|GNT_XLATE_SLOTS|GNT_XLATE_SLOTS|Reserved|


- Table 3-15: TCU_STATUS register bit descriptions


|Bits|Name|Description|
|---|---|---|
|[31:16]|-|Reserved.|
|[15:4]|GNT_XLATE_SLOTS|GNT_XLATE_SLOTS is the number of translation slots that are currently allocated to connected nodes. You can use this value for debugging purposes.|
|[3:0]|-|Reserved.|


###### 3.5.5 TCU_SCR, TCU Secure Control register

The TCU Secure Control register in the MMU-600AE TCU controls whether Non‑secure software is permitted to access each TCU register group.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x08E18 Type

RW

Reset value

See register bit assignments.

###### Usage constraints

Non‑secure accesses to this register are RAZ/WI. This register does not control Secure access to the MMU-600AE PMU registers. To control Secure PMU register access, use the SMMU_PMCG_SCR register.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-5: TCU_SCR register bit assignments

|31| | | | | |4|3 2 1 0|3 2 1 0|3 2 1 0|3 2 1 0|
|---|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | | | |


NS_INIT

Reserved

NS_RAS

NS_UARCH

###### Table 3-16: Register name bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:4]|-|Reserved.|


|Bits|Name|Description|
|---|---|---|
|[3]|NS_INIT|Non‑secure register access to SMMU_S_INIT. When this bit is set to 0, Non‑secure accesses to the SMMU_S_INIT register are RAZ/WI.<br><br>The sec_override input signal deﬁnes the reset value of this bit.|
|[2]| |Reserved.|
|[1]|NS_RAS|Non‑secure register access is permitted for RAS registers. When this bit is set to 0, Non‑secure accesses to register addresses 0x08E80–0x08EC0 are RAZ/WI.<br><br>The sec_override input signal deﬁnes the reset value of this bit.|
|[0]|NS_UARCH|Non‑secure register access is permitted for MMU-600AE registers. When this bit is set to 0, Non‑secure accesses to register addresses 0x08E00–0x08E7C and 0x09000–0x093FC are RAZ/WI.<br><br>The sec_override input signal deﬁnes the reset value of this bit.<br><br>If your implementation might use Secure translation, Arm recommends setting this bit to 0.|


###### 3.5.6 TCU_NODE_CTRLn, TCU Node Control registers

Each TCU Node Control register controls how the MMU-600AE TCU communicates with a single node. A node is a DTI requester that is typically either a TBU or a PCIe Root Complex that implements ATS.

###### Conﬁgurations

The value of the TCUCFG_NUM_TBU conﬁguration parameter deﬁnes n, that is, the number of TCU_NODE_CTRL registers that are implemented. Each register has an address width of 4 bytes, therefore the oﬀset of a register n is:

0x09000 + (4 × n)

###### Attributes

###### Width

32

###### Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x09000‑0x093FC Type

RW

Reset value

0x00000000

###### Usage constraints

The DIS_DVM bit can be used for TBU nodes, but is ignored for ATS nodes.

When TCU_SCR.NS_UARCH = 0, Non‑secure accesses to this register are RAZ/WI. Writes to this register are possible only when both SMMU_CR0.SMMUEN = 0 and SMMU_S_CR0.SMMUEN = 0. Writes at other times are ignored. After modifying this register, software must issue an INV_ALL operation using the SMMU_S_INIT register, before it sets SMMUEN to 1. Failure to issue an INV_ALL operation results in UNPREDICTABLE behavior.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-6: TCU_NODE_CTRL register bit assignments

|31| | | | | |5 4|5 4|3 2 1 0|3 2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | | |


DIS_DVM

Reserved

PRI_LEVEL

###### Table 3-17: TCU_NODE_CTRL register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:5]| |Reserved.|
|[4]|DIS_DVM|Disable DVM. When this bit is set to 1, the corresponding node does not participate in DVM invalidation.<br><br>Software should set this bit to 1 if all the following are true:<br><br>• The node is slow to respond to invalidations that are issued over DTI<br>• Software has knowledge that the node does not require to be part of the DVM domain<br>• Software has knowledge that invalidations for the node can be issued using the Command queue<br><br><br>Note: This bit is ignored for connected DTI‑ATS requesters, because they never participate in DVM invalidation.|
|[3:2]|-|Reserved.|
|[1:0]|PRI_LEVEL|Priority level. This ﬁeld indicates the priority level of the corresponding node. Translation requests from a node with a higher priority level are normally progressed before requests from a node with a lower priority level.|


###### 3.5.7 TCU_NODE_STATUSn, TCU Node Status registers

Each TCU Node Status register provides the status of a DTI requester connected to the MMU-600AE TCU. A node is a DTI requester that is typically either a TBU or a PCIe Root Complex that implements ATS.

###### Conﬁgurations

The value of the TCUCFG_NUM_TBU conﬁguration parameter deﬁnes the number of TCU_NODE_CTRL registers that are implemented. Each register has an address width of 4 bytes, therefore the oﬀset of a register n is:

0x09400 + (4 × n)

###### Attributes

Width

32

Functional group

See Table 3-11: TCU microarchitectural register summary on page 71. Address oﬀset

0x09400‑0x097FC Type

RO

Reset value

0x00000000

###### Usage constraints

This register indicates the status of the corresponding node only when the node is connected. When TCU_SCR.NS_UARCH = 0, Non-secure accesses to this register are RAZ.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-7: TCU_NODE_STATUS register bit assignments

|31| | | | | | |2 1 0|2 1 0|2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | |


ATS

CONNECTED

###### Table 3-18: TCU_NODE_STATUS register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:2]|-|Reserved.|


|Bits|Name|Description|
|---|---|---|
|[1]|ATS|ATS implemented:<br><br>0 The corresponding node is a TBU that is connected to the TCU using the DTI‑TBU protocol.<br>1 The corresponding node is a PCIe Root Complex that supports ATS, and is connected to the TCU using the DTI‑ATS protocol.<br>|
|[0]|CONNECTED|DTI link is connected:<br><br>0 The DTI link for the corresponding node is not connected.<br>1 The DTI link for the corresponding node is connected.<br><br><br>If a DTI link is not connected, accesses to TBU registers are RAZ/WI. However, the state might change between reading this register and attempting to access the TBU.|


##### 3.6 TCU RAS registers

MMU-600AE includes TCU registers that are related to Reliability, Availability, and Serviceability (RAS).

- Table 3-19: TCU RAS register summary


|Oﬀset|Name|Type|Reset|Width|Description|
|---|---|---|---|---|---|
|0x08E80|TCU_ERRFR|RO|0x00000081|32-bit|3.6.1 TCU_ERRFR, TCU Error Feature register on page 81|
|0x08E88|TCU_ERRCTLR|RW|0x00000008|32-bit|3.6.2 TCU_ERRCTLR, TCU Error Control register on page 82|
|0x08E90|TCU_ERRSTATUS|RW|0x00000000|32-bit|3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register on page 83|
|0x08EC0|TCU_ERRGEN|RW|0x00000000|32-bit|3.6.4 TCU_ERRGEN, TCU Error Generation register on page 85|


###### 3.6.1 TCU_ERRFR, TCU Error Feature register

Use the TCU Error Feature register to discover how the MMU-600AE TCU handles errors.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-19: TCU RAS register summary on page 81. Address oﬀset

0x08E80 Type

RO

0x00000081

###### Usage constraints

This register is read-only. When TCU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-8: TCU_ERRFR register bit assignments

|31| | | | |8|7 6 5|7 6 5|2 1 0|2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved<br><br>|Reserved<br><br>|Reserved<br><br>|Reserved<br><br>|Reserved<br><br>|Reserved<br><br>|FI|Reserved|Reserved|ED|


###### Table 3-20: TCU_ERRFR register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:8]| |Reserved|
|[7:6]|FI|The value 0b10 indicates that the fault handling interrupt is controllable|
|[5:2]|-|Reserved|
|[1:0]|ED|The value 0b01 indicates that TCU error detection is always enabled|


###### 3.6.2 TCU_ERRCTLR, TCU Error Control register

Use the TCU Error Control register to enable fault handling interrupts. MMU-600AE must be connected to an interrupt controller when Reliability, Availability, and Serviceability (RAS) interrupts are enabled for fault handling.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-19: TCU RAS register summary on page 81. Address oﬀset

0x08E88 Type

RW

0x00000008

###### Usage constraints

When TCU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-9: TCU_ERRCTLR register bit assignments

|31| | | | | |4|3 2 0|3 2 0|
|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | |


FI Reserved

###### Table 3-21: TCU_ERRCTLR register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:4]| |Reserved.|
|[3]|FI|Enable fault handling interrupts:<br><br>0 No interrupt is generated when a fault occurs.<br>1 An interrupt is generated on ras_irpt when a fault occurs.<br>|
|[2:0]|-|Reserved.|


###### 3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register

Use the TCU Error Record Primary Syndrome register to ﬁnd out whether diﬀerent types of error have occurred on the MMU-600AE TCU.

###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-19: TCU RAS register summary on page 81. Address oﬀset

0x08E90 Type

RW

0x00000000

###### Usage constraints

When TCU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI. To prevent race conditions, under certain circumstances, writes to some bits in this register are ignored. Typically, these writes are ignored when software has not yet observed a new error.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-10: TCU_ERRSTATUS register bit assignments

|31 30 29 28|31 30 29 28|31 30 29 28|27 26 25 24|27 26 25 24|27 26 25 24|23|16|15|8|7|0|
|---|---|---|---|---|---|---|---|---|---|---|---|
| |V| | | |CE|Reserved|Reserved|IERR|IERR|SERR|SERR|


Reserved

OF Reserved Reserved

###### Table 3-22: Register name bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31]| |Reserved.|
|[30]|V|Register valid. This bit is set to 1 to indicate that at least one RAS error was recorded.<br><br>Clear this bit by writing a 1 to it. If CE is not 0b00 and is not being cleared, the write is ignored. A write of 0 is ignored.|
|[29:28]| |Reserved.|
|[27]|OF|Overﬂow. This bit is set to 1 to indicate that multiple correctable errors were recorded. That is, at least one correctable error was recorded when CE != 0b00.<br><br>Clear this bit by writing a 1 to it. A write of 0 is ignored.|
|[26]|-|Reserved.|
|[25:24]|CE|Correctable Error. This ﬁeld is set to 0b10 to indicate that a corrected error occurred. Clear this ﬁeld by writing 0b11 to it. If OF is set to 1 and is not being cleared, the write is ignored. A write of any value other than 0b11 is ignored.|
|[23:16]| |Reserved.|


|Bits|Name|Description|
|---|---|---|
|[15:8]|IERR|IMPLEMENTATION DEFINED error code. When SERR is not set to 0, this ﬁeld indicates the source of the error, as follows:<br><br>0x00 Stage 1, level 0 walk cache.<br>0x01 Stage 1, level 1 walk cache.<br>0x02 Stage 1, level 2 walk cache.<br>0x03 Stage 1, level 3 walk cache.<br>0x04 Stage 2, level 0 walk cache.<br>0x05 Stage 2, level 1 walk cache.<br>0x06 Stage 2, level 2 walk cache.<br>0x07 Stage 2, level 3 walk cache.<br>0x08 Conﬁguration cache.<br><br><br>Writes to this ﬁeld are ignored.|
|[7:0]|SERR|Error code. This read‑only ﬁeld provides information about the earliest unacknowledged correctable error, as follows:<br><br>0x00 No error. This code occurs when CE = 0b00.<br><br>0x07 Tag corrupted. This code can occur when CE != 0b00.<br>0x08 Data corrupted. This code can occur when CE != 0b00.<br>|


###### 3.6.4 TCU_ERRGEN, TCU Error Generation register

Use the MMU-600AE TCU Error Generation Register to generate tag parity errors, for example when testing error‑handling software.

The errors that are injected using this mechanism are correctable errors but are not reported in the FMU error records. See:

- • 4.4.4 Safety Mechanism table on page 109.
- • 4.4.4.2 Injecting an error into a Safety Mechanism on page 112.


###### Conﬁgurations

This register exists in all TCU conﬁgurations.

###### Attributes

###### Width

32

###### Functional group

See Table 3-19: TCU RAS register summary on page 81. Address oﬀset

0x08EC0 Type

RW

Reset value

0x00000000

###### Usage constraints

When TCU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-11: TCU_ERRGEN register bit assignments

|31| | | | | |4|3 2 1 0|3 2 1 0|3 2 1 0|3 2 1 0|
|---|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | | | |


TCC

DCC

TWC

DWC

###### Table 3-23: Register name bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:4]| |Reserved.|
|[3]|TCC|Conﬁguration cache tag parity error:<br><br>0 No tag parity error is written to the conﬁguration cache.<br>1 Entries that are written to the conﬁguration cache include a tag parity error. A fault occurs when the entry is used.<br>|
|[2]|DCC|Conﬁguration cache data parity error:<br><br>0 No data parity error is written to the conﬁguration cache.<br>1 Entries that are written to the conﬁguration cache include a data parity error. A fault occurs when the entry is used.<br><br><br>Note: Tag parity errors mask data parity errors. When testing data parity error functionality, ensure that software does not set this bit and the TCC bit at the same time.|
|[1]|TWC|Walk cache tag parity error:<br><br>0 No tag parity error is written to the walk cache.<br>1 Entries that are written to the walk cache include a tag parity error. A fault occurs when the entry is used.<br>|
|[0]|DWC|Walk cache data parity error:<br><br>0 No data parity error is written to the walk cache.<br>1 Entries that are written to the walk cache include a data parity error. A fault occurs when the entry is used.<br><br><br>Note: Tag parity errors mask data parity errors. When testing data parity error functionality, ensure that software does not set this bit and the TWC bit at the same time.|


##### 3.7 TBU identiﬁcation registers

The TBU identiﬁcation registers provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

- Table 3-24: TBU identiﬁcation register summary

|Oﬀset|Name|Type|Width|Description|
|---|---|---|---|---|
|0x00FD0|SMMU_PIDR4|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FD4|SMMU_PIDR5|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FD8|SMMU_PIDR6|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FDC|SMMU_PIDR7|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FE0|SMMU_PIDR0|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FE4|SMMU_PIDR1|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FE8|SMMU_PIDR2|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FEC|SMMU_PIDR3|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FF0|SMMU_CIDR0|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FF4|SMMU_CIDR1|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FF8|SMMU_CIDR2|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|
|0x00FFC|SMMU_CIDR3|RO|32-bit|3.7.1 TBU component and peripheral ID registers on page 87|


3.7.1 TBU component and peripheral ID registers

The component and peripheral identity registers comply with the format that Arm CoreLink™ and CoreSight™ components use, and that the SMMUv3 architecture recommends. They provide key information about the MMU-600AE hardware, including the product and associated revision number. They also identify Arm as the designer of the SMMU.

These registers are all read‑only. Each ﬁeld deﬁnes a single byte in the least signiﬁcant 8 bits, and the most signiﬁcant 24 bits are reserved. The least signiﬁcant 8 bits of the four Component ID registers form a single 32‑bit conceptual ID register. In a similar way, the deﬁned ﬁelds of the eight Peripheral ID registers form a conceptual 64‑bit ID register.

- Table 3-25: TBU Component and Peripheral ID registers bit descriptions


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
|SMMU_PIDR4|0x00FD0|[7:4]|0x0|4KB region count.|
|SMMU_PIDR4|0x00FD0|[3:0]|0x4|JEP106 continuation code for Arm.|
|SMMU_PIDR5|0x00FD4|[7:0]|0x00|Reserved.|
|SMMU_PIDR6|0x00FD8|[7:0]|0x00|Reserved.|
|SMMU_PIDR7|0x00FDC|[7:0]|0x00|Reserved.|
|SMMU_PIDR0|0x00FE0|[7:0]|0x86|Part number[7:0].|
|SMMU_PIDR1|0x00FE4|[7:4]|0xB|JEP106 ID code[3:0] for Arm.|


|Register|Oﬀset|Bits|Value|Description|
|---|---|---|---|---|
| | |[3:0]|0x4|Part number[11:8].|
|SMMU_PIDR2|0x00FE8|[7:4]|0x1|MMU-600AE major revision.<br><br>The value 0x1 indicates major product revision r1.|
|SMMU_PIDR2|0x00FE8|[3]|0b1|The component uses a manufacturer identity code that JEDEC allocates, according to the JEP106 speciﬁcation.|
|SMMU_PIDR2|0x00FE8|[2:0]|0b011|JEP106 ID code[6:4] for Arm.|
|SMMU_PIDR3|0x00FEC|[7:4]|MAX[0x0,ecorevnum]|MMU-600AE minor revision.<br><br>The value 0x0 indicates minor product revision p0.|
|SMMU_PIDR3|0x00FEC|[3:0]|0x0|CMOD. This ﬁeld is not used.|
|SMMU_CIDR0|0x00FF0|[7:0]|0x0D|Preamble.|
|SMMU_CIDR1|0x00FF4|[7:0]|0xF0|Preamble.|
|SMMU_CIDR2|0x00FF8|[7:0]|0x05|Preamble.|
|SMMU_CIDR3|0x00FFC|[7:0]|0xB1|Preamble.|


##### 3.8 TBU microarchitectural registers

You can set the MMU-600AE TBU microarchitectural registers at boot time to optimize TBU behavior for your system. We recommend that you use the default values for most systems.

- Table 3-26: TBU microarchitectural register summary


|Oﬀset|Name|Type|Reset|Width|Description|
|---|---|---|---|---|---|
|0x08E00|TBU_CTRL|RW|0x00000000|32-bit|3.8.1 TBU_CTRL, TBU Control register on page 88|
|0x08E18|TBU_SCR|RW|0x00000000|32-bit|3.8.2 TBU_SCR, TBU Secure Control register on page 89|


###### 3.8.1 TBU_CTRL, TBU Control register

The TBU_CTRL register disables MMU-600AE TBU features. Do not modify the bits in this register unless we direct you to do so.

0x08E00 Type

RW

Reset value

0x00000000

###### Usage constraints

When TBU_SCR.NS_UARCH = 0, Non‑secure accesses to this register are RAZ/WI. See 3.8.2 TBU_SCR, TBU Secure Control register on page 89.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-12: TBU_CTRL register bit assignments

|31| | |16|15 13 12|15 13 12|11| |1 0|1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved| | |AUX[11:1]|AUX[11:1]|AUX[11:1]| |


AUX[15:13] HAZARD_DIS LTI_PORT_RESOURCE_LIMIT_DISABLE

###### Table 3-27: Register name bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:16]| |Reserved|
|[15:0]|AUX[15:0]|Leave each of these bits as 0|


###### 3.8.2 TBU_SCR, TBU Secure Control register

The TBU Secure Control register in the MMU-600AE TBU controls whether Non‑secure software is permitted to access the TBU registers.

0x08E18 Type

RW

Reset value

0x00000000

###### Usage constraints

This register is accessible only by Secure software. Non‑secure accesses to this register are RAZ/ WI. This register does not control Secure access to the MMU-600AE PMU registers. To control Secure PMU register access, use the SMMU_PMCG_SCR register. See 3.1 SMMU architectural registers on page 61.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-13: TBU_SCR register bit assignments

|31| | | | | | |2 1 0|2 1 0|2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | |


NS_RAS

NS_UARCH

###### Table 3-28: TBU_SCR register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:2]|-|Reserved.|
|[1]|NS_RAS|Non‑secure register access to RAS registers:<br><br>0 Non‑secure accesses to register addresses 0x08E80 0x08EC0 are RAZ/WI.<br>1 Non‑secure access to RAS registers is permitted.<br><br><br>The sec_override input signal deﬁnes the reset value of this bit. See A.21 TBU tie-oﬀ signals for MMU-600AE on page 177.|
|[0]|NS_UARCH|Non‑secure register access to TBU_CTRL:<br><br>0 Non‑secure accesses to TBU_CTRL are RAZ/WI.<br>1 Non‑secure accesses to TBU_CTRL are permitted.<br><br><br>The sec_override input signal deﬁnes the reset value of this bit. See A.21 TBU tie-oﬀ signals for MMU-600AE on page 177.|


##### 3.9 TBU RAS registers

MMU-600AE includes TBU registers that are related to Reliability, Availability, and Serviceability (RAS).

- Table 3-29: TBU RAS register summary


|Oﬀset|Name|Type|Reset|Width|Description|
|---|---|---|---|---|---|
|0x08E80|TBU_ERRFR|RO|0x00000081|32-bit|3.9.1 TBU_ERRFR, TBU Error Feature register on page 91|
|0x08E88|TBU_ERRCTLR|RW|0x00000008|32-bit|3.9.2 TBU_ERRCTLR, TBU Error Control register on page 92|
|0x08E90|TBU_ERRSTATUS|RW|0x00000000|32-bit|3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register on page 93|
|0x08EC0|TBU_ERRGEN|RW|0x00000000|32-bit|3.9.4 TBU_ERRGEN, TBU Error Generation register on page 94|


###### 3.9.1 TBU_ERRFR, TBU Error Feature register

Use the TBU Error Feature register to discover how the MMU-600AE TBU handles errors.

###### Conﬁgurations

This register exists in all TBU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-29: TBU RAS register summary on page 91. Address oﬀset

0x08E80 Type

RO

Reset value

0x00000081

###### Usage constraints

This register is read‑only. When TBU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-14: TBU_ERRFR register bit assignments

|31| | | | |8|7 6 5|7 6 5|2 1 0|2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|FI|Reserved|Reserved|ED|


###### Table 3-30: TBU_ERRFR register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:8]|-|Reserved|
|[7:6]|FI|The value 0b10 indicates that the fault handling interrupt is controllable|
|[5:2]|-|Reserved|
|[1:0]|ED|The value 0b01 indicates that TBU error detection is always enabled|


###### 3.9.2 TBU_ERRCTLR, TBU Error Control register

Use the TBU Error Control register to enable fault handling interrupts. MMU-600AE must be connected to an interrupt controller when Reliability, Availability, and Serviceability (RAS) interrupts are enabled for fault handling.

###### Conﬁgurations

This register exists in all MMU-600AE conﬁgurations. An instance of this register exists for each implemented TBU.

###### Attributes

Width

32

Functional group

See Table 3-29: TBU RAS register summary on page 91. Address oﬀset

0x08E88 Type

RW

Reset value

0x00000008

###### Usage constraints

When TBU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 3-15: TBU_ERRCTLR register bit assignments

|31| | | | | |4|3 2 0|3 2 0|
|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | |


FI Reserved

###### Table 3-31: TBU_ERRCTLR register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:4]|-|Reserved.|
|[3]|FI|Enable fault handling interrupts:<br><br>0 No interrupt is generated when a fault occurs<br>1 An interrupt is generated on ras_irpt when a fault occurs<br>|
|[2:0]|-|Reserved|


###### 3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register

Use the TBU Error Record Primary Syndrome register to ﬁnd out whether diﬀerent types of error have occurred on the MMU-600AE TBU.

###### Conﬁgurations

This register exists in all MMU-600AE conﬁgurations. An instance of this register exists for each implemented TBU.

###### Attributes

Width

32

Functional group

See Table 3-29: TBU RAS register summary on page 91. Address oﬀset

0x08E90 Type

RW

Reset value

0x00000000

###### Usage constraints

When TBU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI. To prevent race conditions, under certain circumstances, writes to some bits in this register are ignored. Typically, these writes are ignored when software has not yet observed a new error.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-16: TBU_ERRSTATUS register bit assignments

|31 30 29 28|31 30 29 28|31 30 29 28|27 26 25 24|27 26 25 24|27 26 25 24|23| | |8|7|0|
|---|---|---|---|---|---|---|---|---|---|---|---|
| |V| | | |CE|Reserved|Reserved|Reserved|Reserved|SERR|SERR|


Reserved

OF Reserved Reserved

###### Table 3-32: TBU_ERRSTATUS register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31]|-|Reserved.|
|[30]|V|Register valid. This bit is set to 1 to indicate that at least one RAS error was recorded.<br><br>Clear this bit by writing a 1 to it. If CE is not 0b00 and is not being cleared, the write is ignored. A write of 0 is ignored.|
|[29:28]| |Reserved.|
|[27]|OF|Overﬂow. This bit is set to 1 to indicate that multiple correctable errors were recorded. That is, at least one correctable error was recorded when CE != 0b00.<br><br>Clear this bit by writing a 1 to it. A write of 0 is ignored.|
|[26]|-|Reserved.|
|[25:24]|CE|Correctable Error. This ﬁeld is set to 0b10 to indicate that a corrected error occurred. Clear this ﬁeld by writing 0b11 to it. If OF is set to 1 and is not being cleared, the write is ignored. A write of any value other than 0b11 is ignored.|
|[23:8]|-|Reserved.|
|[7:0]|SERR|Error code. This ﬁeld provides information about the earliest unacknowledged correctable error, as follows:<br><br>0x00 No error. This code occurs when CE = 0b00.<br><br>0x07 Main TLB tag is corrupted. This code can occur when CE != 0b00.<br>0x08 Main TLB data is corrupted. This code can occur when CE != 0b00.<br><br><br>Writes to this ﬁeld are ignored.|


###### 3.9.4 TBU_ERRGEN, TBU Error Generation register

Use the MMU-600AE TBU Error Generation register to generate tag parity errors. You might want to generate errors in certain cases, such as when testing error‑handling software.

The errors that are injected using this mechanism are correctable errors but are not reported in the FMU error records. See:

- • 4.4.4 Safety Mechanism table on page 109.
- • 4.4.4.2 Injecting an error into a Safety Mechanism on page 112.


###### Conﬁgurations

This register exists in all TBU conﬁgurations.

###### Attributes

Width

32

Functional group

See Table 3-29: TBU RAS register summary on page 91. Address oﬀset

0x08EC0 Type

RW

Reset value

0x00000000

###### Usage constraints

When TBU_SCR.NS_RAS = 0, Non‑secure accesses to this register are RAZ/WI.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 3-17: TBU_ERRGEN register bit assignments

|31| | | | | | |2 1 0|2 1 0|2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | |


TMTLB

DMTLB

###### Table 3-33: TBU_ERRGEN register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[31:2]|-|Reserved.|
|[1]|TMTLB|Main TLB tag parity error:<br><br>0 No tag parity error is written to the Main TLB.<br>1 Entries that are written to the Main TLB include a tag parity error. A fault occurs when the entry is used.<br>|


|Bits|Name|Description|
|---|---|---|
|[0]|DMTLB|Main TLB data parity error:<br><br>0 No data parity error is written to the Main TLB.<br>1 Entries that are written to the Main TLB include a data parity error. A fault occurs when the entry is used.<br><br><br>Note: Tag parity errors mask data parity errors. When testing data parity error functionality, ensure that software does not set this bit and the TMTLB bit at the same time.|


## 4. Functional Safety

MMU-600AE is a version of MMU-600, using the same code base, with Functional Safety (FuSa) detection and correction features added. The original MMU-600 logic and functionality are unchanged.

We describe the Functional Safety FuSa detection features that are unique to MMU-600AE. That is, the focus is only on the added FuSa features and logic unique to MMU-600AE, in contrast to the functionality inherited from MMU-600.

MMU-600AE provides built-in Safety Mechanisms. The following ﬁgure shows the distribution of the main Safety Mechanisms in MMU-600AE.

###### Figure 4-1: Safety Mechanism distribution

Interrupts ACE-Lite Interrupts ACE-Lite Interrupts DTI-ATS

- Q-Channel-cg

P

- Q-Channel-pd Q-Channel-pd


ACE-Lite

Q-Channel-cg

Q-Channel-cg

P P

P

P

P P

SZ SZ

ADB

RAMs

RAMs

RAMs

TBU

TBU

TBU

Full duplication / parity Full duplication / parity Full duplication / parity

P

P

P

Q-Channel-pd

LPD_PD ADB LPD_CG

LPD_PD

ADB

LPD_CG

SZ SZ

RS RS

P

Full duplication / parity

FHI

P

ERI

RAMs

FMU TCU

FMU APB

P

P P P

P

Interrupts

ACE-Lite-DVM APB

Q-Channel-pd

|Legend:<br><br>AXI4-Stream interconnect protection (full duplication)<br><br>Parity protection<br><br>Interrupt protection (parity)<br><br>Interface protection (AMBA FuSa)<br><br>Interface protection (Arm LPI FuSa) P<br><br>Logic (duplication)<br><br>RAM (CRC)<br><br>AXI4-Stream interconnect<br><br>Sizer Register Slice Asynchronous Domain Bridge Low Power Distributor<br><br>SZ RS<br><br>ADB LPD|
|---|


MMU-600AE contains the following main Safety Mechanisms: Lock-step logic protection

The logic is protected with duplicated logic running in lock-step. See 4.5 Lock-step protection on page 140.

RAM protection The RAMs are shared between the duplicated blocks and are protected with 8‑bit CRC, which covers the address and data. See 4.6 RAM protection on page 142.

AMBA® AXI4-Stream interconnect protection The AXI4‑Stream interconnect that connects MMU-600AE blocks is protected by end‑to‑end duplication. The following components, which are part of the DTI interconnect, are protected with full duplication:

- • DTI switch
- • DTI register slice


It is the responsibility of the integrator to instantiate a redundant instance of these components and stitch the duplicate DTI interconnect, except for the ADB.

See:

- • 4.7.2 AXI4-Stream interface parity protection on page 148
- • 4.8 Integrating the TCU, TBU, LPD, PCIe ATS, and DTI AXI4-Stream interconnect on page 153


AMBA external interface protection All external interfaces are protected with AMBA parity extension. AMBA parity extension protects point‑to‑point connections consisting of wires and buﬀers only, and no gates. ACE and APB external ports are also included. See:

- • 4.7.1 ACE-Lite interface parity protection on page 148
- • 4.7.3 APB interface parity protection on page 149


Interrupt outputs parity protection The interrupt outputs from MMU-600AE are protected with parity protection and are compatible with the Arm CoreLink™ GIC-600AE Generic Interrupt Controller IP. There is one parity bit for each interrupt output. For more information, see 4.7.5 Interrupt output protection on page 152.

Q‑Channel protection The Q‑Channel is protected by asynchronous parity. See 4.9 Q-Channel protection on page 154.

Systematic fault watchdog MMU-600AE contains a watchdog‑based PING/ACK mechanism. This mechanism protects against systematic errors on the interconnect that connects the various MMU-600AE TBU blocks. The watchdog works by engaging a hardware mechanism in the TCU that pings each TBU in a round‑robin fashion and waits for a response. If the mechanism does not receive a response within the programmable timeout window, it reports a fault. See 4.10 Systematic fault watchdog protection on page 161.

Clocks and resets The clocks and resets are duplicated. The clocks operate with a temporal delay of two cycles. That is, the primary logic operates two cycles ahead of the redundant logic. See the Clocks and resets section of the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.

Fault Management Unit

The Fault Management Unit (FMU) has the following functions:

- • Processes faults that the Safety Mechanisms in the TCU and TBUs detect.
- • Records the fault syndrome in the Error Records and reports the fault using the Error Recovery Interrupt (ERI) and Fault Handling Interrupt (FHI).
- • Provides fault injection and clearing for each Safety Mechanism.


The FMU communicates with an external Safety Island, using a second APB port that is dedicated to the FMU. The APB port is added for FuSa purposes so that faults can be reported even when the MMU-600 primary logic functionality is either unreliable or inaccessible.

For more information about the FMU, see 4.4 Fault Management Unit on page 105.

##### 4.1 FuSa I/Os

Ports have been added for Functional Safety Fault Detection and Control. This section refers to new interfaces, such as the APB interface, and the protection for existing interfaces. For more information, see 4.7 External interface protection on page 145.

###### 4.1.1 Non-architected FuSa ports

The following FuSa ports have been added for Fault Detection and Control (FDC). The following table shows the non‑architected FuSa ports.

- Table 4-1: Non‑architected FuSa ports


|Port|Direction|Blocks|Description|
|---|---|---|---|
|aclk_fdc|Input|TCU/ TBU|Clock for redundant logic and Safety Mechanisms.|
|aresetn_fdc|Input|TCU/ TBU|Reset for redundant logic and Safety Mechanisms.|
|mbistresetn_fdc|Input|TCU/ TBU|Redundant nmbistreset. Both resets must assert together.|
|fmu_resetn_fdc|Input|TCU|Resets FMU Error Records. Main reset does not reset error records.|
|dftrstdisable_fdc|Input|TCU/ TBU|Prevents reset from asserting when reset generation FDC ﬂops are scanned.|


|Port|Direction|Blocks|Description|
|---|---|---|---|
|dftcgen_fdc|Input|TCU/ TBU|Forces FDC clock gate enable to ensure scanned ﬂops obtain a clock.|
|dftramhold_fdc|Input|TCU/ TBU|Redundant port for dftramhold.|
|fmu_err_out|Output|TBU|Fault indication to the TCU block from a TBU block.|
|ras_irpt_chk|Output|TCU/ TBU|chk bit for legacy MMU-600 Programmer's View (PV) RAS Interrupt from System MMU.|
|pmu_irpt_chk|Output|TCU/ TBU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|event_q_irpt_s_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|event_q_irpt_ns_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|pri_q_irpt_ns_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|cmd_sync_irpt_ns_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|cmd_sync_irpt_s_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|global_irpt_ns_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|global_irpt_s_chk|Output|TCU|chk bit for legacy MMU-600 Interrupt from System MMU.|
|evento_chk|Output|TCU|chk bit for legacy MMU-600 evento output.|
|fmu_fault_int|Output|TCU|FHI Interrupt from TCU FMU to Safety Island.|
|fmu_fault_int_chk|Output|TCU|chk bit for FHI Interrupt from TCU FMU to Safety Island.|
|fmu_err_int|Output|TCU|ERI Interrupt from TCU FMU to Safety Island.|
|fmu_err_int_chk|Output|TCU|chk bit for ERI Interrupt from TCU FMU to Safety Island.|
|freq[TCUCFG_FUSA_FCHAN_COUNT−1:0]|Input|TCU|Fault Indicator Request from ADB/LPD to TCU.|
|fack[TCUCFG_FUSA_FCHAN_COUNT−1:0]|Output|TCU|Fault Indicator Ack from TCU to ADB/LPD.|
|freq_chk[TCUCFG_FUSA_FCHAN_COUNT−1:0]|Input|TBU|Redundant Fault Indicator Request from ADB/LPD to TCU.|
|fack_chk[TCUCFG_FUSA_FCHAN_COUNT−1:0]|Output|TBU|Redundant Fault Indicator Ack from TCU to ADB/LPD.|
|fmu_err_in[TCUCFG_FUSA_ TBU_FAULT_WIRE_COUNT−1:0]|Input|TBU|Fault indicator input from individual TBUs.|
|pcie_mode_chk|Input|TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|ns_sid_high_chk|Input|TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|s_sid_high_chk|Input|TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|cmo_disable_chk|Input|TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|max_tok_trans_chk|Input|TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|utlb_roundrobin_chk|Input|TCU/ TBU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|sec_override_chk|Input|TCU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|sup_cohacc_chk|Input|TCU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|sup_btm_chk|Input|TCU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|sup_sev_chk|Input|TCU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|
|sup_oas_chk|Input|TCU|Redundant tie‑oﬀs, opposite polarity of original tie‑oﬀs.|


###### 4.1.2 Q-Channel FuSa ports

The following interfaces add _chk bits, as speciﬁed in the Arm Q‑Channel Parity Extensions. For more information, see 4.9 Q-Channel protection on page 154.

- Table 4-2: Q-Channel FuSa ports

|Port|Direction|Blocks|Description|
|---|---|---|---|
|qreqn_chk_pd|Input|TCU/TBU|Redundant qreqn port for Q‑Channel power controller|
|qactive_chk_pd|Output|TCU/TBU|Redundant qactive port for Q‑Channel power controller|
|qaccept_chk_pd|Output|TCU/TBU|Redundant qaccept port for Q‑Channel power controller|
|qdeny_chk_pd|Output|TCU/TBU|Redundant qdeny port for Q‑Channel power controller|
|qreqn_chk_cg|Input|TCU/TBU|Redundant qreqn port for Q‑Channel clock controller|
|qactive_chk_cg|Output|TCU/TBU|Redundant qactive port for Q‑Channel clock controller|
|qaccept_chk_cg|Output|TCU/TBU|Redundant qaccept port for Q‑Channel clock controller|
|qdeny_chk_cg|Output|TCU/TBU|Redundant qdeny port for Q‑Channel clock controller|


4.1.3 AMBA interface FuSa ports

The following interfaces add _chk bits, as the AMBA® Parity Extensions specify. For more information, see 4.7 External interface protection on page 145. The APB port was added for FDC between the FMU block residing in the TCU and the Safety Island in the SoC.

- Table 4-3: AMBA interface FuSa ports


|Port|Granularity|Description|
|---|---|---|
|APB interface|TCU|APB4 interface added for FMU as the 4.7 External interface protection on page 145 section describes.|
|AXI4‑Stream AMBA® parity|TCU/TBU|AMBA® Parity added to all external AXI4‑Stream interfaces as the 4.7 External interface protection on page 145 section describes.|
|ACE AMBA® parity|TCU/TBU|AMBA® Parity added to all external ACE‑Lite interfaces as the 4.7 External interface protection on page 145 section describes.|


##### 4.2 Clocks and resets

The MMU-600AE clocks and resets are identical to those of MMU-600, except for:

- • The added redundant clock and reset.
- • The added fmu_resetn and fmu_resetn_fdc signals.


The following ﬁgure shows how the redundant clock and reset is used by the FDC logic. The extra aresetn_fdc and aclk_fdc signals provide redundancy in the clock and reset trees while guarding against faults on the tree branches. If a fault occurs on a branch in either the primary or FDC clock trees, the Dual LockStep (DLS) comparators detect the fault.

###### Figure 4-2: MMU-600AE block resets

fmu_resetn_fdc

aresetn_fdc

fmu_resetn

aclk_fdc

aclk_fdc

aresetn

aclk

aresetn_sync aclk

dftrstdisable dftrstdisable_fdc

dftrstdisable dftrstdisable_fdc

reset_sync_prot (main reset)

reset_sync_prot (fmu reset)

fmu_resetn_fdc_sync

aresetn_fdc_sync

fmu_resetn_sync

|TCU no_ram<br><br>RAM inputs<br><br>RAM outputs| |
|---|---|
|TCU no_ram<br><br>RAM inputs<br><br>RAM outputs|Shared RAM<br><br>|


| |TCU no_ram (duplicated)<br><br>RAM inputs<br><br>RAM outputs|
|---|---|
| |TCU no_ram (duplicated)<br><br>RAM inputs<br><br>RAM outputs|


+

fault

aclk aclk_fdc

The following conditions apply to a TCU block with DLS:

- • Internal _sync resets are asynchronous‑assert.
- • Internal _sync resets are synchronous‑deassert.
- • The aresetn_fdc_sync and fmu_resetn_fdc_sync signals are deasserted two cycles after the non-FDC signals.


The fmu_resetn and fmu_resetn_fdc signals reset only the FMU registers. Asserting only the aresetn and aresetn_fdc signals resets all registers except for the FMU PV registers. This enables software to view the contents of the registers after reset. Both aresetn and fmu_resetn, and their _fdc counterparts, should be reset during a Cold reset or when debug information from the FMU PV registers is not required.

For more information on integrating the FuSa clocks and resets, see the FuSa clocks and resets section of the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.

##### 4.3 DFT protection

Functional Safety Mechanisms have been added to protect the MBIST and ATPG Scan logic from faults during functional mode.

###### 4.3.1 MBIST

The MBIST wrapper logic built into MMU-600AE is duplicated, which lets the mechanism detect faults in this logic.

For example, it can detect a fault on a RAM address bit, due to the comparators at the inputs of the shared RAMs.

The MBIST interface inputs in the following table can cause mission-mode errors, so they are protected.

- Table 4-4: Protected MBIST inputs


|Signal|Protection|Notes|
|---|---|---|
|mbistreq|Assertion detection|If mbistreq is asserted when the MMU block is in functional mode, the MMU detects and reports it. If this happens in MBIST mode, it is assumed the fault is ignored and cleared by software, using a reset or the FMU clearing mechanism. Alternatively, to prevent the fault from asserting, software can disable the Safety Mechanism before entering MBIST mode.|
|nmbistresetn|Duplication|The reset is duplicated. The duplicated reset is nmbistresetn_fdc. For a reset to occur, both nmbistresetn and nmbistresetn_fdc must be asserted. For more information, see Resets of the FuSa clocks and resets section of the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.|


The MBIST interface pins themselves are unchanged from MMU-600.

The other MBIST inputs, including mbistaddr and mbistindata, are benign and cause no harm if they experience faults during functional mode.

If faults occur on the MBIST controller or MBIST signals, it is assumed that the MBIST controller detects them.

###### 4.3.2 ATPG Scan

All DFT and ATPG input ports are duplicated. These duplicate ports allow the SoC integrator to have separate scan chains for clk and clk_fdc, if wanted. If the scan chains are shared by clk and clk_fdc ﬂops, drive the duplicate ports in the same way at the same time.

The following table summarizes the duplicate ports.

- Table 4-5: Duplicate ATPG input ports


|clk scan input|clk_fdc scan input|Description|
|---|---|---|
|dftrstdisable|dftrstdisable_fdc|Prevents reset from asserting when reset generation ﬂops are scanned.|
|dftcgen|dftcgen_fdc|Forces clock gate enable to ensure scanned ﬂops get a clock|
|dftramhold|dftramhold_fdc|Asserting prevents RAM access during ATPG. This can reduce coverage for logic in the RAM shadow.|


###### 4.3.3 LBIST

Arm has veriﬁed that a third-party LBIST controller can be instanced and used to control the scan chains and obtain additional latent fault coverage or diagnostic information.

##### 4.4 Fault Management Unit

The FMU is located in the TCU. It processes faults that the TCU and TBU Safety Mechanisms detect.

The FMU implements the following functionality:

- • Receives errors signaling from all Safety Mechanisms within other MMU blocks.
- • Routes all errors to the Safety Island, if enabled.
- • Maintains Error Records for each MMU block, which are stored in registers.
- • The Error Records and other registers are accessible through a dedicated, safe APB4 interface.
- • It allows software to enable or disable a Safety Mechanism within an MMU block.
- • Enables software error recovery testing by allowing error injection in a Safety Mechanism within an MMU block.
- • Retains Error Records across functional reset. The following ﬁgure shows the FMU interconnections.


This is a dedicated APB port for the FMU, which is diﬀerent than the APB port accessing the programmable registers of MMU-600.

Figure 4-3: FMU in MMU block

###### TCU

###### FMU

APB4 interface

fack

Error Records

fmu_err_int fmu_fault_int

freq

fmu_err_in

###### DTI interconnect

ADB

TBU

###### TBU TBU

fmu_err_out

fmu_err_out fmu_err_out

###### 4.4.1 Error signaling to the FMU

MMU-600AE deploys numerous Safety Mechanisms in each MMU TCU and TBU block to protect them from transient or permanent errors.

Each Safety Mechanism outputs an error signal which then sends those errors to the FMU residing in the TCU.

The TBU uses the existing AXI4-Stream interconnect to report errors that any Safety Mechanism within the block to the TCU detects.

In addition to reporting errors through the AXI4‑Stream interconnect, each TBU has an output, fmu_err_out, which indicates an actual uncorrected error within its block. Corrected errors never raise fmu_err_out, even if conﬁgured to report as uncorrected. See 4.4.8 Correctable Error enable on page 119. The fmu_err_out signal must be connected to the fmu_err_in input of the TCU to provide a redundant path for error signaling from the TBUs to the FMU residing in the TCU. The fmu_err_in wire stays asserted by the TBU until the error recovery software clears the error.

Smaller components without an AXI4‑Stream interface needing to report errors do so through the F‑Channel interface. The TCU can be connected to 16 such interfaces. MMU-600AE has modiﬁed the ADB and LPD components with an F‑Channel interface for this reason. For more information, see 4.7.4 F-Channel on page 149.

- 4.4.2 Error signaling from the FMU When a Safety Mechanism detects an error, it is sent to the FMU.


If enabled, the FMU signals the error to the outside world using the level‑sensitive interrupt error ports:

- • Error Recovery Interrupt (ERI), fmu_err_int.
- • Fault Handling Interrupt (FHI), fmu_fault_int. Error reporting through the ERI or FHI is enabled through the 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.


These error interrupts are disabled after reset by fmu_aresetn and software must enable them at startup.

Detected Uncorrectable Errors can be reported as ERI, FHI, or both when enabled. Detected Correctable Errors can be reported as FHI when enabled. FMU_ERR<n>CTLR.FI and FMU_ERR<n>CTLR.UI control this reporting. The grouping of the errors into these two categories can be helpful in redirecting these errors to diﬀerent error recovery handlers based on the criticality of the errors or other factors that are known at the system level.

###### 4.4.2.1 Reset

The fmu_aresetn signal resets the FMU Error Records and FMU_ERRGSR. The aresetn signal resets the main FMU logic and all other registers, including locking the register ﬁle.

This enables software to reset the MMU while keeping the Error Records intact for later interrogation and debug. For more information, see the Resets section of the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.

If the FMU Error Records are not reset, but the rest of the system is reset, then an error record with FMU_ERR<n>STATUS.V==1 continues to assert an interrupt if the appropriate control bit, FMU_ERR<n>CTLR.UI or FMU_ERR<n>CTLR.FI enables the interrupt. The system reset signal does not aﬀect the interrupt, and when the system comes out of reset, then the interrupt is still asserted.

If the FMU error records are not reset, then when the system comes out of reset, any new RAS errors that occur from system reset are logged if enabled by FMU_ERR<n>CTLR.ED.

- 4.4.3 Error Record table The MMU-600AE faults are recorded in Error Records.


Each MMU block that the MMU block IDs table in 4.4.3.2 Functional PV and FMU PV on page 108 shows has its own error record where the index of the error record is the block ID. Each error record contains several registers and the table FMU PV registers in 4.4.9.1 FMU Programmer's View registers on page 121 describes their oﬀsets.

- 4.4.3.1 Accessing FMU Error Records using the FMU APB These FMU Error Records are accessed only by the dedicated FMU APB interface on the TCU.


This interface is separated from the main functional APB interface to minimize the chance of main logic or interface faults aﬀecting the FDC logic.

###### 4.4.3.2 Functional PV and FMU PV

Non‑FMU registers, such as those used to conﬁgure and control the MMU functions, are accessed through the main APB interface on the TCU.

For any CRC or parity faults in:

- • The TBU MTLB Tag and Data RAMs.
- • The TCU CCB and WCB Tag and Data RAMs.

The MMU-600 PV detects and reports these faults as Corrected Errors in the MMU-600 PV RAS registers. These faults are also Safety Mechanisms TBU SMID:7-8 and TCU SMID:7-10 and an attempt is made to report them independently in the FMU as either Corrected Errors or Uncorrected Errors depending on the corresponding FMU_ERR<n>CTLR.CE_EN ﬁeld. See:

- • 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.
- • 4.4.4 Safety Mechanism table on page 109.


This method enables the host processor and a dedicated FDC processor to act on faults separately and perform diﬀerent tasks. If this functionality is not required, the host software can disable the interrupts that are associated with the functional Error Records.

- Table 4-6: MMU block IDs


|Block ID|MMU block|
|---|---|
|0|TCU.|
|1|Fault Channels. LPD and ADB faults are reported in this Error Record.|
|2|TBU0.|
|3|TBU1.|
|4|TBU2.|
|...|...|
|61|TBU59.|
|62|TBU60.|
|63|TBU61.|


The number of Error Records that are supported depends on the number of TBU blocks that are connected to the TCU. For unused TBU blocks, the Error Record registers become RAZ.

For more information about these Error Records, see 4.4.9 Fault Detection and Control Programmer's View on page 119.

###### 4.4.4 Safety Mechanism table

If an uncorrectable error occurs, then the system might behave unpredictably. To counter this, MMU-600AE provides built-in Safety Mechanisms in the MMU blocks.

For faults in RAMs, unpredictable behavior can include, but is not limited to:

- • Data corruption.
- • System lock‑up in any of the upstream or downstream systems of the TBUs and DTI requesters.
- • Security violation.

For interface errors, unpredictable behavior can include any of the above behaviors. In addition, depending on the interface, the unpredictable behavior can also include, but is not limited to:

- • Potentially, using and/or reporting bad data and addresses to access the FMU register ﬁle.
- • No response to ping packets.
- • FMU_ERRSTATUS.idle never returning to 1, that is, the idle state. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.


The following table shows the Safety Mechanisms inside the MMU blocks.

###### Table 4-7: Safety Mechanisms

|Block|Safety Mechanism identiﬁer|Safety Mechanism description|
|---|---|---|
|TCU|0|Reserved.|
|TCU|1|TCU Dual LockStep error. See 4.5 Lock-step protection on page 140.|
|TCU|2|TCU DTI AXI4‑Stream interface error. See 4.7.2 AXI4-Stream interface parity protection on page 148 and 4.8 Integrating the TCU, TBU, LPD, PCIe ATS, and DTI AXI4-Stream interconnect on page 153.|
|TCU|3|TCU APB interface error. See 4.7.3 APB interface parity protection on page 149.|
|TCU|4|TCU ACE‑Lite DVM requester interface error. See 4.7.1 ACE-Lite interface parity protection on page 148.|
|TCU|5|TCU LPI_PD Q‑Channel interface error.|
|TCU|6|TCU LPI_CG Q‑Channel interface error.|
|TCU|7|TCU Conﬁguration Cache Block (CCB) Tag RAM CRC error. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|
|TCU|8|TCU CCB Entry RAM CRC error. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|
|TCU|9|TCU Walk Cache Block (WCB) Tag RAM CRC error. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|
|TCU|10|TCU WCB Entry RAM CRC error. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|
|TCU|11|TCU DTI Buﬀer (DTIB) RAM CRC error. Stores messages received over DTI.|
|TCU|12|FMU Ping Ack error.|
|TCU|13|FMU APB interface error. See 4.7.3 APB interface parity protection on page 149.|
|TCU|14|MBIST Req error. This is disabled by default.|
|TCU|15|Tie‑oﬀ error. See 4.7.6 Tie-oﬀ input protection on page 152.|
|TCU|16|FMU clock gating override.<br><br>Note: The scope of FMU clock gating override is limited to the FMU and does not apply to the parent module, that is, the TCU. The FMU clock gating override Safety Mechanism TCU_FMU_CLK_GATING does not record errors. Enabling TCU_FMU_CLK_GATING disables the internal FMU Clock Gating, allowing the FMU Clock to run continuously.<br><br>This is disabled by default.|
|TBU|0|Some error in TBU, precise source yet unknown.|
|TBU|1|TBU Dual LockStep error. See 4.5 Lock-step protection on page 140.|
|TBU|2|TBU ACE‑Lite completer (TBS) interface error. See 4.7.1 ACE-Lite interface parity protection on page 148.|
|TBU|3|TBU ACE‑Lite requester (TBM) interface error. See 4.7.1 ACE-Lite interface parity protection on page 148.|
|TBU|4|TBU DTI AXI4‑Stream interface error. See 4.7.2 AXI4-Stream interface parity protection on page 148 and 4.8 Integrating the TCU, TBU, LPD, PCIe ATS, and DTI AXI4-Stream interconnect on page 153.|
|TBU|5|TBU LPI_PD Q‑Channel interface error. Low‑Power Interface Power‑Down (LPI_PD) represents a request from the power-controller to alter the power state of the device.|
|TBU|6|TBU LPI_CG Q‑Channel error. Low‑Power Interface Clock Gating (LPI_CG) might be a software‑based or HW‑based request to go into a lower power state by clock gating.|
|TBU|7|TBU MTLB Tag RAM CRC error. MTLB is a TLB‑like structure for caching translations. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|


|Block|Safety Mechanism identiﬁer|Safety Mechanism description|
|---|---|---|
| |8|TBU MTLB Entry RAM CRC error. See 4.4.8 Correctable Error enable on page 119 and 4.6 RAM protection on page 142.|
| |9|TBU WBB MFIFO RAM CRC error. WBB is the store buﬀer for client data.|
| |10|MBIST Req error. This is disabled by default.|
| |11|Tie‑oﬀ error. See 4.7.6 Tie-oﬀ input protection on page 152.|
| |12|FMU clock gating override.<br><br>Note: The scope of FMU clock gating override is limited to the FMU and does not apply to the parent module, that is, the TBU. The FMU clock gating override Safety Mechanism TBU_FMU_CLK_GATING does not record errors. Enabling TBU_FMU_CLK_GATING disables the internal FMU Clock Gating, allowing the FMU Clock to run continuously.<br><br>This is disabled by default.|
|F‑Channel|id|Index of freq[id].|


The SMID value 0 for TBU error record[N] indicates that the FMU has detected an actual uncorrected error in the TBU[N-2], as indicated by the fmu_err_out of the TBU that is raising an error on the fmu_err_in of the FMU. Corrected errors never raise fmu_err_out, even if conﬁgured to report as uncorrected. See 4.4.1

Error signaling to the FMU on page 106. The safety mechanism that reported this error has still not been determined. The safety mechanism that reported the error is updated after the safety mechanism in the TBU sends this information over the DTI interface to the FMU in the TCU. This information is then updated in the FMU_ERR<n>STATUS.IERR ﬁeld. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.

If a software read of FMU_ERR<n>STATUS.IERR returns SMID:0, then the software is expected to read this register again. If repeated reads of this register do not indicate the safety mechanism other than SMID:0, then it might indicate that the DTI interconnect is broken, possibly because of a permanent fault, and is unable to receive DTI messages. The error recovery software does not have the Safety Mechanism information from the TBU that had this fault. The software should therefore perform error recovery by resetting that TBU and the DTI interconnect components.

- 4.4.4.1 Enabling and disabling a Safety Mechanism All Safety Mechanisms are enabled on reset, except for MBIST Req and FMU clock gating override.


To enable or disable a Safety Mechanism, write to the FMU_SMEN register. FMU_SMEN.BLK speciﬁes the MMU block. FMU_SMEN.SMID speciﬁes the Safety Mechanism within the MMU block to be enabled or disabled.

When a block is powered‑down and then powered‑up again, then the enabled state of the Safety Mechanisms in that block return to the reset value and it might be necessary for software to redisable or reenable them.

###### 4.4.4.2 Injecting an error into a Safety Mechanism

To inject an error into a Safety Mechanism, write to the FMU_SMINJERR register. FMU_SMINJERR.BLK speciﬁes the MMU block, and FMU_SMINJERR.SMID speciﬁes the Safety Mechanism into which to inject the error.

FMU_STATUS.idle protects the FMU_SMINJERR register. See:

- • 4.4.5.3 FMU idle on page 114.
- • 4.4.9.1.9 FMU_SMINJERR, Safety Mechanism Inject Error Register on page 135.
- • 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138. This method injects only one error. No clearing of error injection is required. Errors that are injected into the following are always corrected:
- • TCU CCB and WCB Tag and Data RAMs, TCU SMID: 7-10.
- • TBU MTLB Tag and Data RAMs, TBU SMID: 7-8.

Correction occurs irrespective of whether an attempt is made to report the errors as corrected or uncorrected in the FMU error records, which FMU_ERR<n>CTLR.CE_EN controls. See:

- • 4.4.8 Correctable Error enable on page 119.
- • 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.

Errors that are injected into these correctable RAMs are injected into only the FMU path and therefore:

- • The corresponding errors are not reported in the 3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register on page 83 and 3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register on page 93 MMU‑600 PV RAS registers. See also 4.6.2 RAM fault reporting on page 144.
- • Do not lead to the corrective action of invalidating and refetching those RAM entries.


You can use this mechanism to test the software that is handling the FMU error records without disturbing the software that is handling the MMU‑600 PV RAS registers. However, the mechanism is distinguishable from a true error occurring in these RAMs.

For errors that are injected into these correctable RAMs using 3.6.4 TCU_ERRGEN, TCU Error Generation register on page 85 and 3.9.4 TBU_ERRGEN, TBU Error Generation register on

page 94, no attempt is made to report them in the FMU. These errors are only reported in the MMU‑600 PV RAS registers. This enables the RAS software to be tested independently of the FMU, but is distinguishable from a true error occurring in these RAMs.

Errors that are injected into other RAMs are never corrected and the MMU‑600 PV RAS registers never report them. The MMU‑600 PV RAS registers are also unable to inject errors into these RAMs. An attempt is made to report these errors as Uncorrected Errors in the FMU error records. Because the error injection did not corrupt the RAM data, the SMMU continues to function correctly. However, a real error in these RAMs might lead to the outcomes that the beginning of

- 4.4.4 Safety Mechanism table on page 109 describes.

Errors that are injected into the Ping Ack mechanism (TCU SMID:12) do not give software the opportunity to choose the block ID that is reported in FMU_ERR<n>STATUS.BLKID if it was a real error. This ﬁeld is unchanged.

Attempting to inject an error into the F‑Channel has no eﬀect. To simulate such an error, send a ping message to inject errors along the path that has the ADB/ LPD that is connected to the required F‑Channel. See Directed ping in 4.4.7 Ping mechanism on page 116.

There might be multiple components along the ping path that have an F‑channel interface. If the ping packet is marked as tbu_inject_error, each component attempts to raise an error. Usually, the ping packet travels suﬃciently quickly that the ﬁrst component is recorded in ERR1STATUS.IERR, and subsequent components cause an overﬂow in ERR1STATUS. Subsequent components can only be observed if software is fast enough to acknowledge the ERR1STATUS register before the subsequent components report an error.

Do not inject error into the FMU clock gating override Safety Mechanism.

- 4.4.5 Software interaction


Aspects of how software interacts with the MMU-600AE FMU include initialization, interrupt handling, FMU idle status and power management.

###### 4.4.5.1 Initialization

The initialization routine can use the FMU_ERRIDR register to understand the number of implemented Error Records.

It can further iterate over the FMU_ERR<n>FR, Error Record Feature Registers to understand the capabilities of each Error Record. See 4.4.9.1.1 FMU_ERR<n>FR, Error Record Feature Register on page 122.

All Safety Mechanisms are enabled on reset which can lead to errors being logged in the Error Records. If the system does not support or does not want to check a particular safety feature, then the software should disable that Safety Mechanism. To disable a Safety Mechanism, write to the 4.4.9.1.8 FMU_SMEN, Safety Mechanism Enable Register on page 134 and specify the block ID and the Safety Mechanism ID.

Because most of the safety mechanisms are enabled by default on startup, then they might have already recorded some errors. The initialization routine should analyze the FMU_ERR<n>STATUS registers to deal with any errors that have been found so far. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.

Clear all logged errors by writing ones to the bits that are asserted in the 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.

To enable reporting of errors through the interrupts FHI and/or ERI, write one to FMU_ERR<n>CTLR.FI and FMU_ERR<n>CTLR.UI. See 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.

###### 4.4.5.2 Interrupt handler

When an interrupt, ERI or FHI, is received, the interrupt handling software can identify the Error Record ID, or the MMU block, that is reporting the error by reading the FMU_ERRGSR register.

See 4.4.9.1.4 FMU_ERRGSR, Error Group Status Register on page 129. FMU_ERRGSR[N] being one indicates that Error Record N holds a valid error. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125. The FMU_ERR<N>STATUS.IERR indicates the Safety Mechanism ID which reported the error. In the case where this block has reported more than one error of the same type to this Error Record, FMU_ERR<N>STATUS.OF is set to 1. In the case where this block attempts to report another error while it is still reporting an unacknowledged error, then see 4.4.9.1.3.1 Prioritized ERR<n>STATUS registers on page 128. When the recovery procedure is complete, the errors from this Error Record can be acknowledge by writing an appropriate value to FMU_ERR<n>STATUS. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125 for more information.

###### 4.4.5.3 FMU idle

The APB port to the FMU is designed to not introduce backpressure by deasserting pready. This measure prevents software lockup and makes the Error Records always accessible.

There are several operations which take multiple clock cycles to complete within the FMU. The FMU frees up the APB bus by asserting pready to complete the APB transaction. However, it might still be processing the previous request. When software writes one of the following registers inside the FMU, it is required to poll for the FMU_STATUS.IDLE == 1 before issuing another write to these registers. The APB writes, which require a poll of FMU_STATUS.IDLE == 1, are:

- • FMU_ERR<n>STATUS.
- • FMU_SMEN.
- • FMU_SMINJERR.
- • FMU_PINGNOW. 4.4.5.4 Power management The TBU LP_PD Q‑Channel interface can power down the TBU. Writing to the following registers generates messages to the remote TBU block:
- • 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.
- • 4.4.9.1.7 FMU_PINGNOW, Ping Now Register on page 132.
- • 4.4.9.1.8 FMU_SMEN, Safety Mechanism Enable Register on page 134.
- • 4.4.9.1.9 FMU_SMINJERR, Safety Mechanism Inject Error Register on page 135.


The software should be aware of the power state of the remote TBUs and does not initiate writes to these registers that are targeting a powered‑down TBU. If writes are initiated to the preceding registers, that are targeting a powered‑oﬀ TBU, the write does not come into eﬀect.

The following apply: FMU_ERR<n>STATUS

The write is ignored for all purposes. FMU_ERR<n>STATUS is unchanged.

FMU_PINGNOW The write is ignored for all purposes other than reading back the register. It does not send a PING packet and does not indicate that the FMU is non‑idle through FMU_STATUS.

###### FMU_SMEN

The write is ignored for all purposes. FMU_SMINJERR

The write is ignored for all purposes.

- 4.4.6 Lock and key mechanism The FMU registers are protected against inadvertent writes by a lock and key mechanism.


The FMU registers are in a locked state after reset. If the register ﬁle is locked, then any WriteAccess to any register other than 4.4.9.1.5 FMU_KEY, FMU Key Register on page 130 is ignored.

The register ﬁle is unlocked when a write to FMU_KEY occurs that satisﬁes all of the following:

- • Is Secure.
- • Is for 32 bits. That is, all write strobes.
- • The bottom 8 bits are 0xBE. The register ﬁle is locked again when a write occurs that satisﬁes all of the following:
- • Is a Secure write.
- • Is any width and any write strobes.
- • Is to any register except for FMU_KEY.


A write to the 4.4.9.1.5 FMU_KEY, FMU Key Register on page 130, when unlocked, leaves the register ﬁle unlocked only if the write satisﬁes the criteria for unlocking the register ﬁle. Otherwise, it locks the register ﬁle.

If the register ﬁle is unlocked, the FMU_KEY register reads as 0x00000BE. Otherwise, the FMU_KEY register reads as 0x00000000.

Non‑secure accesses never succeed and never aﬀect the locked state of the register ﬁle.

Some of the FMU registers are 64‑bit registers, but the APB interface width is 32 bits. When in unlocked state, the FMU allows for two consecutive writes to update the same 64‑bit register without requiring unlocking again before the second write. In this sequence, both the writes are Secure, with all write strobes to the same register, except that the second write is targeting the other half of that register.

For example, the following sequence would be successful in updating the register contents:

- 1. Secure write to FMU_KEY with data 0xBE, all write strobes asserted.
- 2. 32‑bit Secure write to FMU_ERR0CTLR[63:32] addr 0x0C, all write strobes asserted.
- 3. 32‑bit Secure write to FMU_ERR0CTLR[31:0] addr 0x08, all write strobes asserted.


This behavior is permitted to allow for the case when the APB interconnect splits a single 64‑bit register access and presents it to the FMU in any order.

###### 4.4.7 Ping mechanism

The MMU-600AE FMU provides a ping mechanism to detect and help debug network issues between the TCU and remote TBU blocks.

###### Background ping

The FMU provides a background ping mechanism to detect network issues between the TCU and the remote TBU blocks. Using the ping mechanism, the TCU sequentially sends a ping message over the AXI4‑Stream network to a remote TBU block, one at a time. It then starts a timer and expects a PING_ACK message back from that remote block. If PING_ACK is not received within the expected interval from the intended remote TBU block, the FMU indicates a PING_ACK timeout error.

The ping mechanism can help identify the following issues:

- • Permanent deadlock that permanent Stuck‑At Faults on valid and ready bits cause.
- • Congestion in the network that exceeds the FMU_PINGCTLR.ping_timeout_value.
- • Systematic issues in the network that cause misrouting of messages.
- • Connectivity issue of remote blocks to the TCU.


The background ping by MMU FMU can be enabled by writing to the 4.4.9.1.6 FMU_PINGCTLR, Ping Control Register on page 131. FMU_PINGCTLR.ping_timeout_value deﬁnes the timeout in the MMU FMU clock. (FMU_PINGCTLR.ping_interval_diﬀ + FMU_PINGCTLR.ping_timeout_value) deﬁnes the interval at which the FMU pings the next remote block, ping_interval. Figure 4-4: Ping mechanism parameters

ping_interval ping_interval

ping_timeout_value ping_interval_diff

Ping sent to TBU0

Ping sent to TBU1

Ping sent to TBU2

It is expected that the background ping using 4.4.9.1.6 FMU_PINGCTLR, Ping Control Register on page 131 and directed ping using 4.4.9.1.7 FMU_PINGNOW, Ping Now Register on page 132 are used mutually exclusively. When background pings are enabled, do not write FMU_PINGNOW.enable = 1.

Before generating directed pings using the FMU_PINGNOW register, turn oﬀ background ping by setting FMU_PINGCTLR.enable = 0 and waiting long enough for the last PING_ACK to be returned.

When the FMU indicates a PING_TIMEOUT error, you can obtain the remote TBU block ID by the reading the 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.

To conserve operational power of the TCU, the TCU accepts the Q‑Channel handshake to enter low powerdown state, if requested by the clock controller.

When the TCU is in the low‑power clock gated state, it does not send background ping messages to the TBUs and does not report PING_ACK violations.

When the TCU is not in the low‑power clock gated state, the FMU resumes background pings.

###### Directed ping

The software can also send directed ping messages to a speciﬁc block using the 4.4.9.1.7 FMU_PINGNOW, Ping Now Register on page 132. This process can help debug PING_ACK violations that are received from background pings. Use the following recommended software procedure to initiate a directed software ping:

- 1. To disable background pings, write FMU_PINGCTLR.enable = 0.
- 2. To clear all ﬂags, write all zeros into the 4.4.9.1.7 FMU_PINGNOW, Ping Now Register on page 132.
- 3. To initiate a directed ping, write:

- a. FMU_PINGNOW.enable = 1.
- b. FMU_PINGNOW.ping_ack_received = 0.
- c. FMU_PINGNOW.blk_id of the TBU block to which the ping is to be sent.


- 4. Poll for FMU_PINGNOW.ping_ack_received == 1.
- 5. Optionally, set Error Injection bits to test TBU or TCU integration, software, or both.


The PINGNOW feature can be used to send an erroneous packet from the TCU to a targeted TBU or from a targeted TBU to the TCU. Using this feature enables the TCU or TBU integrator to verify the AXI4‑Stream and F‑Channel connections between the TBUs and the TCU.

Injecting an error on a TCU ping message and on the subsequent TBU PING_ACK message causes mismatches along the PING/PING_ACK route through the interconnect. After injecting a PINGNOW error, you can read the TCU Error Records and verify that the expected Safety Mechanism errors are reported along the PING or PING_ACK route, for example by the receiving block and by any ADB components along the path.

When writing to the FMU_PINGNOW register and FMU_PINGNOW.enable is written as 1:

- • A single ping is sent for each write to a present block.
- • If another ping is sent before a previous PING_ACK has been received, then:

- ◦ If sent to the same destination, then the ﬁrst ping back sets FMU_PINGNOW.ping_ack_received.
- ◦ If sent to a diﬀerent destination, then the ﬁrst PING_ACK is silently discarded if or when received because it does not match the programmed FMU_PINGNOW.blk_id.


- • An attempt to send a ping to a not-present block does not launch a ping and FMU_PINGNOW.ping_ack_received does not go HIGH.
- • If FMU_PINGNOW.tcu_inject_error == 1, an error is injected on the outgoing PING packet on the TCU->TBU interface. The receiving TBU block and the ADB, if present, detect the erroneous payload and report it as a fault.
- • If FMU_PINGNOW.tbu_inject_error == 1, an error is injected on the outgoing PING_ACK packet by the TBU on the TBU->TCU interface. The receiving TCU block and the ADB, if present, detect the erroneous payload and report it as a fault.


- 4.4.8 Correctable Error enable By default, the FMU reports all errors as Uncorrectable Errors.


The RAMs are protected using CRC code. Some RAMs that act as caches on a CRC error perform invalidation and refetch of that cache line. The following RAMs act as caches:

- • TCU CCB Tag RAM.
- • TCU CCB EntryTag RAM.
- • TCU WCB Tag RAM.
- • TCU WCB Entry RAM.
- • TBU MTLB Tag RAM.
- • TBU MTLB Entry RAM.

Because this error is always corrected, CRC errors from these RAMs can be considered to be reported as correctable.

To program the FMU to consider these RAM errors as correctable for reporting purposes, set FMU_ERR<n>CTLR.CE_EN = 1. These Correctable Errors then update FMU_ERR<n>STATUS.CE. Whatever the value of FMU_ERR<n>CTLR.CE_EN, the errors are always corrected for these RAMs.

See:

- • 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.
- • 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.


###### 4.4.9 Fault Detection and Control Programmer's View

The MMU-600 memory map that is used to address the legacy MMU functional logic is unchanged in MMU-600AE. MMU-600AE uses a separate and independent memory map for the Fault Detection and Control (FDC) Programmer's View (PV).

###### MMU-600 programmer's model

For a summary of the main MMU-600 memory map and registers, see 3. Programmer's model on page 60.

###### MMU-600AE FDC memory map and PV

For a summary of the MMU-600AE FDC memory map and PV, see 4.4.9.1 FMU Programmer's View registers on page 121.

###### General programmer's model behavior

The following information applies to both the FMU and functional MMU-600 registers:

- • The base address is not ﬁxed, and can be diﬀerent for any particular system implementation. The oﬀset of each register from the base address is ﬁxed.
- • Do not attempt to access reserved or unused address locations. Attempting to access these locations can result in UNPREDICTABLE behavior.
- • Unless otherwise stated in the accompanying text:

- ◦ Do‑Not‑Modify UNDEFINED register bits.
- ◦ Ignore UNDEFINED register bits on reads.
- ◦ All register bits are reset to 0 by a system or Cold reset.


- • Access type is described as follows:

- ◦ Read and Write (RW)
- ◦ Read‑Only (RO)
- ◦ Write‑Only (WO)
- ◦ Read-As-Zero (RAZ)
- ◦ Writes Ignored (WI)


- • Bit positions that are described as reserved are:


- ◦ RAZ/WI in an RW register
- ◦ RAZ in a RO register
- ◦ WI in a WO register


The MMU-600 registers are accessed using the PROG APB4 completer interface on the TCU, and cannot be accessed directly through any other completer interfaces.

The FMU PROG APB4 interface is 32 bits wide. Strobed write is also not supported.

The FMU PROG APB4 port permits only Secure access to the FMU. This is performed by checking the PPROT[1] signal during an access. If the access fails the security check, then the access behaves as RAZ/WI.

###### 4.4.9.1 FMU Programmer's View registers

MMU-600AE includes Fault Management Unit (FMU) registers for the Error Records, ping mechanism, and Safety Mechanisms.

For a summary of the main MMU-600 memory map and registers, see 3. Programmer's model on page 60.

The MMU block IDs table in 4.4.3.2 Functional PV and FMU PV on page 108 shows the block ID, n, that corresponds to each component instance.

The system attempts to record errors in error record n, where n corresponds to the ID in the MMU block IDs table.

You can conﬁgure the number of TBUs that the FMU supports by using the TCUCFG_FUSA_TBU_FAULT_WIRE_COUNT parameter. See the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual. If you conﬁgure the number of connected TBUs to be greater than or equal to 54, then MMU-600AE uses 64KB page sizes instead of the normal 4KB page sizes, and this aﬀects the address oﬀset.

The following table shows the FMU PV registers, and their oﬀsets, depending on the number of connected TBUs, which can be one of the following:

- • Less than 54.
- • Greater than or equal to 54.


- Table 4-8: FMU PV register summary


|Oﬀset|Oﬀset|Name|Type|Width|Description|
|---|---|---|---|---|---|
|Number TBUs < 54|Number TBUs ≥ 54|Name|Type|Width|Description|
|0x000 + 64 x n|0x000 + 64 x n|FMU_ERR<n>FR|RO|64-bit|Error Record Feature Register per MMU block. See 4.4.9.1.1 FMU_ERR<n>FR, Error Record Feature Register on page 122.|
|0x008 + 64 x n|0x008 + 64 x n|FMU_ERR<n>CTLR|RW|64-bit|Error Record Control Register per MMU block. See 4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.|
|0x010 + 64 x n|0x010 + 64 x n|FMU_ERR<n>STATUS|RW|64-bit|Error Record Primary Status Register per MMU block. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.|
|0xE00|0xE000|FMU_ERRGSR|RO|64-bit|Error Group Status Register. See 4.4.9.1.4 FMU_ERRGSR, Error Group Status Register on page 129.|
|0xEA0|0xE0A0|FMU_KEY|RW|32-bit|FMU Key Register. See 4.4.9.1.5 FMU_KEY, FMU Key Register on page 130.|


|Oﬀset|Oﬀset|Name|Type|Width|Description|
|---|---|---|---|---|---|
|Number TBUs < 54|Number TBUs ≥ 54|Name|Type|Width|Description|
|0xEA4|0xE0A4|FMU_PINGCTLR|RW|32-bit|Error Ping Control Register. See 4.4.9.1.6 FMU_PINGCTLR, Ping Control Register on page 131.|
|0xEA8|0xE0A8|FMU_PINGNOW|RW|32-bit|Error Ping Now Register. See 4.4.9.1.7 FMU_PINGNOW, Ping Now Register on page 132.|
|0xEB0|0xE0B0|FMU_SMEN|WO|32-bit|Safety Mechanism Enable Register. See 4.4.9.1.8 FMU_SMEN, Safety Mechanism Enable Register on page 134.|
|0xEB4|0xE0B4|FMU_SMINJERR|WO|32-bit|Safety Mechanism Inject Error Register. See 4.4.9.1.9 FMU_SMINJERR, Safety Mechanism Inject Error Register on page 135.|
|0xEC0|0xE0C0|FMU_PINGMASK|RW|64-bit|FMU Ping Mask Register. See 4.4.9.1.10 FMU_PINGMASK, Ping Mask Register on page 137.|
|0xF00|0xE100|FMU_STATUS|RO|32-bit|Fault Management Status Register. See 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138.|
|0xFC8|0xE1C8|FMU_ERRIDR|RO|32-bit|Error Record ID Register. See 4.4.9.1.12 FMU_ERRIDR, Error Record ID Register on page 139.|


4.4.9.1.1 FMU_ERR<n>FR, Error Record Feature Register

This register deﬁnes which of the common architecturally deﬁned MMU-600AE features are implemented and, of the implemented features, which are software programmable.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

64-bit Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0x000 + 64 x n Type

RO

Reset value

0x00000000000000A2

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-5: Error Record Feature Register bit assignments

|63| | | | | | |32|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|


|31| | | | |8|7 6 5 4|7 6 5 4|3 2 1 0|3 2 1 0|
|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|FI|UI| |ED|


Reserved

- Table 4-9: Error Record Feature Register bit descriptions


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[63:8]|-|Reserved.|Res0|All zeros|
|[7:6]|FI|Fault Handling Interrupt. Feature is controllable using ERR<n>CTLR.FI.|RO|2'b10|
|[5:4]|UI|Error Recovery Interrupt for Uncorrected Errors. Feature is controllable using ERR<n>CTLR.UI.|RO|2'b10|
|[3:2]|-|Reserved.|Res0|2'b00|
|[1:0]|ED|Error reporting and logging. Feature is controllable using ERR<n>CTLR.ED.|RO|2'b10|


4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register This MMU-600AE FMU register controls what interrupt types are handled. For this error record:

- • FMU_ERR<n>CTLR.ED controls whether the record logs an error.
- • FMU_ERR<n>CTLR.CE_EN controls whether correctable errors are reported as uncorrectable.
- • FMU_ERR<n>CTLR.FI and FMU_ERR<n>CTLR.UI control the interrupts that are sent when the error record is reporting an error, as indicated by FMU_ERR<n>STATUS.V == 1. A change to these ﬁelds asserts or deasserts the level‑sensitive interrupt as appropriate.


###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

64

Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0x008 + 64 x n

Type

RW

Reset value

0x0000000000000001

###### Usage constraints

This register is only reset by the fmu_aresetn signal. See 4.4.2.1 Reset on page 107 in 4.4.2 Error signaling from the FMU on page 107.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 4-6: Error Record Control Register bit assignments

|63| | | | | | |32|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|


|31| | | | | |4|3 2 1 0|3 2 1 0|3 2 1 0|3 2 1 0|
|---|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| | | | |


FI UI

CE_EN ED

###### Table 4-10: Error Record Control Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[63:4]|-|Reserved.|Res0|All zeros|
|[3]|FI|Fault Handling Interrupt (FHI) enable.<br><br>This controls whether an FHI is generated for all detected and logged (FMU_ERR<n>CTLR.ED == 1) errors that are reported through this error record. That is:<br><br>• Correctable errors, whether reported as CEs or UEs. See 4.6.2 RAM fault reporting on page 144.<br>• Uncorrectable errors.<br>|RW|1'b0|


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[2]|UI|Error Recovery Interrupt (ERI) enable.<br><br>This controls whether an ERI is generated for all detected, logged (FMU_ERR<n>CTLR.ED == 1), RAS errors reported through this error record as UEs. That is:<br><br>• Correctable errors that are reported as uncorrectable (FMU_ERR<n>CTLR.CE_EN == 0). See 4.6.2 RAM fault reporting on page 144.<br>• Uncorrectable errors.<br><br><br>Note: An error that is reported as a UE might generate both an ERI and an FHI.|RW|1'b0|
|[1]|CE_EN|Correctable Error enable:<br><br>0 Treats Correctable Errors as Uncorrectable Errors (default).<br>1 Treats Correctable Errors and Uncorrectable Errors diﬀerently, and reports them separately.<br><br><br>See 4.4.8 Correctable Error enable on page 119.|RW|1'b0|
|[0]|ED|Error reporting and logging enable.|RW|1'b1|


4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register This MMU-600AE FMU register indicates information relating to the recorded errors. Poll the FMU_STATUS register after a write to this register to ensure that the eﬀect of the write is complete. FMU_STATUS.idle == 1 indicates that the eﬀect of a write is complete. See 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138. Until the write takes eﬀect, that is, FMU_STATUS.idle == 1 then:

- • The corresponding bit of FMU_ERRGSR might still report as 1.
- • Any interrupts caused by this record might still be asserted.
- • Any new error that occurs is treated as a second error recording on top of this error and causes an overﬂow to be set. See 4.4.9.1.3.1 Prioritized ERR<n>STATUS registers on page 128.
- • Any read of this register might return the old value, or if a new error has been recorded, then the newly recorded value.


###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

###### Width

64

###### Functional group

4.4.9.1 FMU Programmer's View registers on page 121

###### Address oﬀset

0x010 + 64 x n Type

See bit descriptions. Reset value

0x0000000000300000

###### Usage constraints

This register is only reset by the fmu_aresetn signal. See 4.4.2.1 Reset on page 107 in 4.4.2 Error signaling from the FMU on page 107. Do not write to an FMU_ERR<n>STATUS corresponding to a powered‑oﬀ block. See 4.4.5.4 Power management on page 115. The V, UE, CE, and OF ﬁelds in this register are write‑one‑to‑clear, but there is an interaction between the bits. A write to this register is ignored, unless after the write, the V, UE, OF, and CE ﬁelds are all zero. That is, either the:

- • Field was zero to begin with.
- • Write‑one‑to‑clear cleared the ﬁeld to zero. The CE ﬁeld is a 2‑bit ﬁeld but only ever reports 0b00 or 0b10. The value that is written to CE[0] is IGNORED for all purposes. All other named ﬁelds are read‑only and any value that is written to them is IGNORED for all purposes.


###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-7: Error Record Primary Status Register bit assignments

|63| | | | |40|39|32|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|BLKID|BLKID|


|31 30 29 28|31 30 29 28|31 30 29 28|31 30 29 28|27 26 25 24|27 26 25 24|27 26 25 24|23 22 21 20|23 22 21 20|19 16|15|8|7|0|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| | | | | | |CE|Res|UET|Reserved|IERR|IERR|SERR|SERR|


Reserved

OF

Reserved UE V Reserved

###### Table 4-11: Error Record Primary Status Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[63:40]| |Reserved.|Res0|All zeros|
|[39:32]|BLKID|If a PING_ACK timeout error occurs, this ﬁeld indicates the block ID of the remote MMU block that caused the error.<br><br>Only valid for Error Record 0.<br><br>Only valid when FMU_ERR<n>STATUS.V == 1 and FMU_ERR<n>STATUS.IERR == 8'd12. See 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.<br><br>This ﬁeld is not updated when a PING_ACK timeout error is reported as a result of a software error injection using the 4.4.9.1.9 FMU_SMINJERR, Safety Mechanism Inject Error Register on page 135.|RO|All zeros|
|[31]|-|Reserved.|RO|1'b0|
|[30]|V|Status Register valid.<br><br>This bit is set if the record represents that one or more errors have occurred.<br><br>If this bit is clear, then all other ﬁelds except for FMU_ERR<n>STATUS.SERR are UNKNOWN.<br><br>For write behavior, see the Usage constraints for this register.|RW|1'b0|
|[29]|UE|Uncorrected Error.<br><br>If FMU_ERR<n>STATUS.V == 1 and FMU_ERR<n>STATUS.UE == 1 then one or more uncorrected errors occurred.<br><br>For write behavior, see the Usage constraints for this register.|RW|1'b0|
|[28]|-|Reserved.|Res0|1'b0|


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[27]|OF|Record has overﬂowed.<br><br>Valid if FMU_ERR<n>STATUS.V==1, else UNKNOWN.<br><br>For write behavior, see the Usage constraints for this register.<br><br>See 4.4.9.1.3.1 Prioritized ERR<n>STATUS registers on page 128.|RW|1'b0|
|[26]|-|Reserved.|Res0|All zeros|
|[25:24]|CE|Corrected Error bit:<br><br>2'b00 No errors. 2'b10 One or more errors were corrected.<br><br>Valid if FMU_ERR<n>STATUS.V == 1, else UNKNOWN.<br><br>For write behavior, see the Usage constraints for this register.|RW|2'b00|
|[23:22]|-|Reserved.|RO|2'b0|
|[21:20]|UET|Uncorrected Error type.<br><br>These bits read as:<br><br>2'b11 When FMU_ERR<n>STATUS.V == 1 and FMU_ERR<n>STATUS.UE == 1. UNKNOWN Otherwise.|RO|2'b11|
|[19:16]|-|Reserved.|RO|All zeros|
|[15:8]|IERR|Safety Mechanism ID code.<br><br>See Table 4-7: Safety Mechanisms on page 110 for Safety Mechanism ID encodings.<br><br>Valid if FMU_ERR<n>STATUS.V == 1, else UNKNOWN.|RO|8'd0|
|[7:0]|SERR|Reads as zero if FMU_ERR<n>STATUS.V == 0.<br><br>Otherwise, reads as 1.|RO|8'd0|


4.4.9.1.3.1 Prioritized ERR<n>STATUS registers When a CE (Correctable Error) is followed by a UE (Uncorrectable Error), the following occurs:

- 1. The status registers are updated to reﬂect the Safety Mechanism ID of the Uncorrectable Error.
- 2. The UE is set along with the CE bit.
- 3. OF (Overﬂow) is not set in this case. OF is set only when either:


- • UE is set and another Uncorrectable Error is received.
- • CE is set and another Correctable Error is received.


MMU-600AE has separate UE and CE pipelines to avoid head of line UE blocking by a CE.

- 4.4.9.1.4 FMU_ERRGSR, Error Group Status Register This register shows the status of the MMU-600AE Error Records.


If an error record is being acknowledge by a write to 4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125, then while FMU_STATUS.idle == 1, the corresponding bit in this register might read as 1. Bit n in this register corresponds to ERR<n>STATUS.V in error record n.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

###### Width

64

Functional group

Placeholder Address oﬀset

0xE00 when there are fewer than 54 TBUs or 0xE000 when there are 54 or more TBUs Type

RO

Reset value

See individual bit resets.

###### Usage constraints

This register is only reset by the fmu_aresetn signal. See 4.4.2.1 Reset on page 107 in 4.4.2 Error signaling from the FMU on page 107.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-8: Error Group Status Register bit assignments

63 32

S

31 0

S

###### Table 4-12: Error Group Status Register bit descriptions

|Bits|Name|Description|
|---|---|---|
|[63:0]|S|Indicates the status of Error Record n:<br><br>0 The Error Record is not reporting any errors.<br>1 The Error Record is reporting one or more errors.<br><br><br>Records corresponding to non‑existent TBUs Read-As-Zero.|


4.4.9.1.5 FMU_KEY, FMU Key Register

Writing the correct key to this MMU-600AE register enables the next write to any other writable register to succeed.

See 4.4.6 Lock and key mechanism on page 115 for more information about the lock‑key mechanism.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

32

Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEA0 when there are fewer than 54 TBUs or 0xE0A0 when there are 54 or more TBUs.

This register reads as 0 if the register ﬁle is locked. This register reads as 0xBE if the register ﬁle is unlocked.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-9: FMU Key Register bit assignments

|31| | | | |8|7|0|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|KEY|KEY|


- Table 4-13: FMU Key Register bit descriptions


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:8]| |Reserved.|Res0|All zeros|
|[7:0]|KEY|The required key to write to FMU registers.|RW|8h'00|


4.4.9.1.6 FMU_PINGCTLR, Ping Control Register This MMU-600AE register conﬁgures the error ping timing interval.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

32

Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEA4 when there are fewer than 54 TBUs or 0xE0A4 when there are 54 or more TBUs.

Do not change FMU_PINGMASK while background ping is enabled, that is, FMU_PINGCTLR.enable == 1.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-10: Ping Control Register bit assignments

|31| | |16|15| |4|3 1 0|3 1 0|
|---|---|---|---|---|---|---|---|---|
|ping_interval_diﬀ|ping_interval_diﬀ|ping_interval_diﬀ|ping_interval_diﬀ|ping_timeout_value|ping_timeout_value|ping_timeout_value|Reserved| |


enable

###### Table 4-14: Ping Control Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:16]|ping_interval_diﬀ|Equal to (ping_interval − ping_timeout_value) in SMMU clock cycles.|RW|All zeros|
|[15:4]|ping_timeout_value|Timeout threshold value for ping timeouts in SMMU clock cycles. The minimum supported value is 4.|RW|All zeros|
|[3:1]|-|Reserved.|Res0|All zeros|
|[0]|enable|Enables the TCU ping engine. The TCU sends ping messages to each remote component, and expects a PING_ACK back within the speciﬁed timeout. If the PING_ACK is not received within the speciﬁed timeout, then the TCU records this situation as an error. The TCU sequentially moves to the next block and sends another ping message after ping_interval. If pings are enabled, then the setting in the 4.4.9.1.10 FMU_PINGMASK, Ping Mask Register on page 137 must unmask at least one TBU block.|RW|1'b0|


4.4.9.1.7 FMU_PINGNOW, Ping Now Register

This MMU-600AE register speciﬁes the remote SMMU block to send the ping request to, and monitors whether that block has acknowledged the ping.

Poll the FMU_STATUS register after a write to this register to ensure that the eﬀect of the write is complete. FMU_STATUS.idle == 1 indicates that the eﬀect of a write is complete. See 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138.

For more information on the use of this register, see Directed ping in 4.4.7 Ping mechanism on page 116.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

32

Functional group

Placeholder Address oﬀset

0xEA8 when there are fewer than 54 TBUs or 0xE0A8 when there are 54 or more TBUs. Type

RW

Reset value

0x000000000

###### Usage constraints

Do not send a PING to a powered‑oﬀ block. See 4.4.5.4 Power management on page 115.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 4-11: Ping Now Register bit assignments

|31| | | |12|11 10 9 8|11 10 9 8|11 10 9 8|11 10 9 8|7|0|
|---|---|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved| | | | |block_id|block_id|


tbu_inject_error enable tcu_inject_error ping_ack_received

###### Table 4-15: Ping Now Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:12]| |Reserved|Res0|All zeros|


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[11]|tbu_inject_error|1 Inject an error on the PING_ACK response packet that is sent from the remote SMMU block to the FMU. This action causes errors along the route of the PING_ACK through the interconnect. The presence of errors helps to conﬁrm that the interconnect path from the speciﬁed remote SMMU block to the FMU has been properly connected.|RW|1'b0|
|[10]|tcu_inject_error|1 Inject an error on the PING data packet that is sent from the FMU to the remote<br><br>SMMU block. This action causes errors along the route of the PING route through the interconnect. The presence of errors helps to conﬁrm that the interconnect path from the FMU to the speciﬁed remote SMMU block has been properly connected.|RW|1'b0|
|[9]|ping_ack_received|Indicates whether a PING_ACK has been received:<br><br>0 PING_ACK has not been received.<br>1 PING_ACK has been received from the SMMU block that was pinged.<br>|RW|1'b0|
|[8]|enable|Ping enable:<br><br>0 Does not initiate a ping. Allows software to clear the status of this register without initiating another ping.<br>1 Initiates a ping to the SMMU block speciﬁed in FMU_PINGNOW.block_id.<br>|RW|1'b0|
|[7:0]|block_id|Block identiﬁer. Sends a ping request to the speciﬁed SMMU block. See Table 4-6: MMU block IDs on page 109 for block ID encodings.|RW|8'd0|


4.4.9.1.8 FMU_SMEN, Safety Mechanism Enable Register

This MMU-600AE register enables or disables particular Safety Mechanisms inside a speciﬁed SMMU block. All Safety Mechanisms in the SMMU blocks are enabled at reset.

Poll the FMU_STATUS register after a write to this register to ensure that the eﬀect of the write is complete. FMU_STATUS.idle == 1 indicates that the eﬀect of a write is complete. See 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

###### Width

32

###### Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEB0 when there are fewer than 54 TBUs or 0xE0B0 when there are 54 or more TBUs. Type

WO

Reset value

See individual bit resets.

###### Usage constraints

This feature cannot be used for the following:

- • BLK = TCU, SMID = 0.
- • BLK = Fault channel block.
- • BLK = TBU, SMID = 0.


Do not attempt to enable or disable a powered‑oﬀ block. See 4.4.5.4 Power management on page 115.

If a block is powered‑oﬀ and then powered‑on again, the enabled state of the Safety Mechanism returns to the default reset state. See 4.4.4.1 Enabling and disabling a Safety Mechanism on page 111 in 4.4.4 Safety Mechanism table on

page 109.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 4-12: Safety Mechanism Enable Register bit assignments

|31|24|23|16|15|8|7|1 0|1 0|
|---|---|---|---|---|---|---|---|---|
|SMID|SMID|Reserved|Reserved|BLK|BLK|Reserved|Reserved| |


EN

###### Table 4-16: Safety Mechanism Enable Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:24]|SMID|Safety Mechanism identiﬁer. See Table 4-7: Safety Mechanisms on page 110 for Safety Mechanism ID encodings.|WO|X|
|[23:16]|-|Reserved.|Res0|8'd0|
|[15:8]|BLK|Block identiﬁer. See Table 4-6: MMU block IDs on page 109.|WO|X|
|[7:1]|-|Reserved.|Res0|7'd0|
|[0]|EN|Safety Mechanism enable.|WO|X|


4.4.9.1.9 FMU_SMINJERR, Safety Mechanism Inject Error Register

This MMU-600AE register injects one error into the speciﬁed Safety Mechanism inside an SMMU block. Only one error is injected, and no explicit clearing of this mechanism is required.

Poll the FMU_STATUS register after a write to this register to ensure that the eﬀect of the write is complete. FMU_STATUS.idle == 1 indicates that the eﬀect of a write is complete. See 4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138.

For more information, see:

- • 4.4.4 Safety Mechanism table on page 109.
- • 4.4.4.2 Injecting an error into a Safety Mechanism on page 112.


###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

###### Width

32

###### Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEB4 when there are fewer than 54 TBUs or 0xE0B4 when there are 54 or more TBUs. Type

WO

Reset value

See individual bit resets.

###### Usage constraints

This feature cannot be used for the following:

- • BLK = TCU, SMID = 0.
- • BLK = Fault channel block.
- • BLK = TBU, SMID = 0. Do not attempt to inject an error into a powered‑oﬀ block. See 4.4.5.4 Power management on page 115.


###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-13: Safety Mechanism Inject Error Register bit assignments

|31|24|23|16|15|8|7|0|
|---|---|---|---|---|---|---|---|
|SMID|SMID|Reserved|Reserved|BLK|BLK|Reserved|Reserved|


- Table 4-17: Safety Mechanism Inject Error Register bit descriptions


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:24]|SMID|Safety Mechanism identiﬁer. See Table 4-7: Safety Mechanisms on page 110 for Safety Mechanism ID encodings.|WO|X|
|[23:16]|-|Reserved.|Res0|8'd0|
|[15:8]|BLK|Block identiﬁer. See Error Records.|WO|X|
|[7:0]|-|Reserved.|Res0|8'd0|


4.4.9.1.10 FMU_PINGMASK, Ping Mask Register This MMU-600AE FMU register conﬁgures the ping mask.

It is not necessary to mask oﬀ a powered‑oﬀ block in FMU_PINGMASK before powering it oﬀ. The SMMU automatically stops sending background pings to a powered‑oﬀ block as the block performs the powerdown handshake.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

64

Functional group

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEC0 when there are fewer than 54 TBUs or 0xE0C0 when there are 54 or more TBUs. Type

RW

Reset value

0x0000000000000000

###### Usage constraints

Do not change FMU_PINGMASK while background ping is enabled, that is, FMU_PINGCTLR.enable == 1.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-14: Ping Mask Register bit assignments

63 32

ping_mask

31 0

ping_mask

- Table 4-18: Ping Mask Register bit descriptions


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[63:0]|ping_mask|Ping mask. Bit position corresponds to the MMU block ID.<br><br>To make the FMU skip speciﬁc TBU blocks while generating background ping messages, write a 1 to the corresponding bit:<br><br>Bit [0] TCU<br>Bit [1] Fault Channel<br>Bit [2] TBU0 Bit [2+n] TBUn<br><br><br>For unpopulated MMU blocks, the corresponding bits have no eﬀect. The same applies to bit[0] and bit[1] because the FMU does not ping the TCU or the Fault Channel components.|RW|All zeros|


4.4.9.1.11 FMU_STATUS, FMU Status Register This MMU-600AE register monitors whether or not the FMU is idle.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

32

Placeholder Address oﬀset

0xF00 when there are fewer than 54 TBUs or 0xE100 when there are 54 or more TBUs. Type

RO

Reset value

0x000000001

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions. Figure 4-15: FMU Status Register bit assignments

|31| | | | | | |1 0|1 0|
|---|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved|Reserved| |


idle

- Table 4-19: FMU Status Register bit descriptions


|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:1]|-|Reserved.|Res0|All zeros|
|[0]|idle|Indicates whether the FMU is idle:<br><br>0 FMU is busy processing the previous command.<br>1 FMU is idle.<br>|RO|1'b1|


4.4.9.1.12 FMU_ERRIDR, Error Record ID Register This MMU-600AE register deﬁnes the highest numbered index of the Error Records in this group.

###### Conﬁgurations

This register exists in all FMU conﬁgurations.

###### Attributes

Width

32

4.4.9.1 FMU Programmer's View registers on page 121 Address oﬀset

0xEA4 when there are fewer than 54 TBUs or 0xE0A4 when there are 54 or more TBUs. Type

RO

Reset value

See individual bit resets.

###### Bit descriptions

The following ﬁgure and table show the bit assignments and bit descriptions.

###### Figure 4-16: Error Record ID Register bit assignments

|31| | |16|15| | |0|
|---|---|---|---|---|---|---|---|
|Reserved|Reserved|Reserved|Reserved|NUM|NUM|NUM|NUM|


###### Table 4-20: Error Record ID Register bit descriptions

|Bits|Name|Description|Type|Reset|
|---|---|---|---|---|
|[31:16]|-|Reserved.|Res0|All zeros|
|[15:0]|NUM|1 + highest numbered index of the Error Records in this group.|RO|2 + number of connected TBUs|


##### 4.5 Lock-step protection

The MMU-600AE logic is protected by redundant lock-step checking. The exceptions to this are:

- • The RAMs, which are shared
- • The internal AXI4-Stream interconnect, which uses full duplication The following ﬁgure shows the lock-step for the TCU.


###### Figure 4-17: TCU lock-step

inputs

F F

CRC

F F

CRC

aclk

aclk_fdc

Fault

we

we_fdc

aclk

aclk

aclk_fdc

RAM

addr

addr_fdc

TCU / TCU no_ram ClkGate rcg_clken Primary

TCU / TCU no_ram Redundant

ClkGate

rcg_clken_fdc

wdata

wdata_fdc

F F

rdata

rdata_fdc

aclk_gated

aclk_gated

aclk_gated_fdc

CRC CRC

F F

Fault

outputs

The lock-step has a standard temporal delay of two cycles, with RAM sharing and comparators. In the preceding ﬁgure, a circle with an X in the middle indicates a comparator. To save power, CRC is used to compress the outputs.

The entire noram hierarchy is duplicated, with the comparators instanced in the block top level. The clock gate and reset synchronizers must also be duplicated in the top level.

The clocking is also duplicated. To provide redundancy in the reset and clock trees, the primary (main) and checker (shadow) logic are clocked by a separate clock and separate reset. In the clock tree, if a branch of the reset fails in the primary domain, then the checker domain detects the failure. Similarly, if a branch of the reset fails in the checker domain, then the primary domain detects the failure.

###### 4.5.1 Comparators

The lockstep comparators consist of an XOR tree. The same parameterized comparator component is instanced throughout the design to promote uniformity and allow the implementation to be changed.

The comparators are known to be power hungry. Therefore, qualiﬁcation is used wherever possible so they only check the outputs when necessary. For example, an AXI bus comparator checks the data only when the valid bits are asserted. This methodology is necessary to:

- • Prevent ﬂagging on benign glitches when nothing is reading the bus.
- • Prevent false error from being asserted due to UNKNOWN values on the bus, from RAMs or from uninitialized datapath ﬂops.


###### 4.5.1.1 Comparator duplication option

The comparators can be duplicated by setting a parameter to aid in latent fault diagnostic coverage goals.

Duplicating the comparators provides passive latent fault coverage, preventing the need to achieve coverage through LBIST or software STL library means. The main trade-oﬀ is power and area, but partners should check timing results as well. The option adds one additional gate into the comparator paths.

To duplicate the comparators, set FUSA_COMP_DUP=1 in instantiation.

All comparators in MMU-600AE can be duplicated, including lockstep and CRC comparators.

###### 4.5.2 Non-resettable ﬂops

Non-resettable ﬂops from MMU-600 were made resettable in cases where false output comparator errors can occur due to non-deterministic states of those ﬂops.

###### 4.5.3 ResetLogic has been added to the MMU-600AE to guarantee a proper reset for lockstep logic.

For more information on reset assumptions and requirements related to lockstep logic and FuSa, see the FuSa clocks and resets section in the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.

###### 4.5.4 Error injectionThe FMU can be used to inject a fault into a ﬁxed input of the lockstep comparators.

The main purpose is to test connectivity and software. This is not meant to be an exhaustive test of the comparator XOR tree. For this purpose, the comparators can be duplicated by setting the FUSA_COMP_DUP parameter, as described in 4.5.1.1 Comparator duplication option on page 141.

##### 4.6 RAM protection

For RAM errors that can be corrected, independent of the FMU_ERR<n>STATUS.CE_EN bit, the legacy MMU-600 PV reports some extra information about the RAM fault.

- See 3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register on page 83 and 3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register on page 93.


For correctable and uncorrectable RAM faults, the FMU PV is limited to reporting the RAM in which the fault occurred.

Errors are corrected by invalidating the cache lines that are associated with the error. The address is not protected on MMU-600. MMU-600AE adds 8‑bit CRC protection on both RAM data and RAM address bits. The CRC code is created from the combination of the data and the address being written to. When a memory location is read, the hardware calculates the CRC code, based on the data that is read from the RAM and the address that is used to read the data. If this calculated CRC code does not match the CRC code that was stored with the data, a CRC fault is ﬂagged and correction is initiated, if applicable. The CRC protection scheme can detect the following errors:

- • All Single-Bit data Errors (SBEs).
- • All Double-Bit data Errors (DBEs) on data widths of 128 bits or less.
- • 99.6% of all DBEs on data widths greater than 128 bits.
- • 99.6% of all Multi-Bit data Errors (MBEs).
- • 99.6% of incorrect address errors.


###### Figure 4-18: RAM CRC read path

###### RAM Wrapper

###### NORAM

Way[N] Way[...]

… levels of hierarchy ...

Chip Enable

Way[0]

ramarb

Write Enable

Address

Address

| |CRC Generation|
|---|---|
| |CRC Generation|


RAM

CRC Compare

Data In (with CRC)

CRC Data Data Out (with CRC)

###### Figure 4-19: RAM CRC write path

|NORAM<br><br>|… levels of hierarchy ...<br><br>|ramarb<br><br>Address<br><br>Data<br><br>||CRC|Data|
|---|---|
| |
|---|---|
||CRC|Data|
|---|---|
| |
<br><br>|CRC Generation| |
|---|---|
|CRC Generation| |
<br><br>|
|---|
|
|---|
|Chip Enable|
|---|---|
|NORAM<br><br>|… levels of hierarchy ...<br><br>|ramarb<br><br>Address<br><br>Data<br><br>||CRC|Data|
|---|---|
| |
|---|---|
||CRC|Data|
|---|---|
| |
<br><br>|CRC Generation| |
|---|---|
|CRC Generation| |
<br><br>|
|---|
|
|---|
|Write Enable|
|NORAM<br><br>|… levels of hierarchy ...<br><br>|ramarb<br><br>Address<br><br>Data<br><br>||CRC|Data|
|---|---|
| |
|---|---|
||CRC|Data|
|---|---|
| |
<br><br>|CRC Generation| |
|---|---|
|CRC Generation| |
<br><br>|
|---|
|
|---|
|Address|
|NORAM<br><br>|… levels of hierarchy ...<br><br>|ramarb<br><br>Address<br><br>Data<br><br>||CRC|Data|
|---|---|
| |
|---|---|
||CRC|Data|
|---|---|
| |
<br><br>|CRC Generation| |
|---|---|
|CRC Generation| |
<br><br>|
|---|
|
|---|
|Data In (with CRC)|
|NORAM<br><br>|… levels of hierarchy ...<br><br>|ramarb<br><br>Address<br><br>Data<br><br>||CRC|Data|
|---|---|
| |
|---|---|
||CRC|Data|
|---|---|
| |
<br><br>|CRC Generation| |
|---|---|
|CRC Generation| |
<br><br>|
|---|
|
|---|
|Data Out (with CRC)|


| |RAM Wrapper<br><br>Way[N] Way[...]<br><br>Way[0]<br><br>RAM|
|---|---|
| |RAM Wrapper<br><br>Way[N] Way[...]<br><br>Way[0]<br><br>RAM|


###### 4.6.1 RAM fault correction

On the MMU-600, faulty Tag and Entry RAM caches are invalidated based on a parity error. On the MMU-600AE, these faulty caches are invalidated based on a CRC or parity error.

If the faulty RAM is acting as a buﬀer and not a cache, TCU Translation Request Buﬀer and TBU Write Buﬀer, the fault is not corrected. The uncorrected faulty data is used and can lead to some of the unpredictable eﬀects that 4.4.4 Safety Mechanism table on page 109 describes.

###### 4.6.2 RAM fault reporting

For the correctable RAMs, CRC or parity faults are always corrected by invalidating the cache entry and refetching:

- • TCU CCB and WCB Tag and Data RAMs, TCU SMID: 7-10.
- • TBU MTLB Tag and Data RAMs, TBU SMID: 7-8. An attempt is made to report a CRC or a parity fault as a correctable error in the MMU‑600 PV RAS registers. The PV RAS registers supply extra syndrome information for these errors in TCU_ERRSTATUS.IERR and TBU_ERRSTATUS.IERR. See 3.6.3 TCU_ERRSTATUS, TCU Error Record Primary Syndrome register on page 83 and 3.9.3 TBU_ERRSTATUS, TBU Error Record Primary Syndrome register on page 93.


An attempt is made to report a CRC failure as a correctable or uncorrectable error in the FMU error records. See 4.4.8 Correctable Error enable on page 119.

Parity failure, but not CRC failure, might detect rare Multi‑Bit Errors (MBEs). In which case, the error is only reported in the MMU‑600 PV RAS registers. However, the CRC catches 99.6% of all MBEs and therefore this situation is extremely rare.

Errors in all other RAMs are uncorrectable, and an attempt is made only to report them in the FMU error records. No extra syndrome information is available other than the SMID that identiﬁes the RAM.

The MMU‑600 uses faulty data from uncorrectable RAMs and this can lead to some of the unpredictable eﬀects that 4.4.4 Safety Mechanism table on page 109 describes.

For both correctable and uncorrectable faults, both real faults and faults that software injects diﬀer slightly. See 4.4.4.2 Injecting an error into a Safety Mechanism on page 112.

- 4.6.3 RAM fault control RAM CRC fault reporting can be enabled or disabled using the FMU_SMEN register.

This register only aﬀects reporting of a CRC error. It does not aﬀect the reporting of the legacy parity error in the MMU-600 PV.

- 4.6.4 RAM fault severity

You can select whether a RAM fault is reported to the Safety Island through either an ERI or FHI interrupt.

See 4.4 Fault Management Unit on page 105 for more information.

- 4.6.5 Address protection


Because the RAM is shared, address protection must consider the protection of address decoders within the RAM decoder macro itself. Otherwise, faults within the RAM macro address decoder can cause Common Mode failure (CMF).

The CRC code is created from the combination of the data and the address that is being written to. When a memory location is read, the hardware calculates the CRC code based on the data that is read from the RAM and the address that is used to read the data. If this calculated CRC code does not match the CRC code that was stored with the data, a CRC fault is ﬂagged. The RAM type determines whether the fault is corrected.

- • If the faulty RAM is a cache, correction is initiated by invalidating the failed cache location.
- • If the faulty RAM is a buﬀer, for example the TCU Translation Request Buﬀer or TBU Write Buﬀer, the fault is not corrected.


##### 4.7 External interface protection

All external bus interfaces are protected as deﬁned by the AMBA® Parity Extensions for point-topoint protection. These interfaces include the ACE-Lite, AXI4-Stream, and APB external interfaces.

The following ﬁgure shows the distribution of interface protection within the MMU-600AE. The ACE-Lite, AXI4-Stream, and APB external interfaces are shown as bidirectional orange arrows.

###### Figure 4-20: Interface protection distribution

Interrupts ACE-Lite Interrupts ACE-Lite Interrupts DTI-ATS

- Q-Channel-cg

P

- Q-Channel-pd Q-Channel-pd


ACE-Lite

Q-Channel-cg

Q-Channel-cg

P P

P

P

P P

SZ SZ

ADB

RAMs

RAMs

RAMs

TBU

TBU

TBU

Full duplication / parity Full duplication / parity Full duplication / parity

P

P

P

Q-Channel-pd

LPD_PD ADB LPD_CG

LPD_PD

ADB

LPD_CG

SZ SZ

RS RS

P

Full duplication / parity

FHI

P

ERI

RAMs

FMU TCU

FMU APB

P

P P P

P

Interrupts

ACE-Lite-DVM APB

Q-Channel-pd

|Legend:<br><br>AXI4-Stream interconnect protection (full duplication)<br><br>Parity protection<br><br>Interrupt protection (parity)<br><br>Interface protection (AMBA FuSa)<br><br>Interface protection (Arm LPI FuSa) P<br><br>Logic (duplication)<br><br>RAM (CRC)<br><br>AXI4-Stream interconnect<br><br>Sizer Register Slice Asynchronous Domain Bridge Low Power Distributor<br><br>SZ RS<br><br>ADB LPD|
|---|


The ADB and Register Slice do not support parity on the AXI4-Stream interface. Therefore, only full duplication on AXI4-Stream is supported in the example topology.

###### Point-to-point protection

Point-to-point protection is suﬃcient for wires and buﬀers that cannot cause multiple-bit faults. An example of an interconnect component that could cause multiple-bit faults is a switch. A single fault on a switch mux input can switch the wrong data, causing multiple bits to fail.

###### 4.7.1 ACE-Lite interface parity protection

MMU-600AE supports ACE‑Lite interface parity protection for point‑to‑point connections from MMU-600AE to another functionally safe IP or FuSa interconnect. If a parity fault is detected, and the safety mechanism is enabled, the FMU attempts to report an error in the FMU error records, potentially issuing an FHI and/or an ERI interrupt. The FMU attempts to report an error in a TBU or TCU error record. The MMU uses the faulty data.

If this protection is not required, it can be disabled through the MMU-600AE FMU PV.

###### Assumptions of use for FuSa purposes

Arm expects that:

- • MMU-600AE is directly connected to the far‑end IP with only wires and repeater buﬀers.
- • No complex logic gates, such as ADBs or cross bar switches, exist in the path because they could be a source of Multiple Bit Errors (MBEs).
- • The far‑end IP checks the parity bits that MMU-600AE generates.
- • The far‑end IP generates the incoming parity bits, as ACE-Lite interface parity protection in the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual describes.


###### 4.7.2 AXI4-Stream interface parity protection

MMU-600AE AXI4‑Stream interfaces support AMBA® Parity Extension protection for point‑to‑point connections from MMU-600AE to another FuSa IP or FuSa interconnect. If a parity fault is detected, and the safety mechanism is enabled, the FMU attempts to report an error in the FMU error records, potentially issuing an FHI and/or an ERI interrupt. The FMU attempts to report an error in a TBU or TCU error record. The MMU uses the faulty data.

If this protection is not required, it can be disabled through the MMU-600AE FMU PV.

###### Assumptions of use for FuSa purposes

Arm expects that:

- • MMU-600AE is directly connected to the far‑end IP with only wires and repeater buﬀers.
- • No complex logic gates, such as ADBs or cross bar switches, exist in the path because they could be a source of MBEs.
- • The far‑end IP checks the parity bits that MMU-600AE generates.
- • Far‑end IP generates the incoming parity bits, as AXI4-Stream interface parity protection in the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual describes.


###### 4.7.3 APB interface parity protection

MMU-600AE has two APB interfaces, one for the MMU‑600 PV register ﬁle, and the other to the FMU register ﬁle. MMU-600AE supports APB interface parity protection on both APB point-topoint connections from MMU-600AE to another FuSa IP or FuSa interconnect. If a parity fault is detected, and the safety mechanism is enabled, the FMU attempts to report an error in the FMU error records, potentially issuing an FHI and/or an ERI interrupt. The FMU attempts to report an error in a TBU or TCU error record. The MMU uses the faulty data.

If this protection is not required, it can be disabled through the MMU-600AE FMU PV. Disable this protection when using an interconnect that does not generate AMBA® parity.

Assumptions of use for FuSa purposes

Arm expects that:

- • MMU-600AE is directly connected to the far‑end IP with only wires and repeater buﬀers.
- • No complex logic gates, such as ADBs or cross bar switches, exist in the path because they could be a source of MBEs.
- • The far‑end IP checks the parity bits that MMU-600AE generates.
- • Far‑end IP generates the incoming parity bits, as APB interface parity protection in the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual describes.


###### 4.7.4 F-Channel

The Fault Channel (F-Channel) communicates faults from FuSa blocks and components to the FMU using a Fault Channel interface.

This interface consists of a REQ/ACK four-phase handshake protocol, similar to that of the QChannel interface, that is designed to work across asynchronous domains. The phases of this protocol are:

- 1. The freq signal originating from the APD or LPD (in the case of the MMU-600AE) is asserted.
- 2. The freq signal is held until the fack response is received from the FMU.
- 3. When the fack response is received, the FuSa block deasserts its freq signal.
- 4. When the freq signal is deasserted by the FuSa block, the FMU deasserts its fack signal.


- Table 4-21: F-Channel signals


|Signal|Origin|Destination|Description|
|---|---|---|---|
|freq|ADB/LPD|FMU|Request to report a fault to the FMU.|
|fack|FMU|ADB/LPD|Acknowledgment that the FMU received the fault.|
|freq_chk|ADB/LPD|FMU|Asynchronous redundancy.|
|fack_chk|FMU|ADB/LPD|Asynchronous redundancy.|


The following ﬁgure shows the F-Channel and FMU fault wire connections.

###### Figure 4-21: F-Channel and FMU fault wire connections

- Domain-A

LPD-A

TBU-B0

|ADB|
|---|


ADB

|ADB|
|---|


ADB

fmu_err_in[1]

freq[3] fack[3]

freq[5] fack[5]

- Domain-B


fmu_err_in[0]

TBU-A

freq[0] fack[0]

|ADB|
|---|


|ADB|
|---|


ADB

ADB

freq[1] fack[1]

freq[2] fack[2]

LPD-B

freq[4] fack[4]

TCU

TBU-B1

fmu_err_in[2]

The related parameter settings for these connections are:

- • TCUCFG_FUSA_FCHAN_COUNT=6.
- • TCUCFG_FUSA_TBU_FAULT_WIRE_COUNT=3.


For clarity, the freq_chk and fack_chk connections are not shown.

###### 4.7.5 Interrupt output protection

Each single interrupt output is protected with an inverted chk parity bit. The chk bit is launched on the same clock cycle as the interrupt bit that it is protecting.

This format is compatible with the Arm CoreLink™ GIC-600AE Generic Interrupt Controller interrupt input protection. If you do not require this protection, you can leave the output unconnected.

###### 4.7.6 Tie-oﬀ input protection

The TBU and TCU employ “tie‑oﬀ” or strap bits, which aﬀect the out‑of‑reset behavior of the MMU.

These bits are protected by inverted duplication, which means that port_chk[x:0] = ~port[x:0]. These tie‑oﬀ inputs are expected to be static during and after reset. These inputs can be:

- • Tied oﬀ.
- • Connected to fuses or straps.
- • Driven by other logic as required by the SoC.


- Example 4-1: TCU tie-oﬀ input


For the TCU tie-oﬀ input ecorevnum[3:0], the chk bits are ecorevnum_chk[3:0]. If ecorevnum[3:0] == 0b0010, ecorevnum_chk[3:0] must be 0b1101.

For multi‑bit ports, this protection is diﬀerent from parity.

###### 4.7.7 Connecting to unsafe interfaces

If a TBU or TCU is connected to an interface that is not safe, that is, the interface does not provide the *chk* or *fdc* signals, then you must comply with some guidelines.

The guidelines that you must comply with for unsafe interfaces are as follows:

- • No input signal must be ﬂoating. That is, X or Z.


- • Tie all other unused *chk* input signals, for example, *validchk and *wakeupchk to their inactive value, considering the appropriate polarity.
- • Tie all other unused *fdc* input signals to their original counterpart. For example, tie aclk_fdc to aclk.
- • Tie unused Freq and fmu_err_in to 0.
- • Set the TCUCFG_FUSA_FCHAN_COUNT and TCUCFG_FUSA_TBU_FAULT_WIRE_COUNT parameter to 1, if you are not using F‑Channel error reporting.
- • Set the TBUCFG_FUSA_DTI_FULL_DUP_PROT and TCUCFG_FUSA_DTI_FULL_DUP_PROT parameters to 0 for point‑to‑point protection if the DTI interface is unsafe.
- • Set the TCUCFG_FUSA_DISABLE_PQCHAN_PROT parameter to 1, if you do not have redundant P‑Channel or Q‑Channel protection.
- • Set the TCUCFG_FUSA_DISABLE_SYSCO_PROT parameter to 1, if you do not have redundant SYSCO interface protection, ACE interface.


##### 4.8 Integrating the TCU, TBU, LPD, PCIe ATS, and DTIAXI4-Stream interconnect

MMU-600AE supports the following options for protecting the DTI AXI4-Stream interfaces on the TCU and TBU:

Duplicated DTI AXI4-Stream interfaces The TCU and TBU have duplicated DTI AXI4-Stream ports which can be used to protect the interconnect by duplication. Like MMU-600, MMU-600AE provides AXI4-Stream components that the SoC integrator can use to build their own interconnect. A modiﬁed ADB that supports AXI4-Stream duplication is provided with the MMU-600AE, which is used when the duplicated interconnect crosses asynchronous boundaries.

Single DTI AXI4-Stream interface with protection The single DTI AXI4-Stream interface is protected by AMBA® parity point-to-point interface. The TCU and TBU blocks can be connected to an interconnect or other IP that supports AMBA® Parity Extensions for protecting point-to-point connections. When in this mode, MMU-600AE generates the parity for the interface outputs, checks the parity for interface inputs, and ﬂags a fault if there is mismatch.

If the DTI AXI4-Stream interconnect IP does not support protection, then the safety mechanism inside the TBU (SMID: 4) and TCU (SMID:2) should be disabled during system initialization to prevent errors from being reported for these interfaces.

When a TBU comes out of reset, it exchanges messages with the TCU. If the parity protection has not been disabled by that time, then it attempts to report an error:

- • For the TBU, in FMU error record (tbu_index + 2) with UE = 1, IERR = SMID:4
- • For the TCU, in FMU error record 0 with UE = 1, IERR = SMID:2.


However, because the TCU and TBU continue to use the messages, despite the parity error, then the TBU connects correctly. Software can then later disable the Safety Mechanism (TBU SMID:4, TCU SMID:2) and discard any of the parity errors that the connection messages generated.

For more information, see the FuSa integration section of the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.

##### 4.9 Q-Channel protection

This section describes the Q-Channel logic and connections. The following ﬁgure shows a top-level example of an MMU topology with AXI4-Stream interconnect connections.

###### Figure 4-22: MMU topology with AXI4-Stream interconnect connections

ACEL5-M ACEL5-S ACEL5-M ACEL5-S ACEL5-M ACEL5-S

top_4x1_fmeda

Domain A Domain B

###### TBU0 TBU1

###### TBU2

Register Slice

Register Slice

Switch 2x1

Switch 2x1

|ADB|
|---|


|ADB|
|---|


Domain C

|ADB|
|---|


|ADB|
|---|


ACEL5-M

###### TBU3

AXI4-S

ACEL5-S

Switch 4x1

DTI ATS

Switch 4x1

AXI4-S

ACEL5-DVM-M

TCU

APB

The following ﬁgure shows a top-level example of an MMU topology with corresponding QChannel connections.

###### Figure 4-23: MMU topology with corresponding Q-Channel connections

PD_QCH PD_QCH PD_QCH

###### top_4x1_fmeda

Domain A Domain B

|TBU1|
|---|


|TBU2|
|---|


|TBU0|
|---|


| | | | |
|---|---|---|---|
|LPD_CG|LPD_CG|LPD_CG|LPD_CG|
| | | | |


|Register Slice|
|---|


devreqn_chk[0]

|Register Slice|
|---|


devreqn[0]

devreqn_chk[1] devreqn[1]

CG_QCH LPD_CG

devreqn_chk[2]

devreqn[2]

|ADB|
|---|


|ADB|
|---|


Domain C

|ADB|
|---|


|ADB|
|---|


devreqn[0] devreqn_chk[0]

devqreqn_chk[1] devqreqn[1]

|TBU3|
|---|


devreqn[1]devreqn_chk[1]

devqreqn_chk[0]

devqreqn[0]

PD_QCH LPD_PD

devreqn[2]

LPD_CG

devreqn_chk[2]

devreqn_chk[2]

devreqn[2]

|TCU|
|---|


CG_QCH

PD_QCH

CG_QCH

PD_QCH

The Q-Channels are protected with additional AMBA® LPI speciﬁed redundant chk bits with reverse polarity. Due to the four-phase asynchronous nature of the Q-Channel, signals are checked individually. With four-phase handshaking, all assertions must be held until handshaking feedback is received. Therefore, transient assertions are treated as faults which are ﬁltered for reliability by the MMU-600AE protection logic. The protection logic prevents these faults from reaching mission mode logic and causing errors. Permanent or Stuck-At Faults (SAF) are detected and ﬂagged.

The following ﬁgure is a high-level block diagram of a Q-Channel example employed by the MMU-600AE blocks. The ﬁgure shows that qreqn and qreqn_chk are synchronized separately and then passed through redundant sig_prot blocks where the transient ﬁltering and the stuckat checker counters live. The Q-Channel outputs are passed to the external power controller (or internal MMU LPD) with a temporal delay of no more than two cycles. This variation is allowed by the Q-Channel AMBA® LPI extensions.

###### Figure 4-24: Q-Channel protection connections

|Lockstep device|qactive|
|---|---|
|Lockstep device| |
|Lockstep device|qdeny|
|Lockstep device|qacceptn<br><br>|qreqn|sig_prot|
|---|---|
| |sig_prot|
<br><br>|qreqn_fdc|sig_prot|
|---|---|
| |sig_prot|
<br><br>SYNC qreqn<br><br>qreqn_chk<br><br>SYNC<br><br>|
|Lockstep device|qactive_chk|
|Lockstep device|qdeny_chk|
|Lockstep device|qacceptn_chk|


| |Asynchronous Q-Channel controller|
|---|---|
| |Asynchronous Q-Channel controller|
| |Asynchronous Q-Channel controller|


- 4.9.1 Q-Channel signaling


The following ﬁgure shows the Q-Channel device and controller signal mappings, including the extra _chk signals.

###### Figure 4-25: Q-Channel device and controller signal mappings

|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|---|---|
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |
|Device<br><br>qactive_chk<br><br>qactive<br><br>qdeny_chk<br><br>qdeny<br><br>qacceptn_chk<br><br>qacceptn<br><br>qreqn_chk<br><br>qreqn| |


| |Clock or power controller<br><br>qreqn_chk<br><br>qreqn<br><br>qacceptn_chk<br><br>qacceptn<br><br>qdeny_chk<br><br>qdeny<br><br>qactive_chk<br><br>qactive|
|---|---|
| |Clock or power controller<br><br>qreqn_chk<br><br>qreqn<br><br>qacceptn_chk<br><br>qacceptn<br><br>qdeny_chk<br><br>qdeny<br><br>qactive_chk<br><br>qactive|
| |Clock or power controller<br><br>qreqn_chk<br><br>qreqn<br><br>qacceptn_chk<br><br>qacceptn<br><br>qdeny_chk<br><br>qdeny<br><br>qactive_chk<br><br>qactive|


Each Q-Channel signal bit has a corresponding _chk signal with inverted polarity.

###### 4.9.2 Q-Channel acceptance

The following ﬁgure shows the opposite polarity of the _chk signals during the Q-Channel entry, acceptance, and exit sequence.

Figure 4-26: Q-Channel acceptance

t1 t2 t3 t4 clk

t5

qreqn qreqn_chk

qacceptn

qacceptn_chk

qdeny qdeny_chk

Q_RUN Q_REQUEST Q_STOPPED Q_EXIT Q_RUN

###### 4.9.3 Q-Channel denial

The following ﬁgure shows the opposite polarity of the _chk signals during the Q-Channel denial sequence.

Figure 4-27: Q-Channel denial

t1 t2 t3 t4 clk

t5

qreqn qreqn_chk

qacceptn

qacceptn_chk

qdeny qdeny_chk

Q_RUN Q_REQUEST Q_DENIED Q_CONTINUE Q_RUN

###### 4.9.4 _chk signal timingThere is a hard timing requirement that the Stuck-At Fault (SAF) detection logic imposes.

The skew of the qreqn and qreqn_chk signals must be less than the maximum skew that the SAF detection logic allows.

###### Clock Ratio (CR)

Equal to (MMU clock frequency) / (channel controller clock frequency).

Implementation skew

Silicon skew due to asynchronous clock domain crossings or other factors. Temporal delay skew

Skew between lock-step primary and redundant logic blocks.

Because the MMU-600AE SAF detector counts to 64 before ﬂagging an SAF, the permitted skew is calculated as follows:

• Maximum skew allowed = 64 / CR

###### Example 4-2: Q-Channel skew calculation

- • MMU clock frequency = 1000MHz
- • Q-Channel frequency = 125MHz Based on these frequencies, then:
- • CR = (MMU clock frequency)/(channel controller clock frequency) = 1000MHz / 125MHz = 8.
- • Maximum skew allowed = 64 / CR = 64 / 8 = 8 cycles.


Therefore, the system integrator is allowed eight cycles for implementation skew and temporal delay skew that originate from the SoC Q-Channel controller.

- 4.9.5 Transient faults The following ﬁgure shows the normal situation with no fault.


###### Figure 4-28: Normal assertion of qreqn and qreqn_chk signals

clk

QREQn

QREQn_chk

QREQn_int

|63|62|61|63|
|---|---|---|---|


Count

Error

The following ﬁgure shows how a transient fault on the qreqn signal is ﬁltered.

###### Figure 4-29: Transient fault on the qreqn signal

clk

QREQn

QREQn_chk

QREQn_int

|63|62|61|60|59|63|
|---|---|---|---|---|---|


Count

Error

The following ﬁgure shows how a transient fault on the qreqn_chk signal is ﬁltered.

###### Figure 4-30: Transient fault on qreqn_chk signal

clk

QREQn

QREQn_chk

QREQn_int

|63|62|63|
|---|---|---|


Count

Error

The output of the ﬁltering logic, the qreqn_int signal, does not assert. The ﬁgures depict a version of the qreqn and qreqn_chk signals after they pass synchronizer cells. The counter depicts the operation of the SAF detector. In this example, the SAF detector is set to a value of 63 whenever the qreqn and qreqn_chk signals are the same polarity. If it detects a polarity diﬀerence between the qreqn and qreqn_chk signals, it starts counting down. If the counter reaches zero, it ﬂags an error.

- 4.9.6 Stuck-at faults The following ﬁgure shows how the SAF detector detects a stuck-at-one error on the qreqn signal.


###### Figure 4-31: Stuck-at-one error on qreqn signal

clk

QREQn

QREQn_chk

QREQn_int

|63|62|63|62|61|60|59|
|---|---|---|---|---|---|---|


|2|1|0|
|---|---|---|


Count

Error

The following ﬁgure shows how the SAF detector detects a stuck-at-one error on the qreqn_chk signal.

###### Figure 4-32: Stuck-at-one error on qreqn_chk signal

clk

QREQn

QREQn_chk

QREQn_int

|63|62|61|60|59|
|---|---|---|---|---|


|2|1|0|
|---|---|---|


Count

Error

###### 4.9.7 Disabling Q-Channel Safety Mechanisms

The FMU_SMEN register cannot disable the Q-Channel Safety Mechanisms. They can be disabled during design time using one of the following methods:

- • To disable speciﬁc Q-Channel Safety Mechanisms, tie the qreqn_chk bit to the value of !qreqn on the Q-Channel interface that you want to disable protection for.
- • To disable all Q-Channel Safety Mechanisms in the MMU-600AE, set FUSA_DISABLE_PQCHAN_PROT=1. Setting this parameter disables the following Q-Channel Safety Mechanisms:


◦ TCU SM 5 ◦ TCU SM 6 ◦ TBU SM 5 ◦ TBU SM 6

##### 4.10 Systematic fault watchdog protection

MMU-600AE contains a watchdog-based PING/ACK mechanism that guards against systematic errors on the interconnect.

It engages a hardware mechanism in the MMU TCU, which pings each MMU block in a roundrobin fashion and waits for a response. If a response is not received within a programmable timeout window, a fault is reported. This mechanism can guard against:

- • Lockup on the interconnect that connects the MMU blocks.
- • Possible lockup on external buses that causes the MMU blocks and internal interconnect to


stall. The source of the lockup might be software issues, DoS issues, or systematic faults in the silicon. For more information on this feature see 4.4.7 Ping mechanism on page 116.

## Appendix A Signal descriptions

This appendix describes the MMU-600AE external signals.

###### Secure and Non-secure signals

MMU-600AE external signal names use the following convention:

_s or _s_ Secure, for example event_q_irpt_s or event_q_irpt_s_chk. _ns or _ns_ Non-secure, for example event_q_irpt_ns or event_q_irpt_ns_chk.

For more information on Secure and Non-secure signals, see the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

##### A.1 Clock and reset signals for MMU-600AE

MMU-600AE uses a single set of standard clock and reset signals.

Signal deﬁnitions

- Table A-1: Clock and reset signals

|Signal|Direction|Description|
|---|---|---|
|aclk|Input|Global clock.|
|aresetn|Input|Global reset.|


A.2 TCU QTW/DVM interface signals for MMU-600AE

The TCU QTW/DVM interface signals are based on the AMBA ACE5-Lite signals. For more information about these signals, see the AMBA® AXI and ACE Protocol Speciﬁcation.

Signal deﬁnitions

- Table A-2: TCU QTW/DVM interface signals


|Signal|Direction|Description|
|---|---|---|
|acaddr_qtw|Input|Snoop address.|
|acprot_qtw|Input|Snoop protection type.|
|acready_qtw|Output|Snoop address ready.|
|acsnoop_qtw|Input|Snoop transaction type.|
|acvalid_qtw|Input|Snoop address valid.|
|arid_qtw|Output|Read address ID.|
|araddr_qtw|Output|Read address.|
|arburst_qtw|Output|Burst type.|
|arcache_qtw|Output|Memory type.|


|Signal|Direction|Description|
|---|---|---|
|ardomain_qtw|Output|Shareability domain.|
|arlen_qtw|Output|Burst length.|
|arlock_qtw|Output|Lock type.|
|arprot_qtw|Output|Protection type.|
|arqos_qtw|Output|QoS identiﬁer.|
|arready_qtw|Input|Read address ready.|
|arregion_qtw|Output|Region identiﬁer.|
|arsize_qtw|Output|Burst size.|
|arsnoop_qtw|Output|Transaction type.|
|arvalid_qtw|Output|Read address valid.|
|awid_qtw|Output|Write address ID.|
|awaddr_qtw|Output|Write address.|
|awburst_qtw|Output|Burst type.|
|awcache_qtw|Output|Memory type.|
|awdomain_qtw|Output|Shareability domain.|
|awlen_qtw|Output|Burst length.|
|awlock_qtw|Output|Lock type.|
|awprot_qtw|Output|Protection type.|
|awqos_qtw|Output|QoS identiﬁer.|
|awready_qtw|Input|Write address ready.|
|awregion_qtw|Output|Region identiﬁer.|
|awsize_qtw|Output|Burst size.|
|awsnoop_qtw|Output|Transaction type.|
|awvalid_qtw|Output|Write address valid.|
|crready_qtw|Input|Snoop response ready.|
|crresp_qtw|Output|Snoop response.|
|crvalid_qtw|Output|Snoop response valid.|
|rid_qtw|Input|Read data ID.|
|rdata_qtw|Input|Read data.|
|rlast_qtw|Input|Read last.|
|rready_qtw|Output|Read ready.|
|rresp_qtw|Input|Read response.|
|rvalid_qtw|Input|Read valid.|
|wdata_qtw|Output|Write data.|
|wlast_qtw|Output|Write last.|
|wready_qtw|Input|Write ready.|
|wstrb_qtw|Output|Write strobe.|
|wvalid_qtw|Output|Write valid.|
|bid_qtw|Input|Response ID.|


|Signal|Direction|Description|
|---|---|---|
|bready_qtw|Output|Response ready.|
|bresp_qtw|Input|Write response.|
|bvalid_qtw|Input|Write response valid.|
|awakeup_qtw|Output|Wakeup.|
|acwakeup_qtw|Input|Snoop wakeup.|
|acvmidext_qtw|Input|Snoop Extended Virtual Machine IDentiﬁer (VMID).|


##### A.3 TCU programming interface signals for MMU-600AE

The TCU programming interface signals are based on the AMBA APB4 signals. See the Arm® AMBA® APB Protocol Speciﬁcation for more information about these signals.

###### Signal deﬁnitions

- Table A-3: TCU programming interface signals


|Signal|Direction|Description|
|---|---|---|
|paddr_prog|Input|Peripheral address.|
|psel_prog|Input|Peripheral select.|
|penable_prog|Input|Enable for transfer.|
|pwrite_prog|Input|Write transaction indicator.|
|pprot_prog|Input|Protection type.|
|pwdata_prog|Input|Write data.|
|pstrb_prog|Input|Write data strobe.|
|pslverr_prog|Output|Error response.|
|prdata_prog|Output|Read data.|
|pready_prog|Output|Transfer ready.|
|pwakeup_prog|Input|Interface wakeup.|


##### A.4 TCU SYSCO interface signals for MMU-600AE

The system coherency signals allow the TCU to enter or exit a coherency domain. For example, when the TCU needs to remove itself from a coherency domain in response to an LPI request.

###### Signal deﬁnitions

- Table A-4: TCU SYSCO interface signals

|Signal|Direction|Description|
|---|---|---|
|syscoreq|Output|System coherency request.<br><br>This output transitions:<br><br>HIGH To indicate that the TCU is requesting to enter the coherency domain. LOW To indicate that the TCU is requesting to exit the coherency domain.|
|syscoack|Input|System coherency acknowledge.<br><br>This input transitions to the same level as syscoreq when the request to enter or exit the coherency domain is complete.|


See the Arm® AMBA® AXI and ACE Protocol Speciﬁcation, AXI3, AXI4, AXI5, ACE and ACE5 for more information about these signals.

A.5 TCU PMU snapshot interface signals for MMU-600AE

The TCU signals for creating a Performance Monitoring Unit (PMU) snapshot on request. Connect the PMU snapshot interface to the debug infrastructure of your SoC.

Signal deﬁnitions

- Table A-5: TCU PMU snapshot interface signals


|Signal|Direction|Description|
|---|---|---|
|pmusnapshot_req|Input|PMU snapshot request. The PMU snapshot occurs on the rising edge of pmusnapshot_req.|
|pmusnapshot_ack|Output|PMU snapshot acknowledge. The TCU uses this signal to acknowledge that the PMU snapshot has occurred.<br><br>This signal is LOW after reset.|


##### A.6 TCU LPI_PD interface signals for MMU-600AE

Q-Channel Low Power Interface (LPI) signals for managing powerdown of the TCU.

###### Signal deﬁnitions

- Table A-6: TCU LPI_PD interface signals

|Signal|Direction|Description|
|---|---|---|
|qactive_pd|Output|Component active.|
|qreqn_pd|Input|Quiescence request.|
|qacceptn_pd|Output|Quiescence accept.|
|qdeny_pd|Output|Quiescence deny.|


See the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces for more information about these signals.

A.7 TCU LPI_CG interface signals for MMU-600AE

Q-Channel Low Power Interface (LPI) signals for clock gating the TCU.

Signal deﬁnitions

- Table A-7: TCU LPI_CG interface signals

|Signal|Direction|Description|
|---|---|---|
|qactive_cg|Output|Component active.|
|qreqn_cg|Input|Quiescence request.|
|qacceptn_cg|Output|Quiescence accept.|
|qdeny_cg|Output|Quiescence deny.|


For more information about these signals, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

A.8 TCU DTI interface signals for MMU-600AE

The TCU has DTI interface signals for connecting with one or more requester components, typically via the DTI interconnect. Each requester component can be a TBU or a PCIe requester.

Signal deﬁnitions

- Table A-8: TCU DTI interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn|Input|Flow control signal.|
|tready_dti_dn|Output|Flow control signal.|
|tdata_dti_dn|Input|Message data signal.|


|Signal|Direction|Description|
|---|---|---|
|tid_dti_dn|Input|Identiﬁes the requester (TBU) that initiated the message.|
|tlast_dti_dn|Input|Indicates the last cycle of a message.|
|tkeep_dti_dn|Input|This signal indicates valid bytes.|
|tvalid_dti_up|Output|Flow control signal.|
|tready_dti_up|Input|Flow control signal.|
|tdata_dti_up|Output|Message data signal.|
|tdest_dti_up|Output|Identiﬁes the requester (TBU) that is receiving the message.|
|tlast_dti_up|Output|Indicates the last cycle of a message.|
|tkeep_dti_up|Output|Indicates valid bytes.|
|twakeup_dti_up|Output|Wakeup signal.|
|twakeup_dti_dn|Input|Wakeup signal.|


For more information about the DTI signals, see the AMBA® 4 AXI4‑Stream Protocol Speciﬁcation. For more information about DTI protocol messages, see the AMBA® DTI Protocol Speciﬁcation.

##### A.9 TCU APB FMU interface signals for MMU-600AE

The TCU FMU programming interface signals are based on the AMBA APB4 signals. See the Arm® AMBA® APB Protocol Speciﬁcation for more information about these signals.

###### Signal deﬁnitions

- Table A-9: TCU FMU interface signals


|Signal|Direction|Description|
|---|---|---|
|paddr_fmu_prog|Input|Peripheral address.|
|psel_fmu_prog|Input|Peripheral select.|
|penable_fmu_prog|Input|Enable for transfer.|
|pwrite_fmu_prog|Input|Write transaction indicator.|
|pprot_fmu_prog|Input|Protection type.|
|pwdata_fmu_prog|Input|Write data.|
|pstrb_fmu_prog|Input|Write data strobe.|
|pslverr_fmu_prog|Output|Error response.|
|prdata_fmu_prog|Output|Read data.|
|pready_fmu_prog|Output|Transfer ready.|
|pwakeup_fmu_prog|Input|Interface wakeup.|


##### A.10 TCU interrupt signals for MMU-600AE

The TCU interrupt signals are edge‑triggered. The interrupt controller must detect the rising edge of these signals.

The TCU can also output the following as Message Signaled Interrupts (MSIs) on the QTW/DVM interface and the dedicated MSI delivery interface:

- • Secure and Non-secure Event queue
- • SYNC complete commands
- • Global interrupts
- • Page Request Interface (PRI) queue interrupt If the system supports capturing MSIs from the TCU, there is no requirement to connect the corresponding interrupt signals in this interface.


###### Signal deﬁnitions

- Table A-10: TCU interrupt interface signals


|Signal|Direction|Description|
|---|---|---|
|event_q_irpt_s|Output|Event queue, Secure interrupt. Asserts a Secure interrupt to indicate that the Event queue is not empty.|
|event_q_irpt_ns|Output|Event queue, Non‑secure interrupt. Asserts a Non‑secure interrupt to indicate that the Event queue is not empty.|
|cmd_sync_irpt_ns|Output|SYNC complete, Non‑secure interrupt. Asserts a Non‑secure interrupt to indicate that the CMD_SYNC command is complete.|
|cmd_sync_irpt_s|Output|SYNC complete, Secure interrupt. Asserts a Secure interrupt to indicate that the CMD_SYNC command is complete.|
|global_irpt_ns|Output|Asserts a global Non‑secure interrupt.|
|global_irpt_s|Output|Asserts a global Secure interrupt.|
|ras_irpt|Output|Asserts a Reliability, Availability, and Serviceability (RAS) interrupt.<br><br>Note: MMU-600AE cannot output RAS interrupts as MSIs. You must connect this output to an interrupt controller.|
|pmu_irpt|Output|Asserts a PMU interrupt.<br><br>Note: MMU-600AE cannot output PMU interrupts as MSIs. You must connect this output to an interrupt controller.|
|pri_q_irpt_ns|Output|Asserts a Page Request Interface (PRI) queue interrupt.|


##### A.11 TCU event interface signal for MMU-600AE

The TCU event interface signal is an event output used for connection to processors.

###### Signal deﬁnitions

- Table A-11: TCU event interface signal


|Signal|Direction|Description|
|---|---|---|
|evento|Output|The evento signal is asserted for one cycle to indicate an event that enables processors to wake up from the Wait For Event (WFE) low‑power state. Connect the evento signal of the TCU to the event interface of Arm® processors. Processors that use the DynamIQ Shared Unit (DSU) have a diﬀerent event handshake mechanism.<br><br>The mechanism that the DSU uses is the successor to the mechanism that some MMUs use.<br><br>Arm® processors can use the following event mechanisms:<br><br>• Some processors have an eventi input to connect directly to the evento output from the MMU.<br>• Some processors, including DSU‑based systems, have a req/ack handshake mechanism that requires the evento signal from the MMU to be converted and uses the eventiack, eventireq, eventoack, and eventoreq signals.<br><br><br>Note: You can also route event signals through other interconnects such as the Arm® CoreLink™ CMN‑600AE Coherent Mesh Network instead of connecting evento directly to the processor. These interconnects, like the DSU, only support the newer event mechanism.<br><br>If the rest of your system uses the newer event mechanism, you must add logic to convert events that MMU-600AE generates.<br><br>In both mechanisms, in the signal names:<br><br>i Represents events that are inputs to a particular component. o Represents events that are outputs from a particular component.<br><br>Note: For the signals, the handshake mechanism uses one input and one output in each direction because the acknowledgment of the request operates in the opposite direction to the original request.<br><br>MMU-600AE has an event output and therefore only has the evento signal. The processor has an input interface to receive the event from MMU-600AE, and other devices. This input interface uses the eventiack and eventireq signals, if the processor uses the newer mechanism.<br><br>The conversion, if required, is from the older mechanism, eventi and evento signals, to the newer mechanism, eventiack, eventireq, eventoack, and eventoreq signals.<br><br>When connecting MMU-600AE to a DSU, the only signals to consider are the following:<br><br>• evento and evento_chk signals of MMU-600AE.<br>• EVENTIACK, EVENTIACKCHK, EVENTIREQ, and EVENTIREQCHK signals of the DSU.<br><br><br>For information about how to connect the MMU-600AE event interface to components that use the newer event mechanism, see the Connect the event signaling interface to CMN-600AE section in the Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual.|


For more information, see the documentation for your processor or DSU.

##### A.12 TCU tie-oﬀ signals for MMU-600AE

The TCU tie-oﬀ signals are sampled between exiting reset and the LPI_PD interface ﬁrst entering the Q_RUN state. Ensure that the value of these signals does not change when the LPI_PD interface is in the Q_STOPPED or Q_EXIT state for the ﬁrst time after exiting reset.

###### Signal deﬁnitions

- Table A-12: TCU tie-oﬀ signals


|Signal|Direction|Description|
|---|---|---|
|sup_cohacc|Input|This signal indicates whether the QTW interface is I/O-coherent. Tie HIGH when the TCU is connected to a coherent interconnect.|
|sup_btm|Input|This signal indicates whether the Broadcast TLB Maintenance is supported. Tie HIGH when the TCU is connected to an interconnect that supports DVM.|
|sup_sev|Input|This signal indicates whether the Send Event mechanism is supported. Tie HIGH when evento is connected.|
|sup_oas[2:0]|Input|Output address size supported.<br><br>The encodings for this input are:<br><br>0b000 32 bits.<br>0b001 36 bits.<br><br><br>0b010 40 bits.<br>0b011 42 bits.<br><br><br>0b100 44 bits.<br>0b101 48 bits.<br><br><br>You must not use other encodings, including 0b110 that SMMUv3.1 deﬁnes to indicate 52-bit addresses. They are treated as 0b101.|
|sec_override|Input|When HIGH, certain registers are accessible to Non-secure accesses from reset, as the TCU_SCR register settings describe.|
|ecorevnum[3:0]|Input|Tie this signal to 0 unless directed otherwise by Arm.|


See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0 for more information about the SMMUv3 ID signals.

##### A.13 TCU and TBU test and debug signals for MMU-600AE

The test and debug signals are common to the TCU and TBU.

###### Signal deﬁnitions

- Table A-13: Test and debug signals

|Signal|Direction|Description|
|---|---|---|
|dftcgen|Input|Clock gate enable.<br><br>To enable architectural clock gates for the aclk clock, set this signal HIGH during scan shift.|
|dftrstdisable|Input|Reset disable.<br><br>To disable reset, set this signal HIGH during scan shift.|
|dftramhold|Input|Preserve RAM state.<br><br>To preserve the state of the RAMs and their connected registers, set this signal HIGH during scan shift.|
|mbistresetn|Input|MBIST mode reset. This active-LOW signal is encoded as follows:<br><br>0 Reset MBIST functional logic.<br>1 Normal operation.<br>|
|mbistreq|Input|MBIST test request. This signal is encoded as follows:<br><br>0 Normal operation.<br>1 Enable MBIST testing.<br>|


A.14 TBU TBS interface signals for MMU-600AE

The TBU TBS interface signals are based on the AMBA ACE5-Lite signals.

Signal deﬁnitions

- Table A-14: TBU TBS interface signals


|Signal|Direction|Description|
|---|---|---|
|aclk|Input|Clock input.|
|araddr_s|Input|Read address.|
|arburst_s|Input|Burst type.|
|arcache_s|Input|Memory type.|
|ardomain_s|Input|Shareability domain.|
|aresetn|Input|Active-LOW reset signal.|
|arid_s|Input|Read address ID.|
|arlen_s|Input|Burst length.|
|arlock_s|Input|Lock type.|
|arprot_s|Input|Protection type.|


|Signal|Direction|Description|
|---|---|---|
|arqos_s|Input|Quality of Service (QoS).|
|arready_s|Output|Read address ready.|
|arregion_s|Input|Region identiﬁer.|
|arsize_s|Input|Burst size.|
|armmussid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|armmusid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|armmussidv_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|armmusecsid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|armmuatst_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|arvalid_s|Input|Read address valid.|
|awaddr_s|Input|Write address.|
|awatop_s|Input|Atomic operation.|
|awburst_s|Input|Burst type.|
|awcache_s|Input|Memory type.|
|awdomain_s|Input|Shareability domain.|
|awid_s|Input|Write address ID.|
|awlen_s|Input|Burst length.|
|awlock_s|Input|Lock type.|
|awprot_s|Input|Protection type.|
|awqos_s|Input|QoS.|
|awready_s|Output|Write address ready.|
|awregion_s|Input|Region identiﬁer.|
|awsize_s|Input|Burst size.|
|awmmussid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|awmmusid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|awmmussidv_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|awmmusecsid_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|awmmuatst_s|Input|These signals indicate the StreamID, SubstreamID, and ATS translated status of the originating transaction.<br><br>These signals are deﬁned by the AXI5 Untranslated_Transactions extension.|
|awvalid_s|Input|Write address valid.|
|bid_s|Output|Response ID.|
|bready_s|Input|Response ready.|
|bresp_s|Output|Write response.|
|bvalid_s|Output|Write response valid.|
|rdata_s|Output|Read data.|
|rid_s|Output|Read ID.|
|rlast_s|Output|Read last.|
|rready_s|Input|Read ready.|
|rresp_s|Output|Read response.|
|rvalid_s|Output|Read valid.|


|Signal|Direction|Description|
|---|---|---|
|wdata_s|Input|Write data.|
|wlast_s|Input|Write last.|
|wready_s|Output|Write ready.|
|wstrb_s|Input|Write strobes.|
|wvalid_s|Input|Write valid.|
|aruser_s|Input|Read address (AR) channel user signal.|
|awuser_s|Input|Write address (AW) channel user signal.|
|wuser_s|Input|Write data (W) channel user signal.|
|ruser_s|Output|Read data (R) channel user signal.|
|buser_s|Output|Write response (B) channel user signal.|
|awakeup_s|Input|Wakeup signal.|
|arsnoop_s|Input|Transaction type of read transaction.|
|awsnoop_s[3]|Input|Transaction type of write transaction.|
|awstashnid_s[10:0]|Input|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashniden_s|Input|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashlpid_s[4:0]|Input|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashlpiden_s|Input|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|


##### A.15 TBU TBM interface signals for MMU-600AE

The TBU TBM interface signals are based on the AMBA ACE5-Lite signals.

###### Signal deﬁnitions

- Table A-15: TBU TBM interface signals


|Signal|Direction|Description|
|---|---|---|
|aclk|Input|Clock input.|
|araddr_m|Output|Read address.|
|arburst_m|Output|Burst type.|
|arcache_m|Output|Memory type.|
|ardomain_m|Output|Shareability domain.|
|aresetn|Input|Active-LOW reset signal.|
|arid_m|Output|Read address ID.|
|arlen_m|Output|Burst length.|
|arlock_m|Output|Lock type.|
|arprot_m|Output|Protection type.|
|arqos_m|Output|Quality of Service (QoS).|
|arready_m|Input|Read address ready.|
|arregion_m|Output|Region identiﬁer.|


|Signal|Direction|Description|
|---|---|---|
|arsize_m|Output|Burst size.|
|armmusid_m|Output|These signals indicate the StreamID of the originating transaction.|
|armmusecsid_m|Output|These signals indicate the StreamID of the originating transaction.|
|arvalid_m|Output|Read address valid.|
|awaddr_m|Output|Write address.|
|awatop_m|Output|Atomic operation.|
|awburst_m|Output|Burst type.|
|awcache_m|Output|Memory type.|
|awdomain_m|Output|Shareability domain.|
|awid_m|Output|Write address ID.|
|awlen_m|Output|Burst length.|
|awlock_m|Output|Lock type.|
|awprot_m|Output|Protection type.|
|awqos_m|Output|QoS.|
|awready_m|Input|Write address ready.|
|awregion_m|Output|Region identiﬁer.|
|awsize_m|Output|Burst size.|
|awmmusid_m|Output|These signals indicate the StreamID of the originating transaction.<br><br>The Generic Interrupt Controller (GIC) uses these signals to determine the DeviceID of MSIs that originate from upstream requesters.|
|awmmusecsid_m|Output|These signals indicate the StreamID of the originating transaction.<br><br>The Generic Interrupt Controller (GIC) uses these signals to determine the DeviceID of MSIs that originate from upstream requesters.|
|awvalid_m|Output|Write address valid.|
|bid_m|Input|Response ID.|
|bready_m|Output|Response ready.|
|bresp_m|Input|Write response.|
|bvalid_m|Input|Write response valid.|
|rdata_m|Input|Read data.|
|rid_m|Input|Read ID.|
|rlast_m|Input|Read last.|
|rready_m|Output|Read ready.|
|rresp_m|Input|Read response.|
|rvalid_m|Input|Read valid.|
|wdata_m|Output|Write data.|
|wlast_m|Output|Write last.|
|wready_m|Input|Write ready.|
|wstrb_m|Output|Write strobes.|
|wvalid_m|Output|Write valid.|
|aruser_m|Output|Read address (AR) channel user signal. For information about the extra aruser bit, see 2.1.2 AXI USER bits deﬁned by the MMU-600AE TBU on page 22.|
|awuser_m|Output|Write address (AW) channel user signal. For information about the extra awuser bit, see 2.1.2 AXI USER bits deﬁned by the MMU-600AE TBU on page 22.|


|Signal|Direction|Description|
|---|---|---|
|wuser_m|Output|Write data (W) channel user signal.|
|ruser_m|Input|Read data (R) channel user signal.|
|buser_m|Input|Write response (B) channel user signal.|
|awakeup_m|Output|Wakeup signal.|
|arsnoop_m|Output|Transaction type of read transaction.|
|awsnoop_m[3]|Output|Transaction type of write transaction.|
|awstashnid_m[10:0]|Output|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashniden_m|Output|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashlpid_m[4:0]|Output|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|
|awstashlpiden_m|Output|These signals are deﬁned by the AXI5 Cache_Stash_Transactions extension.<br><br>If TBUCFG_STASH = 0, these signals are ignored.|


##### A.16 TBU PMU snapshot interface signals for MMU-600AE

The TBU signals for creating a Performance Monitoring Unit (PMU) snapshot on request. Connect the PMU snapshot interface signals to the debug infrastructure of your SoC.

###### Signal deﬁnitions

- Table A-16: TBU PMU snapshot interface signals

|Signal|Direction|Description|
|---|---|---|
|pmusnapshot_req|Input|PMU snapshot request. The PMU snapshot occurs on the rising edge of pmusnapshot_req.|
|pmusnapshot_ack|Output|PMU snapshot acknowledge. The TBU uses this signal to acknowledge that the PMU snapshot has occurred.<br><br>This signal is LOW after reset.|


A.17 TBU LPI_PD interface signals for MMU-600AE

Q-Channel Low Power Interface (LPI) signals for managing powerdown of the TBU.

Signal deﬁnitions

- Table A-17: TBU LPI_PD interface signals


|Signal|Direction|Description|
|---|---|---|
|qactive_pd|Output|Component active.|
|qreqn_pd|Input|Quiescence request.|
|qacceptn_pd|Output|Quiescence accept.|
|qdeny_pd|Output|Quiescence deny.|


For more information about these signals, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

##### A.18 TBU LPI_CG interface signals for MMU-600AE

Q-Channel Low Power Interface (LPI) signals for clock gating the TBU.

###### Signal deﬁnitions

- Table A-18: TBU LPI_CG interface signals

|Signal|Direction|Description|
|---|---|---|
|qactive_cg|Output|Component active.|
|qreqn_cg|Input|Quiescence request.|
|qacceptn_cg|Output|Quiescence accept.|
|qdeny_cg|Output|Quiescence deny.|


For more information about these signals, see the AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces.

A.19 TBU DTI interface signals for MMU-600AE

Each TBU has DTI interface signals for connecting to the TCU, typically via the DTI interconnect.

Signal deﬁnitions

- Table A-19: TBU DTI interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn|Output|Flow control signal.|
|tready_dti_dn|Input|Flow control signal.|
|tdata_dti_dn|Output|Message data signal.|
|tlast_dti_dn|Output|Indicates the last cycle of a message.|
|tkeep_dti_dn|Output|Indicates valid bytes.|
|tvalid_dti_up|Input|Flow control signal.|
|tready_dti_up|Output|Flow control signal.|
|tdata_dti_up|Input|Message data signal.|
|tlast_dti_up|Input|Indicates the last cycle of a message.|
|tkeep_dti_up|Input|Indicates valid bytes.|
|twakeup_dti_up|Input|Wakeup signal.|
|twakeup_dti_dn|Output|Wakeup signal.|


For more information about the DTI signals, see the AMBA® 4 AXI4‑Stream Protocol Speciﬁcation. For more information about DTI protocol messages, see the AMBA® DTI Protocol Speciﬁcation.

##### A.20 TBU interrupt signals for MMU-600AE

The TBU interrupt signals are edge‑triggered. The interrupt controller must detect the rising edge of these signals.

The MMU-600AE TBU cannot output these interrupts as Message Signaled Interrupts (MSIs). These signals must be connected to an interrupt controller.

###### Signal deﬁnitions

- Table A-20: TBU interrupt signals

|Signal|Direction|Description|
|---|---|---|
|ras_irpt|Output|RAS interrupt|
|pmu_irpt|Output|PMU interrupt|


A.21 TBU tie-oﬀ signals for MMU-600AE

The TBU tie‑oﬀ signals are sampled between exiting reset and the LPI_PD interface ﬁrst entering the Q_RUN state. Ensure that the value of these signals does not change when the LPI_PD interface is in the Q_STOPPED or Q_EXIT state for the ﬁrst time after exiting reset.

Signal deﬁnitions

- Table A-21: TBU tie‑oﬀ signals


|Signal|Direction|Description|
|---|---|---|
|ns_sid_high[23:TBUCFG_SID_WIDTH]|Input|Provides the high‑order StreamID bits for all transactions with a Non‑secure StreamID that pass through the TBU.|
|s_sid_high[23:TBUCFG_SID_WIDTH]|Input|Provides the high‑order StreamID bits for all transactions with a Secure StreamID that pass through the TBU.|
|max_tok_trans[log2(TBUCFG_XLATE_SLOTS)-1:0]|Input|Indicates the number of DTI translation tokens to request when connecting to the TCU, minus 1.|


|Signal|Direction|Description|
|---|---|---|
|pcie_mode|Input|You must tie this signal HIGH when the TBU is connected to a PCIe interface.<br><br>When this signal is HIGH, the TBU interprets the input AXI memory types as encoding PCI 'No Snoop' information.<br><br>In order for the TBU to provide correct operation, transactions from the PCIe interface must be delivered to the TBU with the following AXI memory types:<br><br>Normal Non‑Cacheable Buﬀerable<br><br>When 'No Snoop' is set for the transaction Write‑Back<br><br>When 'No Snoop' is not set for the transaction<br><br>This TBU behavior is a requirement of the Arm® Server Base System Architecture 7.0 Platform Design Document.<br><br>If this signal is HIGH, the attributes of TBS interface transactions are always combined with the translation attributes, even if stage 1 translation is enabled. That is, the transaction attributes are always calculated as if the DTI_TBU_TRANS_RESP.STRW ﬁeld is EL1‑S2, regardless of the actual STRW value.<br><br>If this signal is HIGH, the input attribute and shareability override information in the ATTR_OVR ﬁeld of the DTI_TBU_TRANS_RESP message is ignored. For SMMUv3, PCIe requesters do not support this feature.|
|sec_override|Input|When HIGH, certain registers are accessible to Non‑secure accesses from reset, as the TBU_SCR register settings describe. See 3.8.2 TBU_SCR, TBU Secure Control register on page 89.|
|ecorevnum[3:0]|Input|Tie this signal to 0 unless directed otherwise by Arm.|
|utlb_roundrobin|Input|Deﬁnes the Micro TLB entry replacement policy.<br><br>When LOW, the Micro TLB uses a Pseudo Least Recently Used (PLRU) replacement policy. This policy typically provides the best average performance.<br><br>When HIGH, the Micro TLB uses a round-robin replacement policy. With this policy, the oldest entry is evicted when the Micro TLB is full.<br><br>Tie this signal HIGH if you want to prevent newer translations from being evicted, even if older translations have been used more recently. Otherwise, tie this signal LOW.|


|Signal|Direction|Description|
|---|---|---|
|cmo_disable|Input|To disable cache maintenance operations, tie this signal HIGH. When this signal is HIGH, the following transactions are always aborted with an SLVERR response:<br><br>• CleanInvalid<br>• CleanShared<br>• CleanSharedPersist<br>• MakeInvalid<br><br><br>Cache maintenance operations can sometimes break the requirements of limited sideband channel communication, such as when a requester component accesses protected content. You can disable cache maintenance operations in such cases.|


###### Related information

TCU_SCR, TCU Secure Control register on page 76

##### A.22 DTI interconnect switch DN_Sn interface signals

The MMU-600AE DTI interconnect switch provides one DN_Sn completer downstream interface per completer interface.

###### Signal deﬁnitions

- Table A-22: DTI interconnect switch DN_Sn interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_sn|Input|Flow control signal|
|tready_dti_dn_sn|Output|Flow control signal|
|tdata_dti_dn_sn|Input|Message data signal|
|tid_dti_dn_sn|Input|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_sn|Input|Indicates the last cycle of a message|
|tkeep_dti_dn_sn|Input|Indicates valid bytes|
|twakeup_dti_dn_sn|Input|Wakeup signal|


A.23 DTI interconnect switch UP_Sn interface signals

The MMU-600AE DTI interconnect switch provides one UP_Sn completer upstream interface per completer interface.

Signal deﬁnitions

- Table A-23: DTI interconnect switch UP_Sn interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_sn|Output|Flow control signal|


|Signal|Direction|Description|
|---|---|---|
|tready_dti_up_sn|Input|Flow control signal|
|tdata_dti_up_sn|Output|Message data signal|
|tdest_dti_up_sn|Output|Indicates the DTI manager that is the target of the message|
|tlast_dti_up_sn|Output|Indicates the last cycle of a message|
|tkeep_dti_up_sn|Output|Indicates valid bytes|
|twakeup_dti_up_sn|Output|Wakeup signal|


##### A.24 DTI interconnect switch DN_M interface signals

The MMU-600AE DTI interconnect switch provides a DN_M requester downstream interface.

###### Signal deﬁnitions

- Table A-24: DTI interconnect switch DN_M interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_m|Output|Flow control signal|
|tready_dti_dn_m|Input|Flow control signal|
|tdata_dti_dn_m|Output|Message data signal|
|tid_dti_dn_m|Output|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_m|Output|Indicates the last cycle of a message|
|tkeep_dti_dn_m|Output|Indicates valid bytes|
|twakeup_dti_dn_m|Output|Wakeup signal|


A.25 DTI interconnect switch UP_M interface signals

The MMU-600AE DTI interconnect switch provides an UP_M requester upstream interface.

Signal deﬁnitions

- Table A-25: DTI interconnect switch UP_M interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_m|Input|Flow control signal|
|tready_dti_up_m|Output|Flow control signal|
|tdata_dti_up_m|Input|Message data signal|
|tdest_dti_up_m|Input|Indicates the DTI manager that is the target of the message|
|tlast_dti_up_m|Input|Indicates the last cycle of a message|
|tkeep_dti_up_m|Input|Indicates valid bytes|
|twakeup_dti_up_m|Input|Wakeup signal|


##### A.26 DTI interconnect sizer LPI_CG interface signals

The MMU-600AE DTI interconnect sizer provides an LPI_CG clock gating interface.

###### Signal deﬁnitions

- Table A-26: DTI interconnect sizer LPI_CG interface signals

|Signal|Direction|Description|
|---|---|---|
|qactive_cg|Output|Component active|
|qreqn_cg|Input|Quiescence request|
|qacceptn_cg|Output|Quiescence accept|
|qdeny_cg|Output|Quiescence deny|


A.27 DTI interconnect sizer DN_S interface signals

The MMU-600AE DTI interconnect sizer provides a DN_S completer downstream interface.

Signal deﬁnitions

- Table A-27: DTI interconnect sizer DN_S interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_s|Input|Flow control signal|
|tready_dti_dn_s|Output|Flow control signal|
|tdata_dti_dn_s|Input|Message data signal|
|tid_dti_dn_s|Input|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_s|Input|Indicates the last cycle of a message|
|tkeep_dti_dn_s|Input|Indicates valid bytes|
|twakeup_dti_dn_s|Input|Wakeup signal|


A.28 DTI interconnect sizer UP_S interface signals

The MMU-600AE DTI interconnect sizer provides an UP_S completer upstream interface.

Signal deﬁnitions

- Table A-28: DTI interconnect sizer UP_S interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_s|Output|Flow control signal|
|tready_dti_up_s|Input|Flow control signal|
|tdata_dti_up_s|Output|Message data signal|
|tdest_dti_up_s|Output|Indicates the DTI manager that is the target of the message|
|tlast_dti_up_s|Output|Indicates the last cycle of a message|


|Signal|Direction|Description|
|---|---|---|
|tkeep_dti_up_s|Output|Indicates valid bytes|
|twakeup_dti_up_s|Output|Wakeup signal|


##### A.29 DTI interconnect sizer DN_M interface signals

The MMU-600AE DTI interconnect sizer provides a DN_M requester downstream interface.

###### Signal deﬁnitions

- Table A-29: DTI interconnect sizer DN_M interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_m|Output|Flow control signal|
|tready_dti_dn_m|Input|Flow control signal|
|tdata_dti_dn_m|Output|Message data signal|
|tid_dti_dn_m|Output|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_m|Output|Indicates the last cycle of a message|
|tkeep_dti_dn_m|Output|Indicates valid bytes|
|twakeup_dti_dn_m|Output|Wakeup signal|


A.30 DTI interconnect sizer UP_M interface signals

The MMU-600AE DTI interconnect sizer provides an UP_M requester upstream interface.

Signal deﬁnitions

- Table A-30: DTI interconnect sizer UP_M interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_m|Input|Flow control signal|
|tready_dti_up_m|Output|Flow control signal|
|tdata_dti_up_m|Input|Message data signal|
|tdest_dti_up_m|Input|Indicates the DTI manager that is the target of the message|
|tlast_dti_up_m|Input|Indicates the last cycle of a message|
|tkeep_dti_up_m|Input|Indicates valid bytes|
|twakeup_dti_up_m|Input|Wakeup signal|


##### A.31 DTI interconnect register slice LPI_CG interface signals

The MMU-600AE DTI interconnect register slice provides an LPI_CG clock gating interface.

###### Signal deﬁnitions

- Table A-31: DTI interconnect register slice LPI_CG interface signals

|Signal|Direction|Description|
|---|---|---|
|qactive_cg|Output|Component active|
|qreqn_cg|Input|Quiescence request|
|qacceptn_cg|Output|Quiescence accept|
|qdeny_cg|Output|Quiescence deny|


A.32 DTI interconnect register slice DN_S interface signals

The MMU-600AE DTI interconnect register slice provides a DN_S completer downstream interface.

Signal deﬁnitions

- Table A-32: DTI interconnect register slice DN_S interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_s|Input|Flow control signal|
|tready_dti_dn_s|Output|Flow control signal|
|tdata_dti_dn_s|Input|Message data signal|
|tid_dti_dn_s|Input|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_s|Input|Indicates the last cycle of a message|
|tkeep_dti_dn_s|Input|Indicates valid bytes|
|twakeup_dti_dn_s|Input|Wakeup signal|


A.33 DTI interconnect register slice UP_S interface signals

The MMU-600AE DTI interconnect register slice provides an UP_S completer upstream interface.

Signal deﬁnitions

- Table A-33: DTI interconnect register slice UP_S interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_s|Output|Flow control signal|
|tready_dti_up_s|Input|Flow control signal|
|tdata_dti_up_s|Output|Message data signal|


|Signal|Direction|Description|
|---|---|---|
|tdest_dti_up_s|Output|Indicates the requester that initiated the message|
|tlast_dti_up_s|Output|Indicates the last cycle of a message|
|tkeep_dti_up_s|Output|Indicates valid bytes|
|twakeup_dti_up_s|Output|Wakeup signal|


##### A.34 DTI interconnect register slice DN_M interface signals

The MMU-600AE DTI interconnect register slice provides a DN_M requester downstream interface.

###### Signal deﬁnitions

- Table A-34: DTI interconnect register slice DN_M interface signals

|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_dn_m|Output|Flow control signal|
|tready_dti_dn_m|Input|Flow control signal|
|tdata_dti_dn_m|Output|Message data signal|
|tid_dti_dn_m|Output|Indicates the DTI manager that initiated the message|
|tlast_dti_dn_m|Output|Indicates the last cycle of a message|
|tkeep_dti_dn_m|Output|Indicates valid bytes|
|twakeup_dti_dn_m|Output|Wakeup signal|


A.35 DTI interconnect register slice UP_M interface signals

The MMU-600AE DTI interconnect register slice provides an UP_M requester upstream interface.

Signal deﬁnitions

- Table A-35: DTI interconnect register slice UP_M interface signals


|Signal|Direction|Description|
|---|---|---|
|tvalid_dti_up_m|Input|Flow control signal|
|tready_dti_up_m|Output|Flow control signal|
|tdata_dti_up_m|Input|Message data signal|
|tdest_dti_up_m|Input|Indicates the requester that initiated the message|
|tlast_dti_up_m|Input|Indicates the last cycle of a message|
|tkeep_dti_up_m|Input|Indicates valid bytes|
|twakeup_dti_up_m|Input|Wakeup signal|


## Appendix B Software initialization examples

This appendix provides examples of how software can initialize and enable the MMU-600AE.

##### B.1 Initializing the SMMU

Software must initialize MMU-600AE before you can use it. MMU-600AE supports Secure and Non-secure translation worlds. This section deﬁnes how to initialize Non-secure translation. The procedures for initializing Secure translation are similar, and require you to access the corresponding MMU-600AE Secure registers.

This section does not describe how to create translation tables. See the Arm® Architecture Reference Manual, Armv8, for Armv8‑A architecture proﬁle for more information.

For more information about MMU-600AE initialization, see the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1.

###### B.1.1 Allocating the Command queue

MMU-600AE uses the Command queue to receive commands. Software must allocate memory for the Command queue and conﬁgure the appropriate registers in the SMMU.

###### About this task

To allocate the Command queue, ensure that your software performs the following steps:

###### Procedure

- 1. Allocate memory for the Command queue.
- 2. Conﬁgure the Command queue size and base address by writing to the SMMU_CMDQ_BASE register.

The queue size can aﬀect how many bits of the SMMU_CMDQ_CONS and SMMU_CMDQ_PROD indices are writeable. It is therefore important that you perform this step before writing to SMMU_CMDQ_CONS and SMMU_CMDQ_PROD.

- 3. Set the queue read index in SMMU_CMDQ_CONS and the queue write index in SMMU_CMDQ_PROD to 0.


Setting the queue read index and the queue write index to the same value indicates that the queue is empty.

###### B.1.2 Allocating the Event queue

MMU-600AE uses the Event queue to signal events. Software must allocate memory for the Event queue and conﬁgure the appropriate registers in the MMU.

About this task

To allocate the Event queue, ensure that your software performs the following steps:

Procedure

- 1. Allocate memory for the Event queue.
- 2. Conﬁgure the Event queue size and base address by writing to the SMMU_EVENTQ_BASE register.

The queue size can aﬀect how many bits of the SMMU_EVENTQ_CONS and SMMU_EVENTQ_PROD indices are writeable. It is therefore important that you perform this step before writing to SMMU_EVENTQ_CONS and SMMU_EVENTQ_PROD.

- 3. Set the queue read index in SMMU_EVENTQ_CONS and the queue write index in SMMU_EVENTQ_PROD to 0.


Setting the queue read index and the queue write index to the same value indicates that the queue is empty.

###### B.1.3 Conﬁguring the Stream table

The Stream table is a conﬁguration structure in memory that uses a Context Descriptor (CD) to locate translation data for a transaction. Software must allocate memory for the Stream table, conﬁgure the table format, and populate the table with Stream Table Entries (STEs).

###### About this task

To conﬁgure the Stream table, ensure that your software performs the following steps:

###### Procedure

- 1. Allocate memory for the Stream table.
- 2. Conﬁgure the format and size of the Stream table by writing to SMMU_STRTAB_BASE_CFG.
- 3. Conﬁgure the base address for the Stream table by writing to SMMU_STRTAB_BASE.


- 4. Prevent uninitialized memory being interpreted as a valid conﬁguration by setting STE.V = 0 for each STE to mark it as invalid.
- 5. Ensure that written data is observable to the SMMU by performing a Data Synchronization Barrier (DSB) operation. If SMMU_IDR0.COHACC = 0, the system does not support coherent access to memory for the TCU. In such cases, you might require extra steps to ensure that the SMMU can observe the written data.


###### B.1.4 Initializing the Command queue

Software must initialize the Command queue by enabling it and checking that the enable operation is complete.

About this task

To initialize the Command queue, ensure that your software performs the following steps:

Procedure

- 1. Enable the Command queue by setting the SMMU_CR0.CMDQEN bit to 1.
- 2. Check that the enable operation is complete by polling SMMU_CR0ACK until CMDQEN reads as 1.


###### B.1.5 Initializing the Event queue

Software must initialize the Event queue by enabling it and checking that the enable operation is complete.

About this task

To initialize the Event queue, ensure that your software performs the following steps:

Procedure

- 1. Enable the Event queue by setting the SMMU_CR0.EVENTQEN bit to 1.
- 2. Check that the enable operation is complete by polling SMMU_CR0ACK until EVENTQEN reads as 1.


###### B.1.6 Invalidating TLBs and conﬁguration caches

Before use, the MMU-600AE TLBs and conﬁguration cache structures must be invalidated by issuing commands to the Command queue. Alternatively, Secure software can invalidate all TLBs and caches with a single write.

To invalidate TLB entries, ensure that your software issues the appropriate command for the translation context. To invalidate TLB entries for:

Non‑secure EL1 contexts

Issue CMD_TLBI_NSNH_ALL EL2 contexts Issue CMD_TLBI_EL2_ALL

###### EL3 contexts Issue CMD_TLBI_EL3_ALL Secure EL1 contexts

Issue CMD_TLBI_NH_ALL

Commands to invalidate Secure TLB entries can only be issued through the Secure Command queue. For a system that implements two security states, Secure software must issue the appropriate command to the Secure Command queue for the ﬁrst TLB invalidation. If your system does not use Secure software, you can permit Non‑secure software to access SMMU_S_INIT by using sec_override. See A.12 TCU tie-oﬀ signals for MMU-600AE on page 170 and A.21 TBU tie-oﬀ signals for MMU-600AE on page 177.

To invalidate both the TCU conﬁguration cache and the TBU combined conﬁguration cache and TLB, issue the CMD_CFGI_ALL command.

To force all previous commands to complete, issue CMD_SYNC. To invalidate all conﬁguration caches and TLB entries for all translation regimes and security states, ensure that Secure software:

- 1. Sets SMMU_S_INIT.INV_ALL to 1. The SMMU sets SMMU_S_INIT.INV_ALL to 0 after the invalidation completes.
- 2. Polls SMMU_S_INIT.INV_ALL to check it is set to 0 before continuing the SMMU conﬁguration. See the Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version
- 3.0 for more information about issuing commands to the Command queue.


###### B.1.7 Creating a basic Context Descriptor

A Context Descriptor (CD) is a data structure in system memory. A CD deﬁnes how Stage 1 translation is performed. The SubstreamID is used to select the CD.

To create a CD, ensure that your software performs the following steps:

- 1. Allocate 64 bytes of memory for the CD.
- 2. Conﬁgure the CD ﬁelds according to the information in the following table.


Table B-1: Conﬁguring the CD

|Field|Description|
|---|---|
|AA64|Translation table format:<br><br>0 AArch32.<br>1 AArch64.<br>|
|EPD0|Enable translations for TTB0 by setting EPD0 to 0.|
|TTB0|Base address of translation table 0.|
|TG0|Translation granule size for TTB0 when CD.AA64 = 1.|


|Field|Description|
|---|---|
|IR0<br><br>OR0|Cacheability attribute to use for translation table walks to TTB0:<br><br>00 Non-cacheable.<br>01 Write-Back Cacheable, Read-Allocate Write-Allocate. 10 Write-through Cacheable, Read-Allocate.<br>|
|SH0|Shareability of translation table walks to TTB0:<br><br>00 Non-shareable.<br>01 Outer Shareable. 10 Inner Shareable.<br>|
|EPD1|If the StreamWorld supports split address spaces, enable table walks for TTB1.|
|ENDI|The endianness for the translation tables.|
|IPS|The IPA size when CD.AA64 = 1.|
|ASET|Deﬁnes whether the ASID values are shared with the ASID values of an Arm processor.<br><br>Note: If you expect this context to receive broadcast TLB invalidation commands from a PE, set ASET to 0.|
|V|Valid CD. This ﬁeld must be set to 1.|


###### B.1.8 Creating a Stream Table Entry

Each Stream Table Entry (STE) conﬁgures how Stage 2 translation is performed, and how the Context Descriptor (CD) table can be found. The StreamID is used to select an STE.

To create an STE, ensure that your software performs the following steps:

- 1. Allocate 64 bytes of memory for the STE.
- 2. Set the STE.Conﬁg ﬁeld as required for Stage 1 translation, Stage 2 translation, or translation bypass:

0b000 No traﬃc can pass through the MMU. An abort is returned.

- 0b100 Stage 1 and Stage 2 bypass.
- 0b101 Stage 1 translation Stage 2 bypass.


- 0b110 Stage 1 bypass Stage 2 translation.
- 0b111 Stage 1 and Stage 2 translation.


- 3. If Stage 1 translation is enabled, you can set the following ﬁelds:

STE.S1CDMax Controls whether STE.S1ContextPtr points to a single CD or a CD

table. STE.S1Fmt If STE.S1CDMax > 0, conﬁgures the format of the CD table. STE. S1ContextPtr

Contains a pointer to either a CD or a CD table. If Stage 2 translation is enabled, this pointer is an intermediate physical address (IPA), otherwise it is an untranslated physical address PA.

- 4. If Stage 2 translation is enabled, you can set the following ﬁelds: STE.S2TTB Points to the Stage 2 translation table base address.


STE.S2PS Contains the PA size of the stage 2 PA range. STE.S2AA64 Indicates whether the Stage 2 tables are AArch32 or AArch64 format. STE.S3ENDI Set this ﬁeld to the required endianness for the stage 2 translation

tables. STE.S2AFFD Disable Access Flag faults for Stage 2 translation. STE.S2TG 0b00: 4KB. STE.S2IR0 and STE.S2OR0

0b00: Non-cacheable.

STE.S2SH0 STE.S2VMID Contains the VMID associated with these translations.

##### B.2 Enabling the SMMU

Software can enable the SMMU by writing to SMMU_CR0 after the Stream table is populated.

###### About this task

To enable the SMMU, carry out the following procedure.

###### Procedure

- 1. Ensure that all Stream table entries are populated in memory.
- 2. Set the SMMU_CR0.SMMUEN bit to 1.
- 3. Check that the enable operation is complete by polling SMMU_CR0ACK until SMMUEN reads as 1.


## Proprietary Notice

This document is protected by copyright and other related rights and the use or implementation of the information contained in this document may be protected by one or more patents or pending patent applications. No part of this document may be reproduced in any form by any means without the express prior written permission of Arm Limited ("Arm"). No license, express or implied, by estoppel or otherwise to any intellectual property rights is granted by this document unless speciﬁcally stated.

Your access to the information in this document is conditional upon your acceptance that you will not use or permit others to use the information for the purposes of determining whether the subject matter of this document infringes any third party patents.

The content of this document is informational only. Any solutions presented herein are subject to changing conditions, information, scope, and data. This document was produced using reasonable eﬀorts based on information available as of the date of issue of this document. The scope of information in this document may exceed that which Arm is required to provide, and such additional information is merely intended to further assist the recipient and does not represent Arm’s view of the scope of its obligations. You acknowledge and agree that you possess the necessary expertise in system security and functional safety and that you shall be solely responsible for compliance with all legal, regulatory, safety and security related requirements concerning your products, notwithstanding any information or support that may be provided by Arm herein. In addition, you are responsible for any applications which are used in conjunction with any Arm technology described in this document, and to minimize risks, adequate design and operating safeguards should be provided for by you.

This document may include technical inaccuracies or typographical errors. THIS DOCUMENT IS PROVIDED "AS IS". ARM PROVIDES NO REPRESENTATIONS AND NO WARRANTIES, EXPRESS, IMPLIED OR STATUTORY, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON-INFRINGEMENT OR FITNESS FOR A PARTICULAR PURPOSE WITH RESPECT TO THE DOCUMENT. For the avoidance of doubt, Arm makes no representation with respect to, and has undertaken no analysis to identify or understand the scope and content of, any patents, copyrights, trade secrets, trademarks, or other rights.

TO THE EXTENT NOT PROHIBITED BY LAW, IN NO EVENT WILL ARM BE LIABLE FOR ANY DAMAGES, INCLUDING WITHOUT LIMITATION ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF ANY USE OF THIS DOCUMENT, EVEN IF ARM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Reference by Arm to any third party’s products or services within this document is not an express or implied approval or endorsement of the use thereof.

This document consists solely of commercial items. You shall be responsible for ensuring that any permitted use, duplication, or disclosure of this document complies fully with any relevant

export laws and regulations to assure that this document or any portion thereof is not exported, directly or indirectly, in violation of such export laws. Use of the word “partner” in reference to Arm’s customers is not intended to create or refer to any partnership relationship with any other company. Arm may make changes to this document at any time and without notice.

This document may be translated into other languages for convenience, and you agree that if there is any conﬂict between the English version of this document and any translation, the terms of the English version of this document shall prevail.

The validity, construction and performance of this notice shall be governed by English Law.

The Arm corporate logo and words marked with ® or ™ are registered trademarks or trademarks of Arm Limited (or its aﬃliates) in the US and/or elsewhere. Please follow Arm’s trademark usage guidelines at https://www.arm.com/company/policies/trademarks. All rights reserved. Other brands and names mentioned in this document may be the trademarks of their respective owners.

Arm Limited. Company 02557590 registered in England. 110 Fulbourn Road, Cambridge, England CB1 9NJ. PRE-1121-V1.0

## Product and document information

Read the information in these sections to understand the release status of the product and documentation, and the conventions used in the documents.

##### Product status

All products and Services provided by Arm require deliverables to be prepared and made available at diﬀerent levels of completeness. The information in this document indicates the appropriate level of completeness for the associated deliverables.

###### Product completeness status

The information in this document is Final, that is for a developed product.

##### Revision history

These sections can help you understand how the document has changed over time.

###### Document release information

The Document history table gives the issue number and the released date for each released issue of this document.

###### Document history

|Issue|Date|Conﬁdentiality|Change|
|---|---|---|---|
|010007|16 August 2024|NonConﬁdential|Fifth release for r1p0 REL.|
|010006|6 December 2023|NonConﬁdential|Fourth release for r1p0 REL. Updated document issue numbering.|
|010002|15 November 2022|NonConﬁdential|Third release for r1p0 REL.|
|010001|18 August 2020|NonConﬁdential|Second release for r1p0 REL.|
|010000|31 July 2020|NonConﬁdential|First release for r1p0 REL.|
|0000-<br>01<br>|7 March 2019|NonConﬁdential|First release for r0p0 EAC.|


|Issue|Date|Conﬁdentiality|Change|
|---|---|---|---|
|000000|14 December 2018|Conﬁdential|First release for r0p0 BET.|


###### The Change history tables describe the technical changes between released issues of this document in reverse order. Issue numbers match the revision history in Document release information on page 193.

- Table 2: Diﬀerences between Issue 0100‑06 REL and Issue 0100‑07 REL

|Change|Location|
|---|---|
|Added bidirectional cross-references from one section to the other in the following sections:<br><br>• Programmer's model chapter<br>• Fault Detection and Control Programmer's View section<br><br><br>Added descriptive text to indicate that the full programmer's model for MMU-600AE includes the registers in the Fault Detection and Control Programmer's Viewsection in addition to the main registers in the Programmer's model chapter.|• 3. Programmer's model on page 60<br>• 4.4.9 Fault Detection and Control Programmer's View on page 119<br>|
|Added text to make it clear that the memory map in this section is only part of the full memory map and that the Fault Detection and Control Programmer's View forms the full memory map.|3.2 MMU-600AE memory map on page 65|
|Improved the description of the FMU registers, including describing how the number of connected TBUs aﬀects the oﬀsets.|4.4.9.1 FMU Programmer's View registers on page 121|
|Improved the descriptions of how to tie oﬀ signals.|4.7.7 Connecting to unsafe interfaces on page 152|


- Table 3: Diﬀerences between Issue 0100‑02 REL and Issue 0100‑06 REL


|Change|Location|
|---|---|
|Simpliﬁed and reorganized the structure of each chapter, where possible.|Throughout book.|
|Corrected non-inclusive language.|Throughout book.|
|Increased use of the product name in either the section title or the ﬁrst paragraph.|Throughout book.|
|Where practical, improved the content description at the beginning of a section.|Throughout book.|
|Increased expansion of abbreviations where they appear for a ﬁrst time in a section.|Throughout book.|
|Fixed redundant subsection nesting, wherever possible, primarily to remove sections that do not have information of value to the reader.|Throughout book.|
|Replaced some Related Information lists with paragraphs that provide context for the links to the related topics.|Throughout book.|
|Replaced document references with links where possible, and made sure that links go to the correct document versions.|Throughout book.|
|Reorganized document references and removed unreferenced and redundant items.|-|


|Change|Location|
|---|---|
|Reinstated bullet points about how the TCU_PROG interface address width depends on TCUCFG_NUM_TBU.|2.4.1.2 TCU PROG interface on page 33.|
|Updated AXI4-Stream support to include: AXI5 Wakeup_Signal added and enabled.|2.4.1.5 TCU DTI interface on page 34, 2.4.2.5 TBU DTI interface on page 37 and all tables in the subsections of 2.4.3 DTI interconnect interfaces on page 38.|
|Corrected TCU prefetch information with respect to performance.|2.2.3 TCU prefetch on page 28.|
|Reorganized details about individual registers.|3. Programmer's model on page 60 and 4.4.9.1 FMU Programmer's View registers on page 121.|
|Updated TBU details to indicate support for up to 62 TBUs.|Table 3-3: MMU-600AE memory map on page 65.|
|Moved the footnote information to notes in the relevant table entries, also with minor corrections.|Table 4-7: Safety Mechanisms on page 110.|
|Fixed text overruns from the ﬁrst column into the second column.|Table 4-8: FMU PV register summary on page 121.|
|Added TCU FMU interface signals.|A.9 TCU APB FMU interface signals for MMU-600AE on page 167.|
|Updated descriptions for event_q_irpt_s and event_q_irpt_ns.|Table A-10: TCU interrupt interface signals on page 168.|
|Updated the evento signal description.|A.11 TCU event interface signal for MMU-600AE on page 168|
|Corrected the DTI interface signal details and added some missing signal information.|A.22 DTI interconnect switch DN_Sn interface signals on page 179 to A.35 DTI interconnect register slice UP_M interface signals on page 184|


###### Table 4: Diﬀerences between Issue 0100‑01 REL and Issue 0100‑02 REL

|Change|Location|
|---|---|
|Improved content descriptions.|Throughout the document.|
|Clariﬁcation of usage constraint, and additional information on sec_override input signal.|3.8.2 TBU_SCR, TBU Secure Control register on page 89.|
|Additional information on prefetch and prefetch triggers.|2.2.3 TCU prefetch on page 28.|
|Information on naming convention for Secure and Non-secure signals.|A. Signal descriptions on page 162|
|Corrected description of behavior of FMU PROG APB4 port when access fails security check.|4.4.9 Fault Detection and Control Programmer's View on page 119|
|Updated value for SMMU_IIDR ProductID.|2.11.1 SMMUv3 support on page 53|
|Added information about the functionality of Safety Mechanism TCU_FMU_CLK_GATING|4.4.4 Safety Mechanism table on page 109|


###### Table 5: Diﬀerences between Issue 0000‑01 EAC and Issue 0100‑01 REL

|Change|Location|
|---|---|
|Corrections to the minor revision, MAX[0x1, ecorevnum]|• 2.11.1 SMMUv3 support on page 53.<br>• 3.4.1 TCU component and peripheral ID registers on page 69.<br>• 3.3.1 TCU PMU component and peripheral ID registers on page 67.<br>• 3.7.1 TBU component and peripheral ID registers on page 87.<br>• 3.3.2 TBU PMU component and peripheral ID registers on page 68.<br>|
|Updates to the description of TCU prefetching|2.2.3 TCU prefetch on page 28.|


|Change|Location|
|---|---|
|Correction to TCU_ERRFR reset value.|3.6.1 TCU_ERRFR, TCU Error Feature register on page 81.|
|Correction to TCU ERRCTRL.FI description.|3.6.2 TCU_ERRCTLR, TCU Error Control register on page 82.|
|Updated the usage constraints and bit descriptions for the TBU_SCR register.|3.8.2 TBU_SCR, TBU Secure Control register on page 89.|
|Correction to TBU_ERRFR reset value.|3.9.1 TBU_ERRFR, TBU Error Feature register on page 91.|
|Correction to TBU ERRCTRL.FI description.|3.9.2 TBU_ERRCTLR, TBU Error Control register on page 92.|
|Updated the description, category, and location of the evento signal.|A.11 TCU event interface signal for MMU-600AE on page 168.|
|Updated the description of the 'Distributed Virtual Memory (DVM) messages' section.|2.2.1 Distributed Virtual Memory messages on page 26.|
|Updated the description of the cmo_disable TBU tie‑oﬀ signal.|A.21 TBU tie-oﬀ signals for MMU-600AE on page 177.|
|Updated the description of the TCU memory map.|3.2 MMU-600AE memory map on page 65.|
|Improved content descriptions.|Throughout the document.|
|Added descriptions for the DTI interconnect sizer.|Throughout the document.|
|Multiple updates that are related to FuSa.|4. Functional Safety on page 97.|


###### Table 6: Diﬀerences between Issue 0000‑00 BET and Issue 0000‑01 EAC

|Change|Location|
|---|---|
|Removed references to P‑Channel.|Throughout the document.|
|Updated TBU register address ranges.|Table 3-3: MMU-600AE memory map on page 65.|
|Updated TCU registers, page 0 description.|Table 3-4: MMU-600AE TCU memory map on page 66.|
|Updated TBU registers, page 0 description.|Table 3-5: MMU-600AE TBU memory map on page 66.|
|Corrected all port names.|Table 4-2: Q-Channel FuSa ports on page 102.|
|Corrected nmbistresetn signal name.|Table 4-4: Protected MBIST inputs on page 104.|
|Removed dftse signal.|Table 4-5: Duplicate ATPG input ports on page 105.|
|Corrected fmu_fault_int, fack, and freq signal names.|Figure 4-3: FMU in MMU block on page 106.|
|Corrected signal names.|4.4.2.1 Reset on page 107.|
|Added TBU8 to TBU14.|Table 4-6: MMU block IDs on page 109.|
|Updated IDs for TCU tie‑oﬀ or Interrupt error and TBU tie‑oﬀ or Interrupt error. Added TCU FMU clock gating override and TBU FMU clock gating override Safety Mechanisms.|Table 4-7: Safety Mechanisms on page 110.|
|Added speciﬁc key value.|4.4.6 Lock and key mechanism on page 115.|
|Updated register summary table to include new registers.|Table 4-8: FMU PV register summary on page 121.|
|Added CE_EN bitﬁeld.|4.4.9.1.2 FMU_ERR<n>CTLR, Error Record Control Register on page 123.|
|Added BLKID and CE bitﬁelds. Updated description for V bitﬁeld.|4.4.9.1.3 FMU_ERR<n>STATUS, Error Record Primary Status Register on page 125.|
|Added new register description.|4.4.9.1.4 FMU_ERRGSR, Error Group Status Register on page 129.|
|Updated bit range for ping_timeout_value.|4.4.9.1.6 FMU_PINGCTLR, Ping Control Register on page 131.|


|Change|Location|
|---|---|
|Added new register description.|4.4.9.1.10 FMU_PINGMASK, Ping Mask Register on page 137.|
|Updated bit range for idle bitﬁeld.|4.4.9.1.11 FMU_STATUS, FMU Status Register on page 138.|
|Updated reset value for NUM bitﬁeld.|4.4.9.1.12 FMU_ERRIDR, Error Record ID Register on page 139.|
|Added new ﬁgure.|Figure 4-21: F-Channel and FMU fault wire connections on page 151.|
|Added new section.|4.7.5 Interrupt output protection on page 152.|
|Added new section.|4.7.6 Tie-oﬀ input protection on page 152.|
|Updated ﬁgures.|4.9 Q-Channel protection on page 154.|
|Removed redundant text.|'Conﬁguration and parameters' section that is no longer contained in this document.|
|Added rresp_s signal.|Table A-14: TBU TBS interface signals on page 171.|
|Added rresp_m signal.|Table A-15: TBU TBM interface signals on page 173.|


###### Table 7: Issue 0000‑00 BET

|Change|Location|
|---|---|
|First release for r0p0 BET.|-|


##### Conventions

The following subsections describe conventions used in Arm documents.

###### Glossary

The Arm Glossary is a list of terms used in Arm documentation, together with deﬁnitions for those terms. The Arm Glossary does not contain terms that are industry standard unless the Arm meaning diﬀers from the generally accepted meaning.

See the Arm Glossary for more information: developer.arm.com/glossary.

###### Typographic conventions

Arm documentation uses typographical conventions to convey speciﬁc meaning.

|Convention|Use|
|---|---|
|italic|Citations.|
|bold|Interface elements, such as menu names.<br><br>Terms in descriptive lists, where appropriate.|
|monospace|Text that you can enter at the keyboard, such as commands, ﬁle and program names, and source code.|
|monospace underline<br><br>|A permitted abbreviation for a command or option. You can enter the underlined text instead of the full command or option name.|


|Convention|Use|
|---|---|
|<and>|Encloses replaceable terms for assembler syntax where they appear in code or code fragments.<br><br>For example:<br><br>MRC p15, 0, <Rd>, <CRn>, <CRm>, <Opcode_2>|
|SMALL CAPITALS|Terms that have speciﬁc technical meanings as deﬁned in the Arm® Glossary. For example, IMPLEMENTATION DEFINED, IMPLEMENTATION SPECIFIC, UNKNOWN, and UNPREDICTABLE.|


We recommend the following. If you do not follow these recommendations your system might not work.

Your system requires the following. If you do not follow these requirements your system will not work.

You are at risk of causing permanent damage to your system or your equipment, or of harming yourself.

This information is important and needs your attention.

This information might help you perform a task in an easier, better, or faster way.

This information reminds you of something important relating to the current content.

## Useful resources

This document contains information that is speciﬁc to this product. See the following resources for other useful information.

Access to Arm documents depends on their conﬁdentiality:

- • Non-Conﬁdential documents are available at developer.arm.com/documentation. Each document link in the following tables goes to the online version of the document.
- • Conﬁdential documents are available to licensees only through the product package.


|Arm product resources|Document ID|Conﬁdentiality|
|---|---|---|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit Conﬁguration and Integration Manual|101413|Conﬁdential|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit Safety Manual|101414|Conﬁdential|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit Development Interface Report|101415|Conﬁdential|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit FMEDA Report|PJDOC-1779577084-26450|Conﬁdential|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit Dependent Failure Analysis Report|PJDOC-1779577084-12315|Conﬁdential|
|Arm® CoreLink™ MMU-600AE System Memory Management Unit Release Note|PJDOC-1779577084-31927|Conﬁdential|
|Arm® CoreLink™ LPD‑500 Low Power Distributor Technical Reference Manual|100361|NonConﬁdential|
|Arm® CoreLink™ ADB‑400 AMBA® Domain Bridge User Guide|DUI 0615|Conﬁdential|


|Arm architecture and speciﬁcations|Document ID|Conﬁdentiality|
|---|---|---|
|Arm® System Memory Management Unit Architecture Speciﬁcation, SMMU architecture version 3.0, and version 3.1|IHI 0070B|NonConﬁdential|
|Arm® Architecture Reference Manual Armv8, for A-proﬁle architecture|DDI 0487G.b|NonConﬁdential|
|AMBA® DTI Protocol Speciﬁcation|IHI 0088E.c|NonConﬁdential|
|AMBA® APB Protocol Speciﬁcation|IHI 0024D|NonConﬁdential|
|AMBA® AXI and ACE Protocol Speciﬁcation|IHI 0022F.b|NonConﬁdential|
|AMBA® 4 AXI4‑Stream Protocol Speciﬁcation|IHI 0051A|NonConﬁdential|
|AMBA® Low Power Interface Speciﬁcation, Arm® Q‑Channel and P‑Channel Interfaces|IHI 0068|NonConﬁdential|
|Arm® Server Base System Architecture 7.0 Platform Design Document|DEN 0029F|NonConﬁdential|
|Arm® Architecture Reference Manual Supplement Reliability, Availability, and Serviceability (RAS), for Armv8‑A|DDI 0587C.b|NonConﬁdential|


