/**
 * Init - private headers
 */

// invok by kmain
void	k_init();

/**
 * Note
 * sub fonction to init intern stuff
 * put here extra func if necessary
 * only call by k_init, they must be called in their declaration order
 */

//	init monitor
int	init_monitor();

//	init descriptor tables
int	init_descriptor_tables();

