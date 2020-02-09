/**
 * allocation.c - Paging allocation routines
 * System sources under license MIT
 */


#include <stdint.h>
#include <stddef.h>
#include <api/mm.h>
#include <cpu/cr.h>
#include <arch/paging.h>

void	*allocate_pages(size_t size, memory_zone_t zone) {
	if (zone < 0 || zone > 1) {
		return NULL;
	}
	return NULL;
}

