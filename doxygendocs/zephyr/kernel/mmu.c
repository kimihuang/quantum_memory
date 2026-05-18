/**
 * @file mmu.c
 * @brief Zephyr 内核虚拟地址空间与页帧管理
 *
 * 本模块实现了 Zephyr 内核的 MMU 相关核心功能，包括：
 * - Page Frame（物理页帧）的分配、回收与状态管理
 * - 虚拟地址空间的分配与释放（基于 bitmap）
 * - 内存的映射与解映射（匿名映射、物理地址映射）
 * - Demand Paging（按需分页）：page fault 处理、页面换入/换出/淘汰
 *
 * 模块通过 z_mm_lock 自旋锁保护所有全局数据结构，
 * 并串行化 arch 层的页表更新操作。
 *
 * 虚拟内存布局（从低到高）：
 * +------------------+ <- K_MEM_VIRT_RAM_START
 * | Undefined VM     | 辅助区域
 * +------------------+ <- K_MEM_KERNEL_VIRT_START
 * | 内核镜像映射      |
 * +------------------+ <- K_MEM_VM_FREE_START
 * | 可用虚拟内存       | virt_region_bitmap 管理，从高地址向低地址分配
 * |                  |
 * +------------------+
 * | Reserved (1页)   | scratch page（demand paging 用）
 * +------------------+ <- K_MEM_VIRT_RAM_END
 *
 * @note 启用 CONFIG_DEMAND_PAGING 时，需要架构层实现 backing store 驱动
 *       以及 arch_mem_scratch()、arch_mem_page_out/in() 等接口。
 * @warning Demand Paging 期间，ISR 代码/数据页必须被 pin，否则 ISR 中
 *          触发 page fault 会导致不可恢复错误。
 *
 * @copyright Copyright (c) 2020 Intel Corporation
 * @ SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <kernel_arch_interface.h>
#include <zephyr/spinlock.h>
#include <mmu.h>
#include <zephyr/init.h>
#include <kernel_internal.h>
#include <zephyr/internal/syscall_handler.h>
#include <zephyr/toolchain.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/sys/bitarray.h>
#include <zephyr/sys/check.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/timing/timing.h>
#include <zephyr/arch/common/init.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(os, CONFIG_KERNEL_LOG_LEVEL);

#ifdef CONFIG_DEMAND_PAGING
#include <zephyr/kernel/mm/demand_paging.h>
#endif /* CONFIG_DEMAND_PAGING */

/*
 * 术语说明：
 * - Page Frame（页帧）：RAM 中一个页大小的物理内存区域，作为 data page 的容器。
 *   通过物理地址引用，用 uintptr_t 表示。每个页帧有一个
 *   struct k_mem_page_frame 实例来存储元数据。
 *
 * - Data Page（数据页）：一个页大小的数据区域。它可能驻留在某个 page frame 中，
 *   也可能被换出到 backing store。通过虚拟地址在 CPU 页表中查找其位置。
 *   数据类型为 void * 或 uint8_t *（需要指针运算时）。
 */

/**
 * @brief 内存管理全局自旋锁
 *
 * 保护本文件中的所有全局数据结构，并串行化 arch 层的页表更新操作。
 * 所有操作 page frame 和虚拟地址分配的函数都必须持有此锁。
 */
struct k_spinlock z_mm_lock;

/*
 * 通用页帧管理
 */

/** @brief 所有 RAM 页帧的元数据数据库，按物理地址索引 */
struct k_mem_page_frame k_mem_page_frames[K_MEM_NUM_PAGE_FRAMES];

#if __ASSERT_ON
/**
 * @brief 页帧数据库是否已初始化的标志
 *
 * 许多 API 在 POST_KERNEL 阶段之前不可用，该标志用于断言检查。
 */
static bool page_frames_initialized;
#endif

/* 为页表转储添加颜色标识以区分映射类型 */
#define COLOR_PAGE_FRAMES	1

#if COLOR_PAGE_FRAMES
#define ANSI_DEFAULT "\x1B" "[0m"      /**< 默认颜色 */
#define ANSI_RED     "\x1B" "[1;31m"   /**< 红色 */
#define ANSI_GREEN   "\x1B" "[1;32m"   /**< 绿色 */
#define ANSI_YELLOW  "\x1B" "[1;33m"   /**< 黄色 */
#define ANSI_BLUE    "\x1B" "[1;34m"   /**< 蓝色 */
#define ANSI_MAGENTA "\x1B" "[1;35m"   /**< 品红色 */
#define ANSI_CYAN    "\x1B" "[1;36m"   /**< 青色 */
#define ANSI_GREY    "\x1B" "[1;90m"   /**< 灰色 */

#define COLOR(x)	printk(_CONCAT(ANSI_, x))
#else
#define COLOR(x)	do { } while (false)
#endif /* COLOR_PAGE_FRAMES */

/* LCOV_EXCL_START */

/**
 * @brief 打印单个页帧的状态字符
 *
 * 根据页帧状态输出对应颜色的单字符标识：
 * - `-` 灰色：空闲（free）
 * - `R` 青色：保留（reserved）
 * - `B` 品红：忙（busy）
 * - `P` 黄色：pinned（不可换出）
 * - `.` 灰色：可淘汰（available/evictable）
 * - `M` 默认：已映射（mapped）
 * - `?` 红色：未知状态
 *
 * @param pf 指向页帧元数据结构
 */
static void page_frame_dump(struct k_mem_page_frame *pf)
{
	if (k_mem_page_frame_is_free(pf)) {
		COLOR(GREY);
		printk("-");
	} else if (k_mem_page_frame_is_reserved(pf)) {
		COLOR(CYAN);
		printk("R");
	} else if (k_mem_page_frame_is_busy(pf)) {
		COLOR(MAGENTA);
		printk("B");
	} else if (k_mem_page_frame_is_pinned(pf)) {
		COLOR(YELLOW);
		printk("P");
	} else if (k_mem_page_frame_is_available(pf)) {
		COLOR(GREY);
		printk(".");
	} else if (k_mem_page_frame_is_mapped(pf)) {
		COLOR(DEFAULT);
		printk("M");
	} else {
		COLOR(RED);
		printk("?");
	}
}

/**
 * @brief 转储所有页帧的状态信息（调试用）
 *
 * 遍历所有页帧，以每行 64 个字符的格式打印状态标识，
 * 直观展示物理内存的使用情况。
 */
void k_mem_page_frames_dump(void)
{
	int column = 0;

	__ASSERT(page_frames_initialized, "%s called too early", __func__);
	printk("Physical memory from 0x%lx to 0x%lx\n",
	       K_MEM_PHYS_RAM_START, K_MEM_PHYS_RAM_END);

	for (int i = 0; i < K_MEM_NUM_PAGE_FRAMES; i++) {
		struct k_mem_page_frame *pf = &k_mem_page_frames[i];

		page_frame_dump(pf);

		column++;
		if (column == 64) {
			column = 0;
			printk("\n");
		}
	}

	COLOR(DEFAULT);
	if (column != 0) {
		printk("\n");
	}
}
/* LCOV_EXCL_STOP */

/**
 * @def VIRT_FOREACH
 * @brief 遍历虚拟地址范围内每一页的宏
 *
 * @param _base 虚拟地址起始值
 * @param _size 区域总大小（字节）
 * @param _pos  循环变量，指向当前页的虚拟地址
 */
#define VIRT_FOREACH(_base, _size, _pos) \
	for ((_pos) = (_base); \
	     (_pos) < ((uint8_t *)(_base) + (_size)); (_pos) += CONFIG_MMU_PAGE_SIZE)

/**
 * @def PHYS_FOREACH
 * @brief 遍历物理地址范围内每一页的宏
 *
 * @param _base 物理地址起始值（uintptr_t）
 * @param _size 区域总大小（字节）
 * @param _pos  循环变量，指向当前页的物理地址
 */
#define PHYS_FOREACH(_base, _size, _pos) \
	for ((_pos) = (_base); \
	     (_pos) < ((uintptr_t)(_base) + (_size)); (_pos) += CONFIG_MMU_PAGE_SIZE)


/*
 * 虚拟地址空间管理
 *
 * 以下所有函数必须在持有 z_mm_lock 的情况下调用。
 *
 * 虚拟内存总览：内核启动后，驻留在 K_MEM_KERNEL_VIRT_START 到
 * K_MEM_KERNEL_VIRT_END 的虚拟内存区域中。该区域之后到
 * CONFIG_KERNEL_VM_SIZE 限制之前的未使用虚拟内存可用于运行时内存映射。
 *
 * 如果设置了 CONFIG_ARCH_MAPS_ALL_RAM，则不仅映射内核镜像，
 * 还会为所有 RAM 建立映射。这用于特殊架构目的，不影响页帧计账或标志；
 * 唯一保证是 Zephyr 镜像之外的此类 RAM 映射不会被后续的内存映射调用干扰。
 *
 * +--------------+ <- K_MEM_VIRT_RAM_START
 * | Undefined VM | <- 可能包含辅助区域（如 x86_64 的 locore）
 * +--------------+ <- K_MEM_KERNEL_VIRT_START（通常 == K_MEM_VIRT_RAM_START）
 * | Mapping for  |
 * | main kernel  |
 * | image        |
 * |		  |
 * |		  |
 * +--------------+ <- K_MEM_VM_FREE_START
 * |              |
 * | Unused,      |
 * | Available VM |
 * |              |
 * |..............| <- mapping_pos（随着映射增加向下增长）
 * | Mapping      |
 * +--------------+
 * | Mapping      |
 * +--------------+
 * | ...          |
 * +--------------+
 * | Mapping      |
 * +--------------+ <- 映射从此处开始
 * | Reserved     | <- 特殊用途虚拟页，大小为 K_MEM_VM_RESERVED
 * +--------------+ <- K_MEM_VIRT_RAM_END
 */

