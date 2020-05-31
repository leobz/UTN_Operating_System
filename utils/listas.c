/*
 * listas.c
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#include "listas.h"

t_list* list_map_free(t_list* self, void*(*transformer)(void*)){
	t_list* mapped = list_create();

	void _add_after_transform(void* element) {
		void* new_element = transformer(element);
		list_add(mapped, new_element);
	}

	list_iterate(self, _add_after_transform);
	free(self);
	return mapped;
}

t_list* strings_to_list(char** strings) {
	t_list* list = list_create();
	while (*strings != NULL) {
		list_add(list, *strings);
		strings++;
	}
	return list;
}

void* list_pop(t_list* lista, int index){
	void* elemento = list_get(lista, index);
	list_remove(lista, index);

	return elemento;
}

void* list_pop_first(t_list* lista){
	return list_pop(lista, 0);
}
