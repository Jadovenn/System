/**
 * mm.h - Kernel Memory Management API
 * System sources under license MIT
 */

#ifndef _K_API_MM_H_
# define _K_API_MM_H_

#include <stdint.h>

typedef enum	memory_zone {
	USER_SPACE = 0,
	KERNEL = 1,
	DMA = 2
}		memory_zone_t;

#define	FLAGE_OWNER	0x01
#define FLAG_RW		0x02

typedef struct	pg_page {
	uintptr_t	vaddr;
	uintptr_t	paddr;
	uint8_t		flags;
}		pg_page_t;

/**
 * @bried allocate size bytes over multiple page
 * @details bakcend function for *
 */
pg_page_t	*alloc_pages(size_t count, memory_zone_t zone, uint8_t flags); 

/**
 * @brief free pages allocate from allocate_page(s)
 */
void	free_page(pg_page_t *page);

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