/**
 * @brief 虚拟地址空间 bitmap
 *
 * 每 1 bit 对应 1 个虚拟页。用于 virt_region_alloc() 确定哪段虚拟地址
 * 可用于内存映射。
 *
 * 注意 bit #0 对应最高地址，因此分配从高地址向低地址进行。
 */
SYS_BITARRAY_DEFINE_STATIC(virt_region_bitmap,
			   CONFIG_KERNEL_VM_SIZE / CONFIG_MMU_PAGE_SIZE);

/** @brief 虚拟地址 bitmap 是否已初始化 */
static bool virt_region_inited;

/** @brief 可分配虚拟地址区域的起始地址 */
#define Z_VIRT_REGION_START_ADDR	K_MEM_VM_FREE_START

/** @brief 可分配虚拟地址区域的结束地址（排除 reserved 区域） */
#define Z_VIRT_REGION_END_ADDR		(K_MEM_VIRT_RAM_END - K_MEM_VM_RESERVED)

/**
 * @brief 将 bitmap 偏移量转换为虚拟地址
 *
 * 由于 bitmap bit #0 对应最高地址，转换时需要从 RAM 末尾向下计算。
 *
 * @param offset bitmap 中的起始偏移量
 * @param size   映射区域大小（字节）
 * @return 对应的虚拟地址
 */
static inline uintptr_t virt_from_bitmap_offset(size_t offset, size_t size)
{
	return POINTER_TO_UINT(K_MEM_VIRT_RAM_END)
	       - (offset * CONFIG_MMU_PAGE_SIZE) - size;
}

/**
 * @brief 将虚拟地址转换为 bitmap 偏移量
 *
 * @param vaddr 虚拟地址起始位置
 * @param size  区域大小（字节）
 * @return bitmap 中的偏移量
 */
static inline size_t virt_to_bitmap_offset(void *vaddr, size_t size)
{
	return (POINTER_TO_UINT(K_MEM_VIRT_RAM_END)
		- POINTER_TO_UINT(vaddr) - size) / CONFIG_MMU_PAGE_SIZE;
}

/**
 * @brief 初始化虚拟地址区域 bitmap
 *
 * 将不应通过 k_mem_map() 和 k_mem_map_phys_bare() 分配的区域
 * 标记为"已分配"，包括：
 * - reserved 区域（虚拟地址空间末尾）
 * - K_MEM_VM_FREE_START 之前的内核镜像区域
 */
static void virt_region_init(void)
{
	size_t offset, num_bits;

	if (K_MEM_VM_RESERVED > 0) {
		/* 将虚拟地址空间末尾的 reserved 区域标记为已分配 */
		num_bits = K_MEM_VM_RESERVED / CONFIG_MMU_PAGE_SIZE;
		(void)sys_bitarray_set_region(&virt_region_bitmap,
					      num_bits, 0);
	}

	/* 将 K_MEM_VM_FREE_START 之前的所有 bit 标记为已分配 */
	num_bits = POINTER_TO_UINT(K_MEM_VM_FREE_START)
		   - POINTER_TO_UINT(K_MEM_VIRT_RAM_START);
	offset = virt_to_bitmap_offset(K_MEM_VIRT_RAM_START, num_bits);
	num_bits /= CONFIG_MMU_PAGE_SIZE;
	(void)sys_bitarray_set_region(&virt_region_bitmap,
				      num_bits, offset);

	virt_region_inited = true;
}

/**
 * @brief 释放一段虚拟地址区域
 *
 * 将指定的虚拟地址区域在 bitmap 中标记为可用。
 * 如果尚未初始化则先调用 virt_region_init()。
 *
 * @param vaddr 要释放的虚拟地址起始位置
 * @param size  区域大小（字节），必须对齐到页大小
 */
static void virt_region_free(void *vaddr, size_t size)
{
	size_t offset, num_bits;
	uint8_t *vaddr_u8 = (uint8_t *)vaddr;

	if (unlikely(!virt_region_inited)) {
		virt_region_init();
	}

#ifndef CONFIG_KERNEL_DIRECT_MAP
	/* 无 DIRECT_MAP 支持时，区域必须在 bitmap 范围内 */
	__ASSERT((vaddr_u8 >= Z_VIRT_REGION_START_ADDR)
		 && ((vaddr_u8 + size - 1) < Z_VIRT_REGION_END_ADDR),
		 "invalid virtual address region %p (%zu)", vaddr_u8, size);
	if (!((vaddr_u8 >= Z_VIRT_REGION_START_ADDR)
	      && ((vaddr_u8 + size - 1) < Z_VIRT_REGION_END_ADDR))) {
		return;
	}

	offset = virt_to_bitmap_offset(vaddr, size);
	num_bits = size / CONFIG_MMU_PAGE_SIZE;
	(void)sys_bitarray_free(&virt_region_bitmap, num_bits, offset);
#else /* !CONFIG_KERNEL_DIRECT_MAP */
	/* 有 DIRECT_MAP 支持时，区域可能在虚拟地址空间之外、
	 * 完全在内、或部分重叠，需要额外处理只释放重叠部分 */
	if (((vaddr_u8 >= Z_VIRT_REGION_START_ADDR) &&
	     (vaddr_u8 < Z_VIRT_REGION_END_ADDR)) ||
	    (((vaddr_u8 + size - 1) >= Z_VIRT_REGION_START_ADDR) &&
	     ((vaddr_u8 + size - 1) < Z_VIRT_REGION_END_ADDR))) {
		uint8_t *adjusted_start = max(vaddr_u8, Z_VIRT_REGION_START_ADDR);
		uint8_t *adjusted_end = min(vaddr_u8 + size,
					    Z_VIRT_REGION_END_ADDR);
		size_t adjusted_sz = adjusted_end - adjusted_start;

		offset = virt_to_bitmap_offset(adjusted_start, adjusted_sz);
		num_bits = adjusted_sz / CONFIG_MMU_PAGE_SIZE;
		(void)sys_bitarray_free(&virt_region_bitmap, num_bits, offset);
	}
#endif /* !CONFIG_KERNEL_DIRECT_MAP */
}

/**
 * @brief 从虚拟地址空间中分配一块区域
 *
 * 从高地址向低地址分配虚拟地址空间，并支持对齐要求。
 * 如果申请的区域未完全对齐，则分配更大的区域后释放多余部分。
 *
 * @param size  需要分配的区域大小（字节），必须对齐到页大小
 * @param align 要求的对齐边界（字节），通常为 CONFIG_MMU_PAGE_SIZE
 * @return 分配到的虚拟地址起始位置；NULL 表示空间不足
 */
static void *virt_region_alloc(size_t size, size_t align)
{
	uintptr_t dest_addr;
	size_t alloc_size;
	size_t offset;
	size_t num_bits;
	int ret;

	if (unlikely(!virt_region_inited)) {
		virt_region_init();
	}

	/* 可能需要多申请一些页以保证能获得对齐的虚拟地址 */
	num_bits = (size + align - CONFIG_MMU_PAGE_SIZE) / CONFIG_MMU_PAGE_SIZE;
	alloc_size = num_bits * CONFIG_MMU_PAGE_SIZE;
	ret = sys_bitarray_alloc(&virt_region_bitmap, num_bits, &offset);
	if (ret != 0) {
		LOG_ERR("insufficient virtual address space (requested %zu)",
			size);
		return NULL;
	}

	/* bitmap bit #0 对应最高地址，因此需要向下计算起始地址 */
	dest_addr = virt_from_bitmap_offset(offset, alloc_size);

	if (alloc_size > size) {
		uintptr_t aligned_dest_addr = ROUND_UP(dest_addr, align);

		/*
		 * 对齐时的内存组织：
		 *
		 * +==============+ <- dest_addr（未对齐的分配起点）
		 * | 未使用       |
		 * |..............| <- aligned_dest_addr（对齐后的实际使用起点）
		 * |              |
		 * | 已对齐的     |
		 * | 映射区域     |
		 * |              |
		 * |..............| <- aligned_dest_addr + size（实际使用终点）
		 * | 未使用       |
		 * +==============+ <- dest_addr + alloc_size（分配终点）
		 */

		/* 释放两端的未使用区域 */
		virt_region_free(UINT_TO_POINTER(dest_addr),
				 aligned_dest_addr - dest_addr);
		if (((dest_addr + alloc_size) - (aligned_dest_addr + size)) > 0) {
			virt_region_free(UINT_TO_POINTER(aligned_dest_addr + size),
					 (dest_addr + alloc_size) - (aligned_dest_addr + size));
		}

		dest_addr = aligned_dest_addr;
	}

	/* 确保不侵入内核镜像区域 */
	if (dest_addr < POINTER_TO_UINT(Z_VIRT_REGION_START_ADDR)) {
		(void)sys_bitarray_free(&virt_region_bitmap, num_bits, offset);
		return NULL;
	}

	return UINT_TO_POINTER(dest_addr);
}

/*
 * 空闲页帧管理
 *
 * 以下所有函数必须在持有 z_mm_lock 的情况下调用。
 */

/**
 * @brief 空闲页帧链表
 *
 * TODO: 当前实现将所有空闲页帧视为等价。但存在一些用例需要合并空闲页，
 * 以便整个 SRAM bank 可以关闭以节省功耗。未来可能需要多个 slist
 * 按内存 bank 分组管理空闲物理页。每个页帧仍只有一个 snode 链接。
 */
static sys_sflist_t free_page_frame_list;

/** @brief 空闲可用页帧数量（该值可能在读取后立即过时） */
static size_t z_free_page_count;

/**
 * @def PF_ASSERT
 * @brief 页帧断言宏，附带物理地址信息
 */
