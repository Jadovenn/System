/**
 * list.c - linked list
 * System sources under license MIT
 */

#include <stddef.h>

#include <algo/list.h>

void kalgotils_list_push_front(List_t* list, List_head_t* block) {
	if (!list) {
		return;
	}
	if (list->size == 0) {
		list->head  = block;
		list->tail  = block;
		block->next = NULL;
		block->prev = NULL;
		list->size  = 1;
		return;
	}
	list->head->prev = block;
	block->next      = list->head;
	block->prev      = NULL;
	list->head       = block;
	list->size += 1;
}

void* kalgotils_list_pop_front(List_t* list) {
	if (!list || list->size == 0) {
		return NULL;
	}
	List_head_t* head = list->head;
	list->head        = head->next;
	if (list->head) {
		list->head->prev = NULL;
	} else {
		list->tail = NULL;
	}
	head->next = NULL;
	list->size -= 1;
	return head->container;
}

void kalgotils_list_push_back(List_t* list, List_head_t* block) {
	if (!list) {
		return;
	}
	if (list->size == 0) {
		kalgotils_list_push_front(list, block);
	} else {
		list->tail->next = block;
		block->prev      = list->tail;
		block->next      = NULL;
		list->tail       = block;
		list->size += 1;
	}
}

void* kalgotils_list_pop_back(List_t* list) {
	if (!list || list->size == 0) {
		return NULL;
	}
	List_head_t* tail = list->tail;
	list->tail        = tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	} else {
		list->head = NULL;
	}
	tail->prev = NULL;
	list->size -= 1;
	return tail->container;
}

void* kalgotils_list_at(List_t* list, size_t index) {
	if (!list) {
		return NULL;
	}
	size_t counter = 0;
	for (List_head_t* idx = list->head; idx; idx = idx->next, counter++) {
		if (counter == index) {
			return idx->container;
		}
	}
	return NULL;
}

void* kalgotils_insert(List_t* list, List_head_t* block, size_t index) {
	if (!list) {
		return NULL;
	}
	if (index > list->size) {
		return NULL;
	}
	if (index == 0) {
		kalgotils_list_push_front(list, block);
		return list->head->container;
	}
	if (index == list->size) {
		kalgotils_list_push_back(list, block);
		return list->tail->container;
	}
	List_head_t* idx = list->head;
	for (size_t counter = 0; idx; idx = idx->next, counter++) {
		if (counter == index) {
			break;
		}
	}
	if (!idx) {
		return NULL;
	}
	block->prev = idx->prev;
	if (idx->prev) {
		idx->prev->next = block;
	}
	idx->prev   = block;
	block->next = idx;
	list->size += 1;
	return block->container;
}

void* kalgotils_remove_at(List_t* list, size_t index) {
	if (!list) {
		return NULL;
	}
	if (index >= list->size) {
		return NULL;
	}
	if (index == 0) {
		List_head_t* block = list->head;
		kalgotils_list_pop_front(list);
		return block->container;
	}
	if (index == list->size - 1) {
		List_head_t* block = list->tail;
		kalgotils_list_pop_back(list);
		return block->container;
	}
	List_head_t* idx = list->head;
	for (size_t counter = 0; idx; idx = idx->next, counter++) {
		if (counter == index) {
			break;
		}
	}
	if (!idx) {
		return NULL;
	}
	if (idx->next) {
		idx->next->prev = idx->prev;
	}
	if (idx->prev) {
		idx->prev->next = idx->next;
	}
	idx->next = NULL;
	idx->prev = NULL;
	list->size -= 1;
	return idx->container;
}

void* kalgotils_find_if(List_t* list, void* data, List_eq_operator_t eq_operator) {
	if (!list || !data) {
		return NULL;
	}
	if (eq_operator) {
    List_for_each((*list)) {
			if (eq_operator(list_it->container, data)) {
				return list_it->container;
			}
		}
	}
	else {
    List_for_each((*list)) {
			if (list_it->container == data) {
				return list_it->container;
			}
		}
	}
	return NULL;
}
