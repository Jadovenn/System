/**
 * mmu.h - Memory Management Unit
 * System under license MIT
 */

#ifndef MMU_H_
# define MMU_H_

#include <stdint.h>
#include <stddef.h>
#include <multiboot.h>

// define state for PTE as a bitfield
#define PTE_PRESENT	0x0001
#define	PTE_NOT_PRESENT	0x0000
#define PTE_RW		0x0002
#define PTE_RO		0x0000
#define PTE_USER	0x0004
#define PTE_KERNEL	0x0000
#define PTE_ACCESSED	0x0020
#define PTE_NOT_ACCESS	0x0000
#define PTE_DIRTY	0x0040
#define PTE_NOT_DIRTY	0x0000

// define state for PTE struct
#define PTE_STRUCT_PRESENT		1
#define PTE_STRUCT_NOT_PRESENT		0
#define PTE_STRUCT_READ_WRITE		1
#define PTE_STRUCT_READ_ONLY		0
#define PTE_STRUCT_USER			1
#define PTE_STRUCT_KERNEL		0
#define PTE_STRUCT_ACCESSED		1
#define PTE_STRUCT_NOT_ACCESSED		0
#define PTE_STRUCT_DIRTY		1
#define PTE_STRUCT_NOT_DIRTY		0

// define kernel reserved byte
#define KERN_BIT_HEAP	0x1000
#define KERN_NOT_HEAP	0
#define KERN_HEAP	1

typedef struct		page_table_entry_t {
	uint32_t	addr:		20;
	uint32_t	os_reserved:	3;
	uint32_t	reserved_1:	2;
	uint32_t	dirirty:	1;
	uint32_t	access:		1;
	uint32_t	reserved_2:	2;
	uint32_t	priviledge:	1;
	uint32_t	read_only:	1;
	uint32_t	present:	1;
}			page_table_entry;

typedef struct		page_table_t {
	page_table_entry	pte[1024];
	void			*pte_physical;
}			page_table;

typedef struct		page_directory_t {
	page_table	*page_tables;
}			page_directory;

#define PHYS_MEMORY_AVAILABLE	1
#define PHYS_MEMORY_RESERVED	2
#define PHYS_MEMORY_ACPI	3
#define PHYS_MEMORY_NVS		4
#define PHYS_MEMORY_BADRAM	5

#endif // MMU_H_