#define PF_ASSERT(pf, expr, fmt, ...) \
	__ASSERT(expr, "page frame 0x%lx: " fmt, k_mem_page_frame_to_phys(pf), \
		 ##__VA_ARGS__)

/**
 * @brief 从空闲链表中获取一个页帧
 *
 * @return 指向空闲页帧的指针；NULL 表示没有空闲页帧
 */
static struct k_mem_page_frame *free_page_frame_list_get(void)
{
	sys_sfnode_t *node;
	struct k_mem_page_frame *pf = NULL;

	node = sys_sflist_get(&free_page_frame_list);
	if (node != NULL) {
		z_free_page_count--;
		pf = CONTAINER_OF(node, struct k_mem_page_frame, node);
		PF_ASSERT(pf, k_mem_page_frame_is_free(pf),
			 "on free list but not free");
		pf->va_and_flags = 0;    /**< 取出后清除所有标志和虚拟地址 */
	}

	return pf;
}

/**
 * @brief 将一个页帧放回空闲链表
 *
 * @param pf 指向要释放的页帧，必须处于 available 状态
 */
static void free_page_frame_list_put(struct k_mem_page_frame *pf)
{
	PF_ASSERT(pf, k_mem_page_frame_is_available(pf),
		 "unavailable page put on free list");

	sys_sfnode_init(&pf->node, K_MEM_PAGE_FRAME_FREE);
	sys_sflist_append(&free_page_frame_list, &pf->node);
	z_free_page_count++;
}

/**
 * @brief 初始化空闲页帧链表
 */
static void free_page_frame_list_init(void)
{
	sys_sflist_init(&free_page_frame_list);
}

/**
 * @brief 释放一个页帧到空闲链表（持有锁时调用）
 *
 * 清除页帧的所有标志并放回空闲链表。
 *
 * @param pf 指向要释放的页帧
 */
static void page_frame_free_locked(struct k_mem_page_frame *pf)
{
	pf->va_and_flags = 0;
	free_page_frame_list_put(pf);
}

/*
 * 内存映射
 */

/**
 * @brief 设置页帧为已映射状态
 *
 * 在 arch 层完成映射后调用，更新本地元数据并进行断言检查。
 * 允许 pinned 页帧的多重映射（如 VSDO 等效物），因为不需要反向映射。
 *
 * @param pf   指向页帧
 * @param addr 映射到的虚拟地址
 */
static void frame_mapped_set(struct k_mem_page_frame *pf, void *addr)
{
	PF_ASSERT(pf, !k_mem_page_frame_is_free(pf),
		  "attempted to map a page frame on the free list");
	PF_ASSERT(pf, !k_mem_page_frame_is_reserved(pf),
		  "attempted to map a reserved page frame");

	/* 允许 pinned 页帧的多重映射，因为不需要反向映射。
	 * 适用于 Zephyr 的 VSDO 等价场景。 */
	PF_ASSERT(pf, !k_mem_page_frame_is_mapped(pf) || k_mem_page_frame_is_pinned(pf),
		 "non-pinned and already mapped to %p",
		 k_mem_page_frame_to_virt(pf));

	uintptr_t flags_mask = CONFIG_MMU_PAGE_SIZE - 1;
	uintptr_t va = (uintptr_t)addr & ~flags_mask;    /**< 对齐到页边界 */

	pf->va_and_flags &= flags_mask;    /**< 保留原有非标志位 */
	pf->va_and_flags |= va | K_MEM_PAGE_FRAME_MAPPED;
}

/* LCOV_EXCL_START */

/**
 * @brief 通过遍历页帧数据库查找虚拟地址对应的物理地址
 *
 * 这是一个纯软件实现的虚拟到物理地址查找，用于 arch 层
 * 未提供高效实现时的回退方案。
 *
 * @param[in]  virt 虚拟地址
 * @param[out] phys 如果找到映射，存储对应的物理地址
 *
 * @return 0 找到有效映射；-EFAULT 虚拟地址未映射
 */
static int virt_to_page_frame(void *virt, uintptr_t *phys)
{
	uintptr_t paddr;
	struct k_mem_page_frame *pf;
	int ret = -EFAULT;

	K_MEM_PAGE_FRAME_FOREACH(paddr, pf) {
		if (k_mem_page_frame_is_mapped(pf)) {
			if (virt == k_mem_page_frame_to_virt(pf)) {
				ret = 0;
				if (phys != NULL) {
					*phys = k_mem_page_frame_to_phys(pf);
				}
				break;
			}
		}
	}

	return ret;
}
/* LCOV_EXCL_STOP */

/** @brief virt_to_page_frame() 的弱符号别名，架构层可覆盖 */
__weak FUNC_ALIAS(virt_to_page_frame, arch_page_phys_get, int);

#ifdef CONFIG_DEMAND_PAGING
static int page_frame_prepare_locked(struct k_mem_page_frame *pf, bool *dirty_ptr,
				     bool page_in, uintptr_t *location_ptr);

static inline void do_backing_store_page_in(uintptr_t location);
static inline void do_backing_store_page_out(uintptr_t location);
#endif /* CONFIG_DEMAND_PAGING */

/**
 * @brief 分配空闲页帧并映射到指定虚拟地址（匿名映射）
 *
 * 从空闲链表获取页帧；若无空闲页帧且启用 demand paging，
 * 则通过淘汰策略回收一个页帧。映射完成后可选地 pin 页帧。
 *
 * TODO: 未来可支持 copy-on-write 零页映射，避免立即分配物理页，
 * 仅在页面被实际访问时才分配。
 *
 * @param addr  目标虚拟地址
 * @param flags 映射标志（K_MEM_PERM_xxx, K_MEM_MAP_LOCK 等）
 *
 * @return 0 成功；-ENOMEM 无可用页帧
 */
static int map_anon_page(void *addr, uint32_t flags)
{
	struct k_mem_page_frame *pf;
	uintptr_t phys;
	bool lock = (flags & K_MEM_MAP_LOCK) != 0U;

	pf = free_page_frame_list_get();
	if (pf == NULL) {
#ifdef CONFIG_DEMAND_PAGING
		/* 空闲链表为空，需要淘汰一个页帧 */
		uintptr_t location;
		bool dirty;
		int ret;

		pf = k_mem_paging_eviction_select(&dirty);
		__ASSERT(pf != NULL, "failed to get a page frame");
		LOG_DBG("evicting %p at 0x%lx",
			k_mem_page_frame_to_virt(pf),
			k_mem_page_frame_to_phys(pf));
		ret = page_frame_prepare_locked(pf, &dirty, false, &location);
		if (ret != 0) {
			return -ENOMEM;
		}
		if (dirty) {
			do_backing_store_page_out(location);
		}
		pf->va_and_flags = 0;
#else
		return -ENOMEM;
#endif /* CONFIG_DEMAND_PAGING */
	}

	phys = k_mem_page_frame_to_phys(pf);
	arch_mem_map(addr, phys, CONFIG_MMU_PAGE_SIZE, flags);

	if (lock) {
		k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_PINNED);
	}
	frame_mapped_set(pf, addr);
#ifdef CONFIG_DEMAND_PAGING
	if (IS_ENABLED(CONFIG_EVICTION_TRACKING) && (!lock)) {
		k_mem_paging_eviction_add(pf);
	}
#endif

	LOG_DBG("memory mapping anon page %p -> 0x%lx", addr, phys);

	return 0;
}

/**
 * @brief 带保护页的内存映射
 *
 * 在映射区域前后各插入一个 unmapped guard page，
 * 使得越界访问会触发 page fault。支持匿名映射和已知物理地址映射。
 *
 * @param phys    物理地址（is_anon=true 时忽略）
 * @param size    映射区域大小（字节），必须页对齐
 * @param flags   映射标志
 * @param is_anon true 为匿名映射（从空闲页帧分配），
 *                false 为物理地址映射
 *
 * @return 映射区域的虚拟地址（跳过前导 guard page）；
 *         NULL 表示失败
 *
 * @note 匿名映射默认执行 memset(0)，除非设置 K_MEM_MAP_UNINIT
 * @note 用户态不可访问未初始化的匿名页（安全限制）
 */
void *k_mem_map_phys_guard(uintptr_t phys, size_t size, uint32_t flags, bool is_anon)
{
	uint8_t *dst;
	size_t total_size;
	int ret;
	k_spinlock_key_t key;
	uint8_t *pos;
	bool uninit = (flags & K_MEM_MAP_UNINIT) != 0U;

	__ASSERT(!is_anon || (is_anon && page_frames_initialized),
		 "%s called too early", __func__);
	__ASSERT((flags & K_MEM_CACHE_MASK) == 0U,
		 "%s does not support explicit cache settings", __func__);

	/* 安全检查：用户态不能访问未初始化的匿名页 */
	if (((flags & K_MEM_PERM_USER) != 0U) &&
	    ((flags & K_MEM_MAP_UNINIT) != 0U)) {
		LOG_ERR("user access to anonymous uninitialized pages is forbidden");
		return NULL;
	}
	if ((size % CONFIG_MMU_PAGE_SIZE) != 0U) {
		LOG_ERR("unaligned size %zu passed to %s", size, __func__);
		return NULL;
	}
	if (size == 0) {
		LOG_ERR("zero sized memory mapping");
		return NULL;
	}

	/* 总大小需要包含前后的 guard page */
	if (size_add_overflow(size, CONFIG_MMU_PAGE_SIZE * 2, &total_size)) {
		LOG_ERR("too large size %zu passed to %s", size, __func__);
		return NULL;
	}

	key = k_spin_lock(&z_mm_lock);

	dst = virt_region_alloc(total_size, CONFIG_MMU_PAGE_SIZE);
	if (dst == NULL) {
		/* 虚拟地址空间已满 */
		goto out;
	}

	/* 解除前后两个 guard page 的映射，确保访问它们会触发 fault */
	arch_mem_unmap(dst, CONFIG_MMU_PAGE_SIZE);
	arch_mem_unmap(dst + CONFIG_MMU_PAGE_SIZE + size,
		       CONFIG_MMU_PAGE_SIZE);

	/* 跳过前导 guard page，返回实际映射区域的起始地址 */
	dst += CONFIG_MMU_PAGE_SIZE;

	if (is_anon) {
		/* 匿名映射：从空闲页帧分配物理内存 */
		flags |= K_MEM_CACHE_WB;
#ifdef CONFIG_DEMAND_MAPPING
		if ((flags & K_MEM_MAP_LOCK) == 0) {
			/* 使用 unpaged 映射模式，不立即分配物理页 */
			flags |= K_MEM_MAP_UNPAGED;
			VIRT_FOREACH(dst, size, pos) {
				arch_mem_map(pos,
					     uninit ? ARCH_UNPAGED_ANON_UNINIT
						    : ARCH_UNPAGED_ANON_ZERO,
					     CONFIG_MMU_PAGE_SIZE, flags);
			}
			LOG_DBG("memory mapping anon pages %p to %p unpaged", dst, pos-1);
			uninit = true;    /**< 跳过下面的 memset() */
		} else
#endif
		{
			VIRT_FOREACH(dst, size, pos) {
				ret = map_anon_page(pos, flags);

				if (ret != 0) {
					/* TODO: 实现 k_mem_unmap() 后回滚已映射的页 */
					dst = NULL;
					goto out;
				}
			}
		}
	} else {
		/* 已知物理地址映射。
		 * arch_mem_map() 是 void 函数，通过 ASSERT 捕获映射错误。 */
		arch_mem_map(dst, phys, size, flags);
	}

out:
	k_spin_unlock(&z_mm_lock, key);

	if (dst != NULL && !uninit) {
		/* 将匿名映射区域清零。
		 * TODO: 如果未来实现了 COW 零页映射，此步骤可以省略 */
		memset(dst, 0, size);
	}

	return dst;
}

