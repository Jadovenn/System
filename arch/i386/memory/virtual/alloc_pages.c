/**
 * allocation.c - Paging allocation routines
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <hal/memory.h>

#include <arch/paging.h>
#include <cpu/cr.h>

Hal_memory_page_t* alloc_pages(size_t count, size_t flags) {
	(void)count;
	(void)flags;
	return NULL;
}

void free_pages(Hal_memory_page_t * p) { (void)p; }
