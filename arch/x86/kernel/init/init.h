/**
 * init.h - init early stage hooks
 * System sources under license MIT
 */

// invok by kmain
void	early_stage();

/**
 * Note
 * sub fonction to init intern stuff
 * put here extra func if necessary
 * only call by k_init, they must be called in their declaration order
 */

//	init monitor
int	init_monitor();

//	init descriptor tables
int	install_descriptor_tables();