/**
 * @brief 解除带保护页的内存映射
 *
 * 验证 guard page 存在后，解除映射区域并释放虚拟地址空间。
 * 匿名映射还会将页帧归还空闲链表。
 *
 * @param addr   之前通过 k_mem_map_phys_guard() 返回的地址
 * @param size   映射区域大小（字节）
 * @param is_anon true 为匿名映射（回收页帧）；false 为物理地址映射
 */
void k_mem_unmap_phys_guard(void *addr, size_t size, bool is_anon)
{
	uintptr_t phys;
	uint8_t *pos;
	struct k_mem_page_frame *pf;
	k_spinlock_key_t key;
	size_t total_size;
	int ret;

	/* 地址必须能前推一个 guard page */
	__ASSERT_NO_MSG(POINTER_TO_UINT(addr) >= CONFIG_MMU_PAGE_SIZE);

	/* 验证包含两个 guard page 的完整区域是否在有效虚拟地址范围内 */
	pos = (uint8_t *)addr - CONFIG_MMU_PAGE_SIZE;
	k_mem_assert_virtual_region(pos, size + (CONFIG_MMU_PAGE_SIZE * 2));

	key = k_spin_lock(&z_mm_lock);

	/* 检查前后 guard page 是否已解除映射。
	 * 如果不是，说明这不是通过 k_mem_map_phys_guard() 创建的映射。 */
	pos = addr;
	ret = arch_page_phys_get(pos - CONFIG_MMU_PAGE_SIZE, NULL);
	if (ret == 0) {
		__ASSERT(ret == 0,
			 "%s: cannot find preceding guard page for (%p, %zu)",
			 __func__, addr, size);
		goto out;
	}

	ret = arch_page_phys_get(pos + size, NULL);
	if (ret == 0) {
		__ASSERT(ret == 0,
			 "%s: cannot find succeeding guard page for (%p, %zu)",
			 __func__, addr, size);
		goto out;
	}

	if (is_anon) {
		/* 解除匿名映射：逐页解除映射并回收页帧 */
		VIRT_FOREACH(addr, size, pos) {
#ifdef CONFIG_DEMAND_PAGING
			enum arch_page_location status;
			uintptr_t location;

			status = arch_page_location_get(pos, &location);
			switch (status) {
			case ARCH_PAGE_LOCATION_PAGED_OUT:
				/*
				 * 页面已被换出，没有关联的页帧。
				 * 直接删除 MMU 条目并释放 backing store 空间。
				 */
				arch_mem_unmap(pos, CONFIG_MMU_PAGE_SIZE);
				k_mem_paging_backing_store_location_free(location);
				continue;
			case ARCH_PAGE_LOCATION_PAGED_IN:
				/*
				 * 页面在内存中，但由于 ARCH_DATA_PAGE_ACCESSED
				 * 标志跟踪，arch_page_phys_get() 可能失败。
				 * 但我们已知实际物理地址。
				 */
				phys = location;
				ret = 0;
				break;
			default:
				ret = arch_page_phys_get(pos, &phys);
				break;
			}
#else
			ret = arch_page_phys_get(pos, &phys);
#endif
			__ASSERT(ret == 0,
				 "%s: cannot unmap an unmapped address %p",
				 __func__, pos);
			if (ret != 0) {
				/* 发现未映射的地址，停止处理 */
				goto out;
			}

			__ASSERT(k_mem_is_page_frame(phys),
				 "%s: 0x%lx is not a page frame", __func__, phys);
			if (!k_mem_is_page_frame(phys)) {
				goto out;
			}

			pf = k_mem_phys_to_page_frame(phys);

			__ASSERT(k_mem_page_frame_is_mapped(pf),
				 "%s: 0x%lx is not a mapped page frame", __func__, phys);
			if (!k_mem_page_frame_is_mapped(pf)) {
				goto out;
			}

			arch_mem_unmap(pos, CONFIG_MMU_PAGE_SIZE);
#ifdef CONFIG_DEMAND_PAGING
			if (IS_ENABLED(CONFIG_EVICTION_TRACKING) &&
			    (!k_mem_page_frame_is_pinned(pf))) {
				k_mem_paging_eviction_remove(pf);
			}
#endif

			/* 将页帧归还空闲链表 */
			page_frame_free_locked(pf);
		}
	} else {
		/*
		 * 解除已知物理地址的映射。
		 * guard page 应该已经被 unmapped，只需解除中间区域。
		 */
		arch_mem_unmap(addr, size);
	}

	/* 释放包含前后 guard page 的完整虚拟地址区域 */
	pos = (uint8_t *)addr - CONFIG_MMU_PAGE_SIZE;
	total_size = size + (CONFIG_MMU_PAGE_SIZE * 2);
	virt_region_free(pos, total_size);

out:
	k_spin_unlock(&z_mm_lock, key);
}

/**
 * @brief 更新已映射内存区域的权限标志
 *
 * 通过先解除映射再以新标志重新映射同一物理内存来实现。
 *
 * @param addr  虚拟地址起始位置
 * @param size  区域大小（字节）
 * @param flags 新的映射标志
 *
 * @return 0 成功；负值表示错误（如地址未映射）
 *
 * @note 未处理 paged-out 内存（TODO）
 */
int k_mem_update_flags(void *addr, size_t size, uint32_t flags)
{
	uintptr_t phys;
	k_spinlock_key_t key;
	int ret;

	k_mem_assert_virtual_region(addr, size);

	key = k_spin_lock(&z_mm_lock);

	/*
	 * 通过解除映射后以新标志重新映射同一物理内存来实现标志更新，
	 * 无需架构层的显式支持。
	 */
	ret = arch_page_phys_get(addr, &phys);
	if (ret < 0) {
		goto out;
	}

	/* TODO: 检测并处理 paged-out 内存 */

	arch_mem_unmap(addr, size);
	arch_mem_map(addr, phys, size, flags);

out:
	k_spin_unlock(&z_mm_lock, key);
	return ret;
}

/**
 * @brief 获取当前可用的空闲内存大小
 *
 * @return 空闲内存的字节数
 *
 * @note 启用 CONFIG_DEMAND_PAGING 时，需要保留一定数量的页帧
 *       （CONFIG_DEMAND_PAGING_PAGE_FRAMES_RESERVE），不计入可用量。
 */
size_t k_mem_free_get(void)
{
	size_t ret;
	k_spinlock_key_t key;

	__ASSERT(page_frames_initialized, "%s called too early", __func__);

	key = k_spin_lock(&z_mm_lock);
#ifdef CONFIG_DEMAND_PAGING
	/* 扣除 demand paging 预留的页帧 */
	if (z_free_page_count > CONFIG_DEMAND_PAGING_PAGE_FRAMES_RESERVE) {
		ret = z_free_page_count - CONFIG_DEMAND_PAGING_PAGE_FRAMES_RESERVE;
	} else {
		ret = 0;
	}
#else
	ret = z_free_page_count;
#endif /* CONFIG_DEMAND_PAGING */
	k_spin_unlock(&z_mm_lock, key);

	return ret * (size_t)CONFIG_MMU_PAGE_SIZE;
}

/**
 * @brief 获取默认的虚拟区域对齐要求
 *
 * 默认返回页大小。架构层可通过覆盖 arch_virt_region_align() 提供
 * 更大的对齐要求（如 superpage 对齐）。
 *
 * @param[in] phys 要映射区域的物理地址（页对齐）
 * @param[in] size 要映射区域的大小（页对齐）
 *
 * @return 虚拟地址应对齐的字节数
 */
static size_t virt_region_align(uintptr_t phys, size_t size)
{
	ARG_UNUSED(phys);
	ARG_UNUSED(size);

	return CONFIG_MMU_PAGE_SIZE;
}

/** @brief virt_region_align() 的弱符号别名 */
__weak FUNC_ALIAS(virt_region_align, arch_virt_region_align, size_t);

/**
 * @brief 将物理地址映射到虚拟地址空间（基础版，无 guard page）
 *
 * 可在 arch 早期启动代码中调用（z_cstart() 之前），不依赖初始化函数。
 * 如果启用了 CONFIG_KERNEL_DIRECT_MAP 且设置了 K_MEM_DIRECT_MAP 标志，
 * 则使用恒等映射。
 *
 * @param[out] virt_ptr 返回映射后的虚拟地址（含偏移调整）
 * @param[in]  phys     物理地址
 * @param[in]  size     映射大小（字节）
 * @param[in]  flags    映射标志
 *
 * @note 失败时调用 k_panic()（虚拟地址空间耗尽或 arch 映射失败）
 */
