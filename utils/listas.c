#include "listas.h"

void* list_pop(t_list* lista, int index){
	void* elemento = list_get(lista, index);
	list_remove(lista, index);

	return elemento;
}

void* list_pop_first(t_list* lista){
	return list_pop(lista, 0);
}
