/**
 * frame.c - Physical Frames Map
 * System under license MIT
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <kernel/multiboot.h>
#include <kernel/heap.h>
#include <cpu/mmu.h>

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

