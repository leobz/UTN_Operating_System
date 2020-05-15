/*
 * diccionarios.h
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#ifndef DICCIONARIOS_H_
#define DICCIONARIOS_H_

#include <commons/collections/dictionary.h>
#include <stdlib.h>

/**
* @NAME: strings_to_dictionary
* @DESC: Dado un char**,  retorna un diccionario con clave igual
* a los char* encontrados, y con un valor igual a la cantidad de
* ocurrencias que tenga dicho char*
*
* Ejemplo: {"Pikachu","Pikachu", "Pidgey"} -> {"Pikachu":2, "Pidgey":1}
*/
t_dictionary* strings_to_dictionary(char** strings);

/**
* @NAME: dictionary_increment_value
* @DESC: Si la key ya existe, aumenta la cantidad del valor en 1.
* Si la clave no existe, la agrega y la inicializa en 1.
*
* Ejemplo:
* dictionary_increment_value("Pikachu"): {"Pikachu":2} -> {"Pikachu":3}
*/
void dictionary_increment_value(t_dictionary* dictionary, char* key);

#endif /* DICCIONARIOS_H_ */
