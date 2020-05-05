#ifndef LISTAS_H_
#define LISTAS_H_

#include <commons/collections/list.h>

/**
	* @NAME: pop
	* @DESC: retorna el primer elemento y lo saca de la lista
	*/
void* list_pop(t_list* lista, int index);
void* list_pop_first(t_list* lista);

#endif /* LISTAS_H_ */
