/**
 * init.h - init early stage hooks
 * System sources under license MIT
 */

/**
 * Note
 * sub fonction to init intern stuff
 * put here extra func if necessary
 * only call by k_init, they must be called in their declaration order
 */

#ifndef INIT_H_
# define INIT_H_

int	monitor_init();

void	install_gdt();
void	install_idt();

#endif // INIT_H_

