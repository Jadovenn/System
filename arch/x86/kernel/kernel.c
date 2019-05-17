/**
 *	Kernel entry point
 */

// private header
#include "init/init.h"
#include <kernel/timer.h>

void	kmain() {
	early_stage();
	init_timer(50);
}

