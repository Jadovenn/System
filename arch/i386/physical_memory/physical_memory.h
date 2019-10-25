/**
 * physical_memory.h - Physical Memory Layout Management
 * System sources under license MIT
 */

#ifndef PHYSICAL_MEMORY_H_
# define PHYSICAL_MEMORY_H_

extern struct physical_memory_group *groups;

struct physical_memory_group {
	int	*bitset;
	struct physical_memory_group *next;
};

#endif // PHYSICAL_MEMORY_H_

