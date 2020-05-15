/*
 * diccionarios.c
 *
 *  Created on: 15 may. 2020
 *      Author: leo bazan
 */

#include "diccionarios.h"

t_dictionary* strings_to_dictionary(char** strings) {
	t_dictionary* dictionary = dictionary_create();
	while (*strings != NULL) {
		dictionary_increment_value(dictionary, *strings);
		strings++;
	}
	return dictionary;
}

void dictionary_increment_value(t_dictionary* dictionary, char* key) {
	if (key != NULL) {
		if (dictionary_has_key(dictionary,key)){
			int cantidad = dictionary_get(dictionary, key);
			cantidad++;
			dictionary_put(dictionary, key, cantidad);
		}
		else
			dictionary_put(dictionary, key, 1);
	}
}
