/*
 * diccionarios.h
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#ifndef DICCIONARIOS_H_
#define DICCIONARIOS_H_

#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include "../utils/serializacion.h"

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


/**
* @NAME: dictionary_increment_value_in
* @DESC: Si la key ya existe, incrementa su valor en una cantidad X.
* Si la clave no existe, la agrega y la inicializa en X.
*
* Ejemplo:
* dictionary_increment_value_in("Pikachu", 10): {"Pikachu":2} -> {"Pikachu":12}
*/
void dictionary_increment_value_in(t_dictionary* dictionary, char* key, int amount);


/**
* @NAME: sum_dictionaries_values
* @DESC: Dada una lista de diccionarios, retorna la suma de todos los valores.
*/
t_dictionary* sum_dictionaries_values(t_list* dictionaries_list);

/**
* @NAME: sum_dictionary_values
* @DESC: Dado un diccionario con valores numericos, retorna la suma de todos
* los valores
*
* Ejemplo:
* {"A":2, "B":3} -> 5
*/
int sum_dictionary_values(t_dictionary* );

/**
* @NAME: dictionaries_are_equals
* @DESC: Compara 2 diccionarios, si son iguales retorna true
*
* Ejemplo:
* {"A":2} {"A":2} -> 1
* {"A":2} {"A":0} -> 0
*/

int dictionaries_are_equals(t_dictionary*, t_dictionary*);
void meter_en_diccionario(t_dictionary* dictionary,int key,void*value);
void*obtener_de_diccionario(t_dictionary*dictionary, int key);
bool esta_en_diccionario(t_dictionary* dictionary,int key);

#endif /* DICCIONARIOS_H_ */
