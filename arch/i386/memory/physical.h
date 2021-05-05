/**
 * memory/physical.h - i386 Physical Memory Management API
 * System sources under license MIT
 */

#ifndef I386_PHYSICAL_H_
#define I386_PHYSICAL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/********************************
 **    Structures              **
 ********************************/

typedef enum Memory_type {
	mt_AVAILABLE        = 1,
	mt_RESERVED         = 2,
	mt_ACPI_RECLAIMABLE = 3,
	mt_NVS              = 4,
	mt_BAD_RAM          = 5,
	mt_BIOS             = 6,
	mt_LOWER_REGION     = 7,
} Memory_type_e;

typedef struct Physical_memory_region {
	struct Physical_memory_region* next;
	uintptr_t                      startAddr;
	uintptr_t                      endAddr;
	uint32_t                       bytes;
	size_t                         page_nb;
	uint32_t                       type;
	uint32_t*                      bitset;
} Physical_memory_region_t;

typedef enum Physical_memory_state_flags {
	pms_PRESENT     = 1,
	pms_NOT_PRESENT = 2,
} Physical_memory_state_e;

/********************************
 **    Globals declaration     **
 ********************************/

extern uint32_t                  G_Physical_mmap_start;
extern uint32_t                  G_Physical_mmap_end;
extern Physical_memory_region_t* G_Physical_memory_map;

/********************************
 **     Public functions       **
 ********************************/

/**
 * @brief Init physical memory
 * @param aMultibootMmapAddr - Multiboot memory map addr
 * @param aMultibootMmapLength - Multiboot memory map length
 */
void Init_physical_memory(uintptr_t aMultibootMmapAddr,
                          uint32_t  aMultibootMmapLength);

/**
 * @brief Free a physical page
 * @param addr - address to the page to free
 * @details the physical page should not be used anymore
 * the free is performed via an offset computation so it is
 * pretty fast, O(1)
 */
void Physical_memory_release_page(uintptr_t aPhysicalAddress);

/**
 * @brief Allocate one physical page
 * @param type - memory flags from api/mm.h
 * @return physical address to the allocated physical page
 * @details the allocator is implemented as a sequential search O(N)
 */
uintptr_t Physical_memory_get_page(Memory_type_e type);

/**
 * @brief Set value to physical page
 * @param p_addr - aligned physical page's addr
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t Physical_memory_set_page(uintptr_t               aPhysicalAddr,
                                  Physical_memory_state_e aState);

/**
 * @brief Set value to physical region including p_end_addr
 * @param p_start_addr - first page addr to map aligned
 * @param p_end_addr - last including page to map aligned
 * @param value - value to set true/false
 * @return EXIT_SUCCESS/FAILURE
 */
uint32_t Physical_memory_set_region(uintptr_t               aPhysicalStartAddr,
                                    uintptr_t               aPhysicalEndAddr,
                                    Physical_memory_state_e aState);
#endif // I386_PHYSICAL_H_
