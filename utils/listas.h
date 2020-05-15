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

#endif /* LISTAS_H_ */
