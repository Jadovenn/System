/**
 *	Kernel entry point
 */

// private header
#include "init/init.h"
#include <kernel/timer.h>

void	kmain() {
	// after early stage all kernel
	// basic functionality and hardware are enable and ready to be used
	// see init section to go threw the initial kernel code
	early_stage();
	init_timer(50);
}

