#ifndef LISTAS_H_
#define LISTAS_H_

#include <commons/collections/list.h>

/**
 * @NAME: list_pop_first
 * @DESC: retorna el primer elemento y lo saca de la lista
 */
void* list_pop_first(t_list* lista);

/**
 * @NAME: list_pop
 * @DESC: retorna un elemento en cierta posicion y lo saca de la lista
 */
void* list_pop(t_list* lista, int index);

#endif /* LISTAS_H_ */
