/**
 * allocation.c - Paging allocation routines
 * System sources under license MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <arch/paging.h>

#include <api/mm.h>
#include <cpu/cr.h>

pg_page_t* alloc_pages(size_t count, size_t flags) {
	(void)count;
	(void)flags;
	return NULL;
}

void free_pages(pg_page_t* p) { (void)p; }
