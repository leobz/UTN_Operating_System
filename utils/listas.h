/*
 * listas.h
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#ifndef LISTAS_H_
#define LISTAS_H_

#include <commons/collections/list.h>
#include "commons/string.h"
#include <stdlib.h>

/**
 * @NAME: list_first
 * @DESC: retorna el primer elemento de la lista
 */
void* list_first(t_list* lista);

/**
 * @NAME: list_last
 * @DESC: retorna el ultimo elemento de la lista
 */
void* list_last(t_list* lista);

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
* @NAME: list_to_char_array
* @DESC: Dada una lista de strings retorna una array en forma de char*
*
* Ejemplo: {"1","2"} -> "[1, 2]"
*/
char* list_to_char_array(t_list*lista);

/**
* @NAME: list_remove_element
* @DESC: Elimina al primer elemento que coincida con el puntero
* del elemento pasado
*/
void list_remove_element(t_list* lista, void* element);

/**
* @NAME: list_include
* @DESC: Si la lista contiene el elemento retorna true, sino, retorna false
*/
bool list_include(t_list* lista, void* element);

/**
* @NAME: list_include_string
* @DESC: Si la lista contiene el string retorna true, sino, retorna false
*/
bool list_include_string(t_list* lista, char* element);

/**
* @NAME: list_intersection
* @DESC: Retorna la interseccion entre dos listas
*/
t_list* list_intersection(t_list* lista_a, t_list* lista_b);

#endif /* LISTAS_H_ */
