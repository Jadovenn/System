/**
 * container_of.h - retrieve struct ptr from a member
 * System sources under license MIT
 */

#ifndef KALGOTILS_CONTAINER_OF_H
#define KALGOTILS_CONTAINER_OF_H

#define container_of(ptr, type, member)                                        \
	({                                                                           \
		const typeof(((type*)0)->member)* __mptr = (ptr);                          \
		(type*)((char*)__mptr - offsetof(type, member));                           \
	})

#endif // KALGOTILS_CONTAINER_OF_H
