/**
 * paging.h - x86 paging API
 * System sources under license MIT
 */

#ifndef PAGING_H_
# define PAGING_H_

#include <kernel/types.h>
#include <isr.h>

typedef struct page {
	uint32_t	present	 : 1; // set when the page is present
	uint32_t	rw	 : 1; // if set the fault was caused by a write, by a read otherwise
	uint32_t	user	 : 1; // if set the fault was caused in user mode, in kernel space otherwise
	uint32_t	accessed : 1; // if set the fault was caused by reserved bits being overwritten
	uint32_t	dirty	 : 1; // if set the fault was caused during an instruction fetch, too bad
	uint32_t	unused_t : 7;
	uint32_t	frame	 : 20;
}		page_t;

typedef struct	page_table {
	page_t	pages[1024];
}		page_table_t;

typedef struct page_directory {
	page_table_t	*tables[1024]; // array of pointer to pagetables
	uint32_t	tables_physical_address[1024]; // physical address for loading cr3 register
	uint32_t	physical_address; // physical address of table_physical_address

}		page_directory_t;

void	init_paging();

void	switch_page_directory(page_directory_t	*new);

page_t	*get_page(uint32 address, int make, page_directory_t *dir);

void	page_fault(register_t regs);

#endif // PAGING_H_

