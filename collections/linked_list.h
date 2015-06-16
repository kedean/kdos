#pragma once

#include "../kernel/types.h"

typedef struct list_node list_node_s;
typedef struct list list_s;

struct list_node {
	struct list_node* next;
	struct list_node* prev;
	void* data;
};

struct list {
	struct list_node* head;
	struct list_node* tail;
	size_t elem_size;
};

list_s* list_create(size_t element_size);

void* list_get(list_s* list, uint_t index);

void* list_first(list_s* list);

void* list_last(list_s* list);

void list_remove(list_s* list, uint_t index);

void list_append(list_s* list, void* data);