void k_mem_map_phys_bare(uint8_t **virt_ptr, uintptr_t phys, size_t size, uint32_t flags)
{
	uintptr_t aligned_phys, addr_offset;
	size_t aligned_size, align_boundary;
	k_spinlock_key_t key;
	uint8_t *dest_addr;
	size_t num_bits;
	size_t offset;

#ifndef CONFIG_KERNEL_DIRECT_MAP
	__ASSERT(!(flags & K_MEM_DIRECT_MAP), "The direct-map is not enabled");
#endif /* CONFIG_KERNEL_DIRECT_MAP */

	/* 将物理地址和大小按页对齐，计算偏移量 */
	addr_offset = k_mem_region_align(&aligned_phys, &aligned_size,
					 phys, size,
					 CONFIG_MMU_PAGE_SIZE);
	__ASSERT(aligned_size != 0U, "0-length mapping at 0x%lx", aligned_phys);
	__ASSERT(aligned_phys < (aligned_phys + (aligned_size - 1)),
		 "wraparound for physical address 0x%lx (size %zu)",
		 aligned_phys, aligned_size);

	align_boundary = arch_virt_region_align(aligned_phys, aligned_size);

	key = k_spin_lock(&z_mm_lock);

	if (IS_ENABLED(CONFIG_KERNEL_DIRECT_MAP) &&
	    (flags & K_MEM_DIRECT_MAP)) {
		/* Direct Map 模式：虚拟地址等于物理地址 */
		dest_addr = (uint8_t *)aligned_phys;

		/* 如果映射区域与虚拟地址空间重叠，标记对应的 bitmap 位 */
		if (IN_RANGE(aligned_phys,
			      (uintptr_t)K_MEM_VIRT_RAM_START,
			      (uintptr_t)(K_MEM_VIRT_RAM_END - 1)) ||
		    IN_RANGE(aligned_phys + aligned_size - 1,
			      (uintptr_t)K_MEM_VIRT_RAM_START,
			      (uintptr_t)(K_MEM_VIRT_RAM_END - 1))) {
			uint8_t *adjusted_start = max(dest_addr, K_MEM_VIRT_RAM_START);
			uint8_t *adjusted_end = min(dest_addr + aligned_size,
						    K_MEM_VIRT_RAM_END);
			size_t adjusted_sz = adjusted_end - adjusted_start;

			num_bits = adjusted_sz / CONFIG_MMU_PAGE_SIZE;
			offset = virt_to_bitmap_offset(adjusted_start, adjusted_sz);
			if (sys_bitarray_test_and_set_region(
			    &virt_region_bitmap, num_bits, offset, true)) {
				goto fail;
			}
		}
	} else {
		/* 正常模式：从虚拟地址空间中分配一块区域 */
		dest_addr = virt_region_alloc(aligned_size, align_boundary);
		if (!dest_addr) {
			goto fail;
		}
	}

	__ASSERT((uintptr_t)dest_addr <
		 ((uintptr_t)dest_addr + (size - 1)),
		 "wraparound for virtual address %p (size %zu)",
		 dest_addr, size);

	LOG_DBG("arch_mem_map(%p, 0x%lx, %zu, %x) offset %lu", (void *)dest_addr,
		aligned_phys, aligned_size, flags, addr_offset);

	arch_mem_map(dest_addr, aligned_phys, aligned_size, flags);
	k_spin_unlock(&z_mm_lock, key);

	*virt_ptr = dest_addr + addr_offset;
	return;
fail:
	/*
	 * 虚拟地址空间耗尽或 arch_mem_map() 失败是不可恢复的情况。
	 * 其他非资源耗尽问题通过断言处理（属于编程错误）。
	 */
	LOG_ERR("memory mapping 0x%lx (size %zu, flags 0x%x) failed",
		phys, size, flags);
	k_panic();
}

/**
 * @brief 解除物理地址到虚拟地址的映射（基础版）
 *
 * @param virt 虚拟地址起始位置
 * @param size 映射区域大小（字节）
 */
void k_mem_unmap_phys_bare(uint8_t *virt, size_t size)
{
	uintptr_t aligned_virt, addr_offset;
	size_t aligned_size;
	k_spinlock_key_t key;

	addr_offset = k_mem_region_align(&aligned_virt, &aligned_size,
					 POINTER_TO_UINT(virt), size,
					 CONFIG_MMU_PAGE_SIZE);
	__ASSERT(aligned_size != 0U, "0-length mapping at 0x%lx", aligned_virt);
	__ASSERT(aligned_virt < (aligned_virt + (aligned_size - 1)),
		 "wraparound for virtual address 0x%lx (size %zu)",
		 aligned_virt, aligned_size);

	key = k_spin_lock(&z_mm_lock);

	LOG_DBG("arch_mem_unmap(0x%lx, %zu) offset %lu",
		aligned_virt, aligned_size, addr_offset);

	arch_mem_unmap(UINT_TO_POINTER(aligned_virt), aligned_size);
	virt_region_free(UINT_TO_POINTER(aligned_virt), aligned_size);
	k_spin_unlock(&z_mm_lock, key);
}

/*
 * 杂项工具函数
 */

/**
 * @brief 对地址和大小进行页对齐
 *
 * 将地址向下对齐到 align 边界，并相应调整大小以确保覆盖原始范围。
 *
 * @param[out] aligned_addr 对齐后的地址
 * @param[out] aligned_size 对齐后的大小
 * @param[in]  addr         原始地址
 * @param[in]  size         原始大小
 * @param[in]  align        对齐边界
 *
 * @return 原始地址与对齐地址之间的偏移量
 */
size_t k_mem_region_align(uintptr_t *aligned_addr, size_t *aligned_size,
			  uintptr_t addr, size_t size, size_t align)
{
	size_t addr_offset;

	/* 将物理地址向下对齐，并扩展大小以覆盖原始范围 */
	*aligned_addr = ROUND_DOWN(addr, align);
	addr_offset = addr - *aligned_addr;
	*aligned_size = ROUND_UP(size + addr_offset, align);

	return addr_offset;
}

#if defined(CONFIG_LINKER_USE_BOOT_SECTION) || defined(CONFIG_LINKER_USE_PINNED_SECTION)

/**
 * @brief 设置或清除 linker section 对应页帧的 pinned 标志
 *
 * 遍历指定虚拟地址范围内的每一页，更新对应页帧的 mapped 和 pinned 状态。
 *
 * @param start_addr section 的起始虚拟地址
 * @param end_addr   section 的结束虚拟地址
 * @param pin        true 设置 PINNED；false 清除 PINNED
 */
static void mark_linker_section_pinned(void *start_addr, void *end_addr,
				       bool pin)
{
	struct k_mem_page_frame *pf;
	uint8_t *addr;

	uintptr_t pinned_start = ROUND_DOWN(POINTER_TO_UINT(start_addr),
					    CONFIG_MMU_PAGE_SIZE);
	uintptr_t pinned_end = ROUND_UP(POINTER_TO_UINT(end_addr),
					CONFIG_MMU_PAGE_SIZE);
	size_t pinned_size = pinned_end - pinned_start;

	VIRT_FOREACH(UINT_TO_POINTER(pinned_start), pinned_size, addr)
	{
		pf = k_mem_phys_to_page_frame(K_MEM_BOOT_VIRT_TO_PHYS(addr));
		frame_mapped_set(pf, addr);

		if (pin) {
			k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_PINNED);
		} else {
			k_mem_page_frame_clear(pf, K_MEM_PAGE_FRAME_PINNED);
#ifdef CONFIG_DEMAND_PAGING
			/* 解除 pin 后，如果可淘汰则加入淘汰跟踪 */
			if (IS_ENABLED(CONFIG_EVICTION_TRACKING) &&
			    k_mem_page_frame_is_evictable(pf)) {
				k_mem_paging_eviction_add(pf);
			}
#endif
		}
	}
}
#endif /* CONFIG_LINKER_USE_BOOT_SECTION) || CONFIG_LINKER_USE_PINNED_SECTION */

#ifdef CONFIG_LINKER_USE_ONDEMAND_SECTION

/**
 * @brief 映射 ondemand linker section
 *
 * 将 ondemand text 和 rodata section 以 unpaged 方式映射，
 * 使其在首次访问时触发 page fault 完成实际加载。
 * 页面数据来自预写入的 backing store。
 */
static void z_paging_ondemand_section_map(void)
{
	uint8_t *addr;
	size_t size;
	uintptr_t location;
	uint32_t flags;

	/* 映射 ondemand text section（可执行） */
	size = (uintptr_t)lnkr_ondemand_text_size;
	flags = K_MEM_MAP_UNPAGED | K_MEM_PERM_EXEC | K_MEM_CACHE_WB;
	VIRT_FOREACH(lnkr_ondemand_text_start, size, addr) {
		k_mem_paging_backing_store_location_query(addr, &location);
		arch_mem_map(addr, location, CONFIG_MMU_PAGE_SIZE, flags);
		sys_bitarray_set_region(&virt_region_bitmap, 1,
					virt_to_bitmap_offset(addr, CONFIG_MMU_PAGE_SIZE));
	}

	/* 映射 ondemand rodata section（只读） */
	size = (uintptr_t)lnkr_ondemand_rodata_size;
	flags = K_MEM_MAP_UNPAGED | K_MEM_CACHE_WB;
	VIRT_FOREACH(lnkr_ondemand_rodata_start, size, addr) {
		k_mem_paging_backing_store_location_query(addr, &location);
		arch_mem_map(addr, location, CONFIG_MMU_PAGE_SIZE, flags);
		sys_bitarray_set_region(&virt_region_bitmap, 1,
					virt_to_bitmap_offset(addr, CONFIG_MMU_PAGE_SIZE));
	}
}
#endif /* CONFIG_LINKER_USE_ONDEMAND_SECTION */

