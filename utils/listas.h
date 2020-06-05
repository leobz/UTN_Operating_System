/*
 * listas.h
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#ifndef LISTAS_H_
#define LISTAS_H_

#include <commons/collections/list.h>
#include <stdlib.h>

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


/**
* @NAME: list_map_free
* @DESC: Retorna una nueva lista con los
* con los elementos transformados y libera
* la lista de entrada
*/
t_list* list_map_free(t_list* self, void*(*transformer)(void*));

/**
* @NAME: strings_to_list
* @DESC: Dada un char** retorna una lista de char*
*
* Ejemplo: {"1","2"} -> ["1", "2"]
*/
t_list* strings_to_list(char** strings);

/**
* @NAME: list_remove_element
* @DESC: Elimina al primer elemento que coincida con el puntero
* del elemento pasado
*/
void list_remove_element(t_list* lista, void* element);

#endif /* LISTAS_H_ */
