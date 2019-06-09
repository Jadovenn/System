/**
 * frame.c - Frame allocation and bitset arithmetic
 * System sources under license MIT
 */

#include <system.h>
#include <string.h>
#include <kernel/heap.h>
#include <kernel/paging.h>
#include <kernel/types.h>

uint32_t	*frames;
uint32_t	n_frames;

extern uint32_t	placement_address;

#define INDEX_FROM_BIT(x)	(x/(8*4))
#define OFFSET_FROM_BIT(x)	(x%(8*4))

/**
 * @details static func to set a bit in the frames bitsets
 */
static void	set_frame(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

/**
 * @details static func to clear a bit int the frames bitsets
 */
static void	clear_frame(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

/**
 * @details test if a bit is set in the frames bitsets
 */
static bool	is_set_frames(uint32_t	frame_addr) {
	uint32_t	frame = frame_addr / 0x1000;
	uint32_t	idx = INDEX_FROM_BIT(frame);
	uint32_t	off = OFFSET_FROM_BIT(frame);
	return !((frames[idx] & (0x1 << off)) == 0);
}

static uint32_t		first_available_frame() {
	for (uint32_t i = 0; i < INDEX_FROM_BIT(n_frames); i++) {
		if (frames[i] != 0xffffffff) {
			for (uint32_t j = 0; j < 32; j++) {
				uint32_t to_test = (0x1 << j);
				if (!(frames[i] & to_test)) {
					return i * 4 * 8 + j;
			}
		}
	}
}

void	alloc_frame(page_t *page, int is_kernel, int is_writeable) {
	if (page->frame != 0) { // Special case: page already have a frame
		return;
	}
	else { // Regular case: give the page a frame
		uint32_t	idx = first_available_frame();
		if (idx == (uint32_t)-1) { // Special case: no available frame
			PANIC("No available frames.");
		}
		set_frame(idx*0x1000);
		page->present = 1;
		page->rw = (is_writeable) ? 0 : 1;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void	free_frame(page_t *page) {
	uint32_t	frame;
	if (!(frame = page->frame)) {
		return;
	}
	else {
		clear_frame(frame);
		page->frame = 0x0;
	}
}

/**
 * @details initialize paging, carefull with kmalloc, block
 * could not be free, no need to check the return value it can not failed
 */
void	initialise_paging() {
	uint32_t	mem_end_page = 0x1000000; // 16Mb
	n_frames = mem_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(n_frames));
	memset(frames, 0, INDEX_FROM_BIT(n_frames));

}

