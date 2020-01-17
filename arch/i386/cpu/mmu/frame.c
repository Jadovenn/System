/**
 * frame.c - Physical Frames Map
 * System under license MIT
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <bitset.h>
#include <multiboot.h>
#include <kernel/heap.h>

#include "cpu/mmu.h"

int	map_frame_region(uint32_t physical_addr, size_t length, uint32_t memory_type) {
	size_t	size = length / 0x1000 / 32; 
	if (size == 0) { // Special Case: Frame to small do not map it
		return EXIT_FAILURE;
	}
	frame	*new_frame = kmalloc(sizeof(frame));
	if (!new_frame) {
		return EXIT_FAILURE;
	}
	new_frame->next = NULL;
	new_frame->physical_addr = physical_addr;
	new_frame->size = size;
	new_frame->bitset = kmalloc(new_frame->size * sizeof(uint32_t));
	if (!new_frame->bitset) {
		kfree(new_frame);
		return EXIT_FAILURE;
	}
	memset(new_frame->bitset, 0, new_frame->size);
	new_frame->type = memory_type;
	if (!mmu.frames) {
		mmu.frames = new_frame;
	} else {
		frame *iterator = mmu.frames;
		for (;iterator->next; iterator = iterator->next);
		iterator->next = new_frame;
	}
	return EXIT_SUCCESS;
}

uint32_t	first_available_page() {
	frame	*iterator = mmu.frames;
	uint32_t physical_addr = 0;
	for (;;) {
		if (!iterator) { // Special Case: No available frames
			break;
		}
		else if (iterator->type == PHYS_MEMORY_AVAILABLE) { // Regular Case: frames available
			unsigned i, j;
			for (i = 0; i < iterator->size; i++) {
				if (_has_zero(iterator->bitset[i])) { // Regular Case: a frame is available
					j = 0;
					uint32_t bit_mask = 0x00000001;
					while (j < 32) {
						if (!(iterator->bitset[i] & (bit_mask << j))) { // Regular Case: frame found
							physical_addr = iterator->physical_addr + (i * 0x1000 * 32) + (j * 0x1000);
							iterator->bitset[i] |= bit_mask;
							return physical_addr;
						}
						j += 1;
					}
				}
			}
		}
		iterator = iterator->next;
	}
	return physical_addr;
}

void		release_frame(uint32_t physical_addr) {
	frame	*iterator = mmu.frames;
	for (;;) {
		if (!iterator) {
			break;
		}
		else if (physical_addr >= iterator->physical_addr
				&& physical_addr <= (iterator->physical_addr + iterator->size * 0x1000 * 32)) {
			unsigned i = physical_addr / (32 * 0x1000);
			unsigned j = (i - (0x1000 * 32)) / 0x1000;
			uint32_t bitset = 0x00000001 << j;
			iterator->bitset[i] &= ~bitset;
		}
		iterator = iterator->next;
	}
}