/**
 * @brief 页帧管理子系统初始化
 *
 * 在主线程入口处调用（POST_KERNEL 之前），完成以下工作：
 * 1. 初始化空闲页帧链表
 * 2. 标记架构层保留的页帧
 * 3. 将内核镜像的页帧标记为 mapped + pinned
 * 4. 标记 boot/pinned linker section
 * 5. 将剩余可用页帧加入空闲链表
 * 6. 初始化 demand paging 子系统（如果启用）
 * 7. 映射 ondemand section（如果启用）
 */
void z_mem_manage_init(void)
{
	uintptr_t phys;
	uint8_t *addr;
	struct k_mem_page_frame *pf;
	k_spinlock_key_t key = k_spin_lock(&z_mm_lock);

	free_page_frame_list_init();

	ARG_UNUSED(addr);

#ifdef CONFIG_ARCH_HAS_RESERVED_PAGE_FRAMES
	/* 架构层标记不可用页帧为 RESERVED */
	arch_reserved_pages_update();
#endif /* CONFIG_ARCH_HAS_RESERVED_PAGE_FRAMES */

#ifdef CONFIG_LINKER_GENERIC_SECTIONS_PRESENT_AT_BOOT
	/*
	 * 所有组成 Zephyr 镜像的页在启动时以可预测方式映射。
	 * 运行期间可能改变。
	 */
	VIRT_FOREACH(K_MEM_KERNEL_VIRT_START, K_MEM_KERNEL_VIRT_SIZE, addr)
	{
		pf = k_mem_phys_to_page_frame(K_MEM_BOOT_VIRT_TO_PHYS(addr));
		frame_mapped_set(pf, addr);

		/* TODO: 当前 pin 整个内核镜像。Demand paging 目前仅测试匿名映射页。
		 * 未来需要设置 linker region 标记关键 CPU 数据结构和
		 * page fault 处理代码为 pinned，其余内核代码可被淘汰。 */
		k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_PINNED);
	}
#endif /* CONFIG_LINKER_GENERIC_SECTIONS_PRESENT_AT_BOOT */

#ifdef CONFIG_LINKER_USE_BOOT_SECTION
	/* 启动期间 pin boot section 防止被换出，启动完成后 unpin */
	mark_linker_section_pinned(lnkr_boot_start, lnkr_boot_end, true);
#endif /* CONFIG_LINKER_USE_BOOT_SECTION */

#ifdef CONFIG_LINKER_USE_PINNED_SECTION
	/* Pin pinned linker section 对应的页帧 */
	mark_linker_section_pinned(lnkr_pinned_start, lnkr_pinned_end, true);
#endif /* CONFIG_LINKER_USE_PINNED_SECTION */

	/* 将所有未被映射、保留或 pin 的页帧加入空闲链表 */
	K_MEM_PAGE_FRAME_FOREACH(phys, pf) {
		if (k_mem_page_frame_is_available(pf)) {
			free_page_frame_list_put(pf);
		}
	}
	LOG_DBG("free page frames: %zu", z_free_page_count);

#ifdef CONFIG_DEMAND_PAGING
#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
	z_paging_histogram_init();
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */
	k_mem_paging_backing_store_init();
	k_mem_paging_eviction_init();

	if (IS_ENABLED(CONFIG_EVICTION_TRACKING)) {
		/* 将上面安装的可淘汰页帧加入淘汰跟踪 */
		K_MEM_PAGE_FRAME_FOREACH(phys, pf) {
			if (k_mem_page_frame_is_evictable(pf)) {
				k_mem_paging_eviction_add(pf);
			}
		}
	}
#endif /* CONFIG_DEMAND_PAGING */

#ifdef CONFIG_LINKER_USE_ONDEMAND_SECTION
	z_paging_ondemand_section_map();
#endif

#if __ASSERT_ON
	page_frames_initialized = true;
#endif
	k_spin_unlock(&z_mm_lock, key);

#ifndef CONFIG_LINKER_GENERIC_SECTIONS_PRESENT_AT_BOOT
	/* 如果 BSS section 在启动时不在内存中，则未被清零。
	 * 现在 paging 机制已初始化，可以将 BSS 页面调入物理内存来清零。 */
	arch_bss_zero();
#endif /* CONFIG_LINKER_GENERIC_SECTIONS_PRESENT_AT_BOOT */
}

/**
 * @brief 启动完成后清理页帧 pin 状态
 *
 * 解除 boot section 的 pinned 状态，使其可以被 demand paging 淘汰。
 */
void z_mem_manage_boot_finish(void)
{
#ifdef CONFIG_LINKER_USE_BOOT_SECTION
	/* 启动完成后 unpin boot section，不再需要常驻内存 */
	mark_linker_section_pinned(lnkr_boot_start, lnkr_boot_end, false);
#endif /* CONFIG_LINKER_USE_BOOT_SECTION */
}

#ifdef CONFIG_DEMAND_PAGING

#ifdef CONFIG_DEMAND_PAGING_STATS
/** @brief 全局 paging 统计数据 */
struct k_mem_paging_stats_t paging_stats;
extern struct k_mem_paging_histogram_t z_paging_histogram_eviction;
extern struct k_mem_paging_histogram_t z_paging_histogram_backing_store_page_in;
extern struct k_mem_paging_histogram_t z_paging_histogram_backing_store_page_out;
#endif /* CONFIG_DEMAND_PAGING_STATS */

/**
 * @brief 执行 backing store page-in 操作（带性能统计）
 *
 * 对于 CONFIG_DEMAND_MAPPING 模式下的特殊地址（零页/未初始化页），
 * 直接在 scratch page 上填充而不访问 backing store。
 *
 * @param location backing store 中的位置，或特殊值
 *                 ARCH_UNPAGED_ANON_ZERO / ARCH_UNPAGED_ANON_UNINIT
 */
static inline void do_backing_store_page_in(uintptr_t location)
{
#ifdef CONFIG_DEMAND_MAPPING
	/* 处理 demand mapping 特殊情况 */
	switch (location) {
	case ARCH_UNPAGED_ANON_ZERO:
		memset(K_MEM_SCRATCH_PAGE, 0, CONFIG_MMU_PAGE_SIZE);
		__fallthrough;
	case ARCH_UNPAGED_ANON_UNINIT:
		/* 零页已填充或未初始化，无需其他操作 */
		return;
	default:
		break;
	}
#endif /* CONFIG_DEMAND_MAPPING */

#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
	uint32_t time_diff;

#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	timing_t time_start, time_end;

	time_start = timing_counter_get();
#else
	uint32_t time_start;

	time_start = k_cycle_get_32();
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */

	k_mem_paging_backing_store_page_in(location);

#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	time_end = timing_counter_get();
	time_diff = (uint32_t)timing_cycles_get(&time_start, &time_end);
#else
	time_diff = k_cycle_get_32() - time_start;
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */

	z_paging_histogram_inc(&z_paging_histogram_backing_store_page_in,
			       time_diff);
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */
}

/**
 * @brief 执行 backing store page-out 操作（带性能统计）
 *
 * @param location backing store 中的目标位置
 */
static inline void do_backing_store_page_out(uintptr_t location)
{
#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
	uint32_t time_diff;

#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	timing_t time_start, time_end;

	time_start = timing_counter_get();
#else
	uint32_t time_start;

	time_start = k_cycle_get_32();
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */

	k_mem_paging_backing_store_page_out(location);

#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	time_end = timing_counter_get();
	time_diff = (uint32_t)timing_cycles_get(&time_start, &time_end);
#else
	time_diff = k_cycle_get_32() - time_start;
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */

	z_paging_histogram_inc(&z_paging_histogram_backing_store_page_out,
			       time_diff);
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */
}

#if defined(CONFIG_SMP) && defined(CONFIG_DEMAND_PAGING_ALLOW_IRQ)
/**
 * @brief SMP 下的全局 demand paging 互斥锁
 *
 * SMP 环境下需要全局串行化 demand paging 操作。虽然理论上可以让
 * scratch page 按核分配、限制 backing store 驱动在故障核上执行等，
 * 但最终都要进行外部存储的内存传输，这本身就是慢操作且大多串行化。
 * 因此简单地使用 mutex 实现全局串行化即可，不会有实际的并行收益。
 */
static K_MUTEX_DEFINE(z_mm_paging_lock);
#endif

/**
 * @brief 对指定虚拟地址范围内的每一页执行回调函数
 *
 * @param addr 要处理的虚拟地址起始位置
 * @param size 区域大小（字节）
 * @param func 对每一页执行的回调函数
 */
static void virt_region_foreach(void *addr, size_t size,
				void (*func)(void *))
{
	k_mem_assert_virtual_region(addr, size);

	for (size_t offset = 0; offset < size; offset += CONFIG_MMU_PAGE_SIZE) {
		func((uint8_t *)addr + offset);
	}
}

/**
 * @brief 淘汰前的准备工作
 *
 * 在页帧被淘汰到 backing store 之前执行：
 * - 如果需要，将页帧映射到 scratch 区域
 * - 获取 backing store 位置并更新页表
 * - 标记页帧为 busy（仅 CONFIG_DEMAND_PAGING_ALLOW_IRQ）
 *
 * @param[in]     pf           要准备的页帧
 * @param[in,out] dirty_ptr    输入：页帧是否脏；输出：更新后的脏状态
 * @param[in]     page_fault   是否由 page fault 触发（总是映射 scratch）
 * @param[out]    location_ptr 输出：backing store 中的位置
 *
 * @return 0 成功；-ENOMEM backing store 空间不足
 */
