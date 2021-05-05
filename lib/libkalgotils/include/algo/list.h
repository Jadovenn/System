/**
 * algo/list.h - linked list
 * System sources under license MIT
 */

#ifndef KALGOTILS_LIST_H
#define KALGOTILS_LIST_H

#include <stdbool.h>

#include <utils/container_of.h>

typedef struct List_head_s {
	struct List_head_s* next;
	struct List_head_s* prev;
	void*               container;
} List_head_t;

typedef struct List_s {
	List_head_t* head;
	List_head_t* tail;
	size_t       size;
} List_t;

#define LIST_BLOCK_HEAD List_head_t kalgotils_list_head

#define List_block_init(block)                                                 \
	do {                                                                         \
		block.kalgotils_list_head.next      = NULL;                                \
		block.kalgotils_list_head.prev      = NULL;                                \
		block.kalgotils_list_head.container = &block;                              \
	} while (0)

#define List_init(list)                                                        \
	do {                                                                         \
		list.head = NULL;                                                          \
		list.tail = NULL;                                                          \
		list.size = 0;                                                             \
	} while (0)

#define List_get_it(type) type* lblock = list_it->container

#define List_get_size(list) list.size

#define List_for_each(list)                                                    \
	for (List_head_t* list_it = list.head; list_it; list_it = list_it->next)

void kalgotils_list_push_front(List_t* list, List_head_t* block);
#define List_push_front(list, block)                                           \
	kalgotils_list_push_front(&list, &(block.kalgotils_list_head))

void* kalgotils_list_pop_front(List_t* list);
#define List_pop_front(list) kalgotils_list_pop_front(&list)

void kalgotils_list_push_back(List_t* list, List_head_t* block);
#define List_push_back(list, block)                                            \
	kalgotils_list_push_back(&list, &(block.kalgotils_list_head))

void* kalgotils_list_pop_back(List_t* list);
#define List_pop_back(list) kalgotils_list_pop_back(&list)

void* kalgotils_list_at(List_t* list, size_t index);
#define List_at(list, index) kalgotils_list_at(&list, index)

void* kalgotils_insert(List_t* list, List_head_t* block, size_t index);
#define List_insert(list, block, index)                                        \
	kalgotils_insert(&list, &(block.kalgotils_list_head), index)

void* kalgotils_remove_at(List_t* list, size_t index);
#define List_remove_at(list, index) kalgotils_remove_at(&list, index)

typedef bool(List_eq_operator_t)(void* rhs, void* lhs);

void* kalgotils_find_if(List_t* list, void* data, List_eq_operator_t op);
#define List_find_if(list, data, eq_operator)                                  \
	kalgotils_find_if(&list, data, eq_operator)

#endif // KALGOTILS_LIST_H
