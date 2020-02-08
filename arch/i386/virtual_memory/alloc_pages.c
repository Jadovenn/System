/**
 * allocation.c - Paging allocation routines
 * System sources under license MIT
 */


#include <stdint.h>
#include <stddef.h>
#include <api/mm.h>
#include <cpu/cr.h>
#include <arch/paging.h>

/**
 * @brief mark selected pages with the correct values
 */
static void	*_alloc_pages(arch_vma_t *vma, uint8_t flags) {

}

static arch_vma_t	arch_vma[] = {
	{ 0x00100000, 0xBFFFFFFF, USER_SPACE },
	{ 0xC0000000, 0xFFFFFFFF, KERNEL }
};

void	*allocate_pages(size_t size, memory_zone_t zone) {
	if (zone < 0 || zone > 1) {
		return NULL;
	}
	return _alloc_pages(&arch_vma[zone], zone);
}