static int page_frame_prepare_locked(struct k_mem_page_frame *pf, bool *dirty_ptr,
				     bool page_fault, uintptr_t *location_ptr)
{
	uintptr_t phys;
	int ret;
	bool dirty = *dirty_ptr;

	phys = k_mem_page_frame_to_phys(pf);
	__ASSERT(!k_mem_page_frame_is_pinned(pf), "page frame 0x%lx is pinned",
		 phys);

	/*
	 * 如果 backing store 中没有该页的副本，即使未被修改也视为脏页。
	 * 可能原因：
	 * 1) 页从未被换出过，backing store 未预填充此数据
	 * 2) 页曾被换出但换回时内容未保留
	 * 3) 页换回时保留了内容但后来被从 backing store 中淘汰以腾出空间
	 */
	if (k_mem_page_frame_is_mapped(pf)) {
		dirty = dirty || !k_mem_page_frame_is_backed(pf);
	}

	if (dirty || page_fault) {
		/* 需要访问页面内容时，映射到 scratch 区域 */
		arch_mem_scratch(phys);
	}

	if (k_mem_page_frame_is_mapped(pf)) {
		/* 在 backing store 中分配空间并获取位置 */
		ret = k_mem_paging_backing_store_location_get(pf, location_ptr,
							      page_fault);
		if (ret != 0) {
			LOG_ERR("out of backing store memory");
			return -ENOMEM;
		}
		/* 更新页表：将虚拟地址指向 backing store 位置 */
		arch_mem_page_out(k_mem_page_frame_to_virt(pf), *location_ptr);

		if (IS_ENABLED(CONFIG_EVICTION_TRACKING)) {
			k_mem_paging_eviction_remove(pf);
		}
	} else {
		/* 不应发生：未映射的页不应为脏 */
		__ASSERT(!dirty, "un-mapped page determined to be dirty");
	}
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	/* 标记为 busy，使 k_mem_page_frame_is_evictable() 返回 false */
	__ASSERT(!k_mem_page_frame_is_busy(pf), "page frame 0x%lx is already busy",
		 phys);
	k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_BUSY);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	/* 更新脏标志（可能因未 backed 而被设为 true） */
	*dirty_ptr = dirty;

	return 0;
}

/**
 * @brief 淘汰指定虚拟地址对应的页面
 *
 * 获取页面信息，准备淘汰，执行 backing store 写回（脏页），
 * 然后将页帧归还空闲链表。
 *
 * @param addr 要淘汰的虚拟地址
 *
 * @return 0 成功；负值表示错误
 *
 * @note 如果启用了 CONFIG_DEMAND_PAGING_ALLOW_IRQ，在 I/O 操作期间
 *       释放自旋锁以允许中断（SMP 用 mutex，UP 用 sched_lock 串行化）。
 */
static int do_mem_evict(void *addr)
{
	bool dirty;
	struct k_mem_page_frame *pf;
	uintptr_t location;
	k_spinlock_key_t key;
	uintptr_t flags, phys;
	int ret;

#if CONFIG_DEMAND_PAGING_ALLOW_IRQ
	__ASSERT(!k_is_in_isr(),
		 "%s is unavailable in ISRs with CONFIG_DEMAND_PAGING_ALLOW_IRQ",
		 __func__);
#ifdef CONFIG_SMP
	k_mutex_lock(&z_mm_paging_lock, K_FOREVER);
#else
	k_sched_lock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	key = k_spin_lock(&z_mm_lock);
	flags = arch_page_info_get(addr, &phys, false);
	__ASSERT((flags & ARCH_DATA_PAGE_NOT_MAPPED) == 0,
		 "address %p isn't mapped", addr);
	if ((flags & ARCH_DATA_PAGE_LOADED) == 0) {
		/* 未映射或已被淘汰，无需操作 */
		ret = 0;
		goto out;
	}

	dirty = (flags & ARCH_DATA_PAGE_DIRTY) != 0;
	pf = k_mem_phys_to_page_frame(phys);
	__ASSERT(k_mem_page_frame_to_virt(pf) == addr, "page frame address mismatch");
	ret = page_frame_prepare_locked(pf, &dirty, false, &location);
	if (ret != 0) {
		goto out;
	}

	__ASSERT(ret == 0, "failed to prepare page frame");
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	/* 在 I/O 操作期间释放自旋锁以允许中断处理 */
	k_spin_unlock(&z_mm_lock, key);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	if (dirty) {
		do_backing_store_page_out(location);
	}
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	key = k_spin_lock(&z_mm_lock);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	/* 将页帧归还空闲链表 */
	page_frame_free_locked(pf);
out:
	k_spin_unlock(&z_mm_lock, key);
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
#ifdef CONFIG_SMP
	k_mutex_unlock(&z_mm_paging_lock);
#else
	k_sched_unlock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	return ret;
}

/**
 * @brief 将虚拟地址范围内的页面主动换出到 backing store
 *
 * @param addr 虚拟地址起始位置
 * @param size 区域大小（字节）
 *
 * @return 0 成功；负值表示错误
 */
int k_mem_page_out(void *addr, size_t size)
{
	__ASSERT(page_frames_initialized, "%s called on %p too early", __func__,
		 addr);
	k_mem_assert_virtual_region(addr, size);

	for (size_t offset = 0; offset < size; offset += CONFIG_MMU_PAGE_SIZE) {
		void *pos = (uint8_t *)addr + offset;
		int ret;

		ret = do_mem_evict(pos);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

/**
 * @brief 淘汰指定物理地址对应的页帧
 *
 * 与 do_page_fault() 类似，但没有需要换入的数据页。
 * 如果页帧未映射则直接返回成功。
 *
 * @param phys 要淘汰的页帧物理地址
 *
 * @return 0 成功；-ENOMEM backing store 空间不足
 */
int k_mem_page_frame_evict(uintptr_t phys)
{
	k_spinlock_key_t key;
	struct k_mem_page_frame *pf;
	bool dirty;
	uintptr_t flags;
	uintptr_t location;
	int ret;

	__ASSERT(page_frames_initialized, "%s called on 0x%lx too early",
		 __func__, phys);

#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	__ASSERT(!k_is_in_isr(),
		 "%s is unavailable in ISRs with CONFIG_DEMAND_PAGING_ALLOW_IRQ",
		 __func__);
#ifdef CONFIG_SMP
	k_mutex_lock(&z_mm_paging_lock, K_FOREVER);
#else
	k_sched_lock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	key = k_spin_lock(&z_mm_lock);
	pf = k_mem_phys_to_page_frame(phys);
	if (!k_mem_page_frame_is_mapped(pf)) {
		/* 未映射，无需操作 */
		ret = 0;
		goto out;
	}
	flags = arch_page_info_get(k_mem_page_frame_to_virt(pf), NULL, false);
	/* 不应发生 */
	__ASSERT((flags & ARCH_DATA_PAGE_LOADED) != 0, "data page not loaded");
	dirty = (flags & ARCH_DATA_PAGE_DIRTY) != 0;
	ret = page_frame_prepare_locked(pf, &dirty, false, &location);
	if (ret != 0) {
		goto out;
	}

#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	k_spin_unlock(&z_mm_lock, key);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	if (dirty) {
		do_backing_store_page_out(location);
	}
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	key = k_spin_lock(&z_mm_lock);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	page_frame_free_locked(pf);
out:
	k_spin_unlock(&z_mm_lock, key);
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
#ifdef CONFIG_SMP
	k_mutex_unlock(&z_mm_paging_lock);
#else
	k_sched_unlock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
	return ret;
}

/**
 * @brief 递增 page fault 统计计数器
 *
 * @param faulting_thread 触发 fault 的线程
 * @param key             进入 fault 时的中断状态键值
 */
static inline void paging_stats_faults_inc(struct k_thread *faulting_thread,
					   int key)
{
#ifdef CONFIG_DEMAND_PAGING_STATS
	bool is_irq_unlocked = arch_irq_unlocked(key);

	paging_stats.pagefaults.cnt++;

	if (is_irq_unlocked) {
		paging_stats.pagefaults.irq_unlocked++;
	} else {
		paging_stats.pagefaults.irq_locked++;
	}

#ifdef CONFIG_DEMAND_PAGING_THREAD_STATS
	faulting_thread->paging_stats.pagefaults.cnt++;

	if (is_irq_unlocked) {
		faulting_thread->paging_stats.pagefaults.irq_unlocked++;
	} else {
		faulting_thread->paging_stats.pagefaults.irq_locked++;
	}
#else
	ARG_UNUSED(faulting_thread);
#endif /* CONFIG_DEMAND_PAGING_THREAD_STATS */

#ifndef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	if (k_is_in_isr()) {
		paging_stats.pagefaults.in_isr++;

#ifdef CONFIG_DEMAND_PAGING_THREAD_STATS
		faulting_thread->paging_stats.pagefaults.in_isr++;
#endif /* CONFIG_DEMAND_PAGING_THREAD_STATS */
	}
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */
#endif /* CONFIG_DEMAND_PAGING_STATS */
}

/**
 * @brief 递增淘汰统计计数器
 *
 * @param faulting_thread 触发淘汰的线程
 * @param dirty           是否为脏页淘汰
 */
static inline void paging_stats_eviction_inc(struct k_thread *faulting_thread,
					     bool dirty)
{
#ifdef CONFIG_DEMAND_PAGING_STATS
	if (dirty) {
		paging_stats.eviction.dirty++;
	} else {
		paging_stats.eviction.clean++;
	}
#ifdef CONFIG_DEMAND_PAGING_THREAD_STATS
	if (dirty) {
		faulting_thread->paging_stats.eviction.dirty++;
	} else {
		faulting_thread->paging_stats.eviction.clean++;
	}
#else
	ARG_UNUSED(faulting_thread);
#endif /* CONFIG_DEMAND_PAGING_THREAD_STATS */
#endif /* CONFIG_DEMAND_PAGING_STATS */
}

/**
 * @brief 选择一个页帧进行淘汰（带性能统计）
 *
 * @param[out] dirty 输出：被选中页帧是否为脏
 *
 * @return 被选中淘汰的页帧
 */
static inline struct k_mem_page_frame *do_eviction_select(bool *dirty)
{
	struct k_mem_page_frame *pf;

#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
	uint32_t time_diff;

#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	timing_t time_start, time_end;

	time_start = timing_counter_get();
#else
	uint32_t time_start;

	time_start = k_cycle_get_32();
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */

	pf = k_mem_paging_eviction_select(dirty);

#ifdef CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM
#ifdef CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS
	time_end = timing_counter_get();
	time_diff = (uint32_t)timing_cycles_get(&time_start, &time_end);
#else
	time_diff = k_cycle_get_32() - time_start;
#endif /* CONFIG_DEMAND_PAGING_STATS_USING_TIMING_FUNCTIONS */

	z_paging_histogram_inc(&z_paging_histogram_eviction, time_diff);
#endif /* CONFIG_DEMAND_PAGING_TIMING_HISTOGRAM */

	return pf;
}

