/**
 * test/arch/i386/physical_memory.c - Physical Memory Manager
 * System sources under license MIT
 */

#include <stdbool.h>
#include <stddef.h>
#include <arch/physical_memory.h>

static bool _check_pmm_flag(uint32_t addr, bool value) {
    pmm_region_t *region = physical_memory_map;
    while (region) {
        if (region->physical_addr <= addr &&
            region->physical_addr + (region->page_nb * 0x1000) >= addr) {
            uint32_t offset = (addr - region->physical_addr) / 0x1000 / 32;
            uint32_t bit = ((addr - region->physical_addr) / 0x1000) % 32;
            if (value) {
                if ((region->bitset[offset] & (0x80000000 >> bit)) > 0) {
                    return true;
                }
            } else {
                if ((region->bitset[offset] & (0x80000000 >> bit)) == 0) {
                    return true;
                }

            }
        }
        region = region->next;
    }
    return false;
}

int test_physical_memory_manager() {
    void *page_1 = pmm_alloc();
    void *page_2 = pmm_alloc();
    void *page_3 = pmm_alloc();
    if (!_check_pmm_flag((uint32_t) page_1, true)) {
        return 1;
    }
    if (!_check_pmm_flag((uint32_t) page_2, true)) {
        return 1;
    }
    if (!_check_pmm_flag((uint32_t) page_3, true)) {
        return 1;
    }

    pmm_free(page_1);
    if (!_check_pmm_flag((uint32_t) page_1, false)) {
        return 1;
    }
    pmm_free(page_3);
    if (!_check_pmm_flag((uint32_t) page_3, false)) {
        return 1;
    }
    pmm_free(page_2);
    if (!_check_pmm_flag((uint32_t) page_2, false)) {
        return 1;
    }
    return 0;
}
