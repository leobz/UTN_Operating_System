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


t_dictionary* sum_dictionaries_values(t_list* dictionaries_list){
	t_dictionary* sum_of_dictionaries = dictionary_create();

	void increment_sum_of_dictionaries(char* key, void* value){
		dictionary_increment_value_in(sum_of_dictionaries, key, value);
	}

	void merge_dictionary(t_dictionary* dic_entrada){
		dictionary_iterator(dic_entrada, increment_sum_of_dictionaries);
	}


	list_iterate(dictionaries_list, merge_dictionary);

	return sum_of_dictionaries;
}

int sum_dictionary_values(t_dictionary* dictionary){
	int sum = 0;
	void sum_values(char* key, int value){
		sum += value;
	}

	dictionary_iterator(dictionary, sum_values);

	return sum;
}

void dictionary_increment_value(t_dictionary* dictionary, char* key) {
	dictionary_increment_value_in(dictionary, key, 1);
}

void dictionary_increment_value_in(t_dictionary* dictionary, char* key, int amount) {
	if (key != NULL) {
		if (dictionary_has_key(dictionary,key)){
			int cantidad = dictionary_get(dictionary, key);
			cantidad+= amount;
			dictionary_put(dictionary, key, cantidad);
		}
		else
			dictionary_put(dictionary, key, amount);
	}
}