/**
 * @brief Page fault 核心处理函数
 *
 * 处理流程：
 * 1. 查询页面当前状态（paged out / paged in / bad）
 * 2. 如果已 paged in，可选地 pin 后返回
 * 3. 如果 paged out：
 *    a. 尝试从空闲链表获取页帧
 *    b. 空闲链表为空则通过淘汰策略选择牺牲页
 *    c. 准备被淘汰页帧（映射到 scratch，写回脏数据）
 *    d. 从 backing store 读入目标页到 scratch page
 *    e. 更新页表映射到新的物理页
 *    f. 更新页帧元数据
 *
 * 关于 CONFIG_DEMAND_PAGING_ALLOW_IRQ 的详细说明：
 * - 仅在异常触发时中断已解锁的情况下才重新使能中断
 * - ISR 中的 page fault 是 bug（ISR 代码/数据必须 pin）
 * - UP 下通过 k_sched_lock() 防止其他线程被调度
 * - SMP 下通过 mutex 全局串行化（因为另一核可能运行任意线程）
 *
 * @param addr 触发 fault 的虚拟地址
 * @param pin  true 表示需要 pin 该页面（不可淘汰）
 *
 * @return true fault 处理成功；false 地址无效（致命错误）
 */
static bool do_page_fault(void *addr, bool pin)
{
	struct k_mem_page_frame *pf;
	k_spinlock_key_t key;
	uintptr_t page_in_location, page_out_location;
	enum arch_page_location status;
	bool result;
	bool dirty = false;
	struct k_thread *faulting_thread;
	int ret;

	__ASSERT(page_frames_initialized, "page fault at %p happened too early",
		 addr);

	LOG_DBG("page fault at %p", addr);

#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	/*
	 * 仅在异常触发时中断已解锁时才重新使能中断；
	 * ISR 中的 page fault 是 bug（ISR 代码/数据必须 pin）。
	 *
	 * UP：锁调度器防止其他线程运行。不支持 backing store 驱动睡眠。
	 * SMP：无法仅靠合作线程保证互斥，必须用 mutex。睡眠/调度是可以接受的。
	 */
	__ASSERT(!k_is_in_isr(), "ISR page faults are forbidden");
#ifdef CONFIG_SMP
	k_mutex_lock(&z_mm_paging_lock, K_FOREVER);
#else
	k_sched_lock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */

	key = k_spin_lock(&z_mm_lock);
	faulting_thread = _current;

	status = arch_page_location_get(addr, &page_in_location);
	if (status == ARCH_PAGE_LOCATION_BAD) {
		/* 返回 false 以触发致命错误处理 */
		result = false;
		goto out;
	}
	result = true;

	if (status == ARCH_PAGE_LOCATION_PAGED_IN) {
		if (pin) {
			/* 页已在物理内存中，获取物理地址并 pin */
			uintptr_t phys = page_in_location;

			pf = k_mem_phys_to_page_frame(phys);
			if (!k_mem_page_frame_is_pinned(pf)) {
				if (IS_ENABLED(CONFIG_EVICTION_TRACKING)) {
					k_mem_paging_eviction_remove(pf);
				}
				k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_PINNED);
			}
		}

		/* 页已在内存中，无需换入操作，直接返回 */
		goto out;
	}
	__ASSERT(status == ARCH_PAGE_LOCATION_PAGED_OUT,
		 "unexpected status value %d", status);

	/* 记录 page fault 统计 */
	paging_stats_faults_inc(faulting_thread, key.key);

	/* 尝试获取空闲页帧 */
	pf = free_page_frame_list_get();
	if (pf == NULL) {
		/* 空闲链表为空，需要淘汰一个页帧 */
		pf = do_eviction_select(&dirty);
		__ASSERT(pf != NULL, "failed to get a page frame");
		LOG_DBG("evicting %p at 0x%lx",
			k_mem_page_frame_to_virt(pf),
			k_mem_page_frame_to_phys(pf));

		paging_stats_eviction_inc(faulting_thread, dirty);
	}

	/* 准备被淘汰页帧（映射 scratch、获取 backing store 位置等） */
	ret = page_frame_prepare_locked(pf, &dirty, true, &page_out_location);
	__ASSERT(ret == 0, "failed to prepare page frame");

#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	/* 在 I/O 操作期间释放自旋锁以允许中断 */
	k_spin_unlock(&z_mm_lock, key);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */

	/* 执行 backing store I/O */
	if (dirty) {
		do_backing_store_page_out(page_out_location);    /**< 写回脏数据 */
	}
	do_backing_store_page_in(page_in_location);         /**< 读入目标页 */

#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
	key = k_spin_lock(&z_mm_lock);
	k_mem_page_frame_clear(pf, K_MEM_PAGE_FRAME_BUSY);
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */

	/* 更新页帧元数据：解除旧映射，建立新映射 */
	k_mem_page_frame_clear(pf, K_MEM_PAGE_FRAME_MAPPED);
	frame_mapped_set(pf, addr);
	if (pin) {
		k_mem_page_frame_set(pf, K_MEM_PAGE_FRAME_PINNED);
	}

	/* 更新页表：将虚拟地址映射到新的物理页 */
	arch_mem_page_in(addr, k_mem_page_frame_to_phys(pf));
	k_mem_paging_backing_store_page_finalize(pf, page_in_location);
	if (IS_ENABLED(CONFIG_EVICTION_TRACKING) && (!pin)) {
		k_mem_paging_eviction_add(pf);
	}
out:
	k_spin_unlock(&z_mm_lock, key);
#ifdef CONFIG_DEMAND_PAGING_ALLOW_IRQ
#ifdef CONFIG_SMP
	k_mutex_unlock(&z_mm_paging_lock);
#else
	k_sched_unlock();
#endif
#endif /* CONFIG_DEMAND_PAGING_ALLOW_IRQ */

	return result;
}

/**
 * @brief 将单页换入物理内存
 *
 * @param addr 要换入的虚拟地址
 */
static void do_page_in(void *addr)
{
	bool ret;

	ret = do_page_fault(addr, false);
	__ASSERT(ret, "unmapped memory address %p", addr);
	(void)ret;
}

/**
 * @brief 将虚拟地址范围内的所有页面换入物理内存
 *
 * @param addr 虚拟地址起始位置
 * @param size 区域大小（字节）
 */
void k_mem_page_in(void *addr, size_t size)
{
	__ASSERT(!IS_ENABLED(CONFIG_DEMAND_PAGING_ALLOW_IRQ) || !k_is_in_isr(),
		 "%s may not be called in ISRs if CONFIG_DEMAND_PAGING_ALLOW_IRQ is enabled",
		 __func__);
	virt_region_foreach(addr, size, do_page_in);
}

/**
 * @brief Pin 单页（标记为不可淘汰）
 *
 * @param addr 要 pin 的虚拟地址
 */
static void do_mem_pin(void *addr)
{
	bool ret;

	ret = do_page_fault(addr, true);
	__ASSERT(ret, "unmapped memory address %p", addr);
	(void)ret;
}

/**
 * @brief Pin 虚拟地址范围内的所有页面（标记为不可淘汰）
 *
 * 如果页面已在物理内存中则直接 pin；如果已被换出则先换入再 pin。
 *
 * @param addr 虚拟地址起始位置
 * @param size 区域大小（字节）
 */
void k_mem_pin(void *addr, size_t size)
{
	__ASSERT(!IS_ENABLED(CONFIG_DEMAND_PAGING_ALLOW_IRQ) || !k_is_in_isr(),
		 "%s may not be called in ISRs if CONFIG_DEMAND_PAGING_ALLOW_IRQ is enabled",
		 __func__);
	virt_region_foreach(addr, size, do_mem_pin);
}

/**
 * @brief 处理 page fault（由 arch 层异常处理程序调用）
 *
 * @param addr 触发 fault 的虚拟地址
 *
 * @return true 处理成功；false 地址无效（arch 层应视为致命错误）
 */
bool k_mem_page_fault(void *addr)
{
	return do_page_fault(addr, false);
}

/**
 * @brief Unpin 单页（允许淘汰）
 *
 * @param addr 要 unpin 的虚拟地址
 */
static void do_mem_unpin(void *addr)
{
	struct k_mem_page_frame *pf;
	k_spinlock_key_t key;
	uintptr_t flags, phys;

	key = k_spin_lock(&z_mm_lock);
	flags = arch_page_info_get(addr, &phys, false);
	__ASSERT((flags & ARCH_DATA_PAGE_NOT_MAPPED) == 0,
		 "invalid data page at %p", addr);
	if ((flags & ARCH_DATA_PAGE_LOADED) != 0) {
		pf = k_mem_phys_to_page_frame(phys);
		if (k_mem_page_frame_is_pinned(pf)) {
			k_mem_page_frame_clear(pf, K_MEM_PAGE_FRAME_PINNED);

			if (IS_ENABLED(CONFIG_EVICTION_TRACKING)) {
				/* 解除 pin 后加入淘汰跟踪 */
				k_mem_paging_eviction_add(pf);
			}
		}
	}
	k_spin_unlock(&z_mm_lock, key);
}

/**
 * @brief Unpin 虚拟地址范围内的所有页面（允许淘汰）
 *
 * @param addr 虚拟地址起始位置
 * @param size 区域大小（字节）
 */
void k_mem_unpin(void *addr, size_t size)
{
	__ASSERT(page_frames_initialized, "%s called on %p too early", __func__,
		 addr);
	virt_region_foreach(addr, size, do_mem_unpin);
}

#endif /* CONFIG_DEMAND_PAGING */
