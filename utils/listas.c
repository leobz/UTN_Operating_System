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
	list_destroy_and_destroy_elements(self, (void*)free);
	return mapped;
}

t_list* strings_to_list(char** strings) {
	t_list* list = list_create();
	while (*strings != NULL) {
		list_add(list, strdup(*strings));
		strings++;
	}
	return list;
}

char* list_to_char_array(t_list*lista){

	int i=0;
	char* vector_de_bloques = string_new();

	string_append(&vector_de_bloques, "[");


	void meter_en_vector(char* elemento_en_lista){

		if(i!=0)
			string_append(&vector_de_bloques, ",");

		string_append_with_format(&vector_de_bloques, "%s",elemento_en_lista);
		i++;
	}
	list_iterate(lista,meter_en_vector);

	string_append(&vector_de_bloques, "]");

	return vector_de_bloques;
}

void* list_first(t_list* lista){
	return list_get(lista, 0);
}

void* list_last(t_list* lista){
	return list_get(lista, list_size(lista)-1);
}

void* list_pop(t_list* lista, int index){
	void* elemento = list_get(lista, index);
	list_remove(lista, index);

	return elemento;
}

void* list_pop_first(t_list* lista){
	return list_pop(lista, 0);
}

void list_remove_element(t_list* lista, void* element){
	for(int i = 0;  i < list_size(lista); i++){
		if  (list_get(lista, i) == element)
			list_remove(lista, i);
	}
}

bool list_include(t_list* lista, void* element) {
	bool include = false;
	for(int i = 0;  i < list_size(lista); i++){
		if  (list_get(lista, i) == element)
			include = true;
	}

	return include;
}

bool list_include_string(t_list* lista, char* element) {
	bool include = false;
	for(int i = 0;  i < list_size(lista); i++){
		if  (strcmp(list_get(lista, i),element) == 0)
			include = true;
	}

	return include;
}

t_list* list_filter_strings(t_list* self, bool(*condition)(void*)){
	t_list* filtered = list_create();

	void _add_if_apply(void* element) {
		if (condition(element)) {
			list_add(filtered, strdup(element));
		}
	}

	list_iterate(self, _add_if_apply);
	return filtered;
}

t_list* list_intersection_strings(t_list* lista_a, t_list* lista_b) {
	bool list_b_include_element_a(char* elemento_de_a) {
		return list_include_string(lista_b, elemento_de_a);
	}

	return list_filter_strings(lista_a, (void*) list_b_include_element_a);
}
