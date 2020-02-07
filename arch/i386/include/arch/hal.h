/**
 * hal.h Hardware Abstract Layer
 * System sources under license MIT
 */

#ifndef HAL_H_
# define HAL_H_

#include <stdint.h>

typedef struct	mmu {
	uintptr_t	page_directory;
}		mmu_t;


typedef struct	hal {
	mmu_t	mmu;
}		hal_t;

extern hal_t	hal;

#endif // HAL_H_

