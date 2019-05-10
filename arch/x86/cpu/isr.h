/**
 * Interfaces and structures for high level interrupt service routines
 */

#ifndef ISR_H_
# define ISR_H_

#include <kernel/types.h>

typedef struct registers {
	uint32_t	ds; // datasegment selector
	uint32_t	edi, esi, ebp, esp; // pusha
	uint32_t	ebx, edx, ecx, eax; // pusha
	uint32_t	interr_nb, err_code; // interrupt nb and error
	uint32_t	eip, cs, eflags, useresp, ss; // pushed by the processor
} registers_t;

#endif // ISR_H_

