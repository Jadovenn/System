/**
 * mm.h - Kernel Memory Management API
 * System sources under license MIT
 */

#ifndef _K_API_MM_H_
# define _K_API_MM_H_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Virtual Memory Page
 */
typedef struct		pg_page {
	uintptr_t	vaddr;
	uintptr_t	paddr;
	uint8_t		flags;
} __attribute__((packed)) pg_page_t;

#define	PG_OWNER	0x01
#define PG_ACCESS	0x02
#define PG_ANONYMOUS	0x04

/**
 * @brief Virtual Memory Area Structur
 * @detail hold information on Virtual Memory Area
 */
typedef struct	vma_struct {
	uintptr_t	start;
	uintptr_t	end;
	uint8_t		flags;
	size_t		type;
}		vma_struct_t;

#define	VMA_USERSPACE	0
#define	VMA_BIOS	1
#define VMA_RESERVED	2
#define	VMA_KERNEL_IMG	3
#define VMA_LOW_MEMORY	4
#define VMA_HEAP	5
#define	VMA_DEVICES	6
#define VMA_DATA	7

/**
 * @brief Memory Structur
 * @details contain the memory structure
 * 	of a program. It Should first be initialized
 * 	for the kernel as it is considered as a program
 *
 */
typedef struct	mm_struct {
	vma_struct_t	*mmap;
}		mm_struct_t;

/**
 * @bried allocate size bytes over multiple page
 * @details bakcend function for *
 */
pg_page_t	*alloc_pages(size_t count, size_t flags); 

#define AP_DMA		0
#define AP_HEAP		1
#define AP_USERSPACE	2
#define _AP_ANONYMOUS	0x10000

/**
 * @brief free pages allocate from allocate_page(s)
 */
void	free_pages(pg_page_t *p);

/**
 * @brief allocate size bytes from virtual memory
 * @details allocate memory that is not managed by the heap,
 *	this allocator stand for big allocation.
 *	Block from vmalloc are not contiguous in physical mem.
 * @return {void*} - valide virtual addr pointer
 */
void	*vmalloc(size_t size);

/**
 * @brief free memory allocated with vmalloc
 */
void	vfree(void *addr);

#endif // _K_API_MM_H_

