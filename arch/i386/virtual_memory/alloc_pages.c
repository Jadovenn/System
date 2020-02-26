/**
 * allocation.c - Paging allocation routines
 * System sources under license MIT
 */


#include <stdint.h>
#include <stddef.h>
#include <api/mm.h>
#include <cpu/cr.h>
#include <arch/paging.h>

pg_page_t	*alloc_pages(size_t count, size_t flags) {
	(void)count;
	(void)flags;
	return NULL;
}

void	free_pages(pg_page_t *p) {
	(void)p;
}

