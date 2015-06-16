#include "../collections/linked_list.h"
#include "../memory/memory.h"

list_s* list_create(size_t element_size){
	list_s* list = (list_s*) malloc(sizeof(list_s));
	list->head = NULL;
	list->tail = NULL;
	list->elem_size = element_size;

	return list;
}

void* list_get(list_s* list, uint_t index){
	list_node_s* current = list->head;

	for(int i = 0; i < index; i++){
		if(current == NULL){
			break;
		} else{
			current = current->next;
		}
	}

	if(current == NULL){
		return NULL;
	} else{
		return current->data;
	}
}

void* list_first(list_s* list){
	if(list->head == NULL){
		return NULL;
	} else{
		return list->head->data;
	}
}

void* list_last(list_s* list){
	if(list->tail == NULL){
		return NULL;
	} else{
		return list->tail->data;
	}
}

void list_remove(list_s* list, uint_t index){
	list_node_s* current = list->head;

	for(int i = 0; i < index; i++){
		if(current == NULL){
			return;
		} else{
			current = current->next;
		}
	}

	if(current != NULL){
		current->prev->next = current->next;
		current->next->prev = current->prev;
		if(list->head == current){
			list->head = current->next;
		}
		if(list->tail == current){
			list->tail = current->prev;
		}
		free(current);
	}
}

void list_append(list_s* list, void* data){
	list_node_s* node = malloc(sizeof(list_node_s));
	node->prev = list->tail;
	node->next = NULL;
	node->data = data;

	if(list->head == NULL){
		list->head = node;
	}
	if(list->tail != NULL){
		list->tail->next = node;
	}

	list->tail = node;
}

list_node_s* list_get_next(list_s* list, list_node_s* current){
	if(list->tail == current){
		return list->head;
	} else{
		return current->next;
	}
}

list_node_s* list_get_prev(list_s* list, list_node_s* current){
	if(list->head == current){
		return list->tail;
	} else {
		return current->prev;
	}
}
