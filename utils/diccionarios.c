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


void dictionary_decrement_value(t_dictionary* dictionary, char* key) {
	dictionary_increment_value_in(dictionary, key, -1);
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

	if (dictionary_get(dictionary, key) < 0) {
		dictionary_put(dictionary, key, 0);
	}
}

int dictionaries_are_equals(t_dictionary* dic1, t_dictionary* dic2) {
	int equals = 1;
	void same_value_as_dic_2(char* key, int value){
		if (dictionary_get(dic2, key) != value){
			equals = 0;
		}
	}
	dictionary_iterator(dic1, same_value_as_dic_2);

	return equals;
}

void meter_en_diccionario(t_dictionary* dictionary,int key,void*value){
	char*char_key=pasar_a_char(key);
	dictionary_put(dictionary,char_key,value);
	free(char_key);
}

void* sacar_de_diccionario(t_dictionary* dictionary,int key){
	char*char_key=pasar_a_char(key);
	void*eliminar=dictionary_remove(dictionary,char_key);
	free(char_key);
	return eliminar;
}

void*obtener_de_diccionario(t_dictionary*dictionary, int key){
	char*char_key=pasar_a_char(key);
	void*value=dictionary_get(dictionary,char_key);
	return value;
}

bool esta_en_diccionario(t_dictionary* dictionary,int key){
	char*char_key=pasar_a_char(key);
	bool verificacion=dictionary_has_key(dictionary,char_key);
	free(char_key);
	return verificacion;
}

t_dictionary* dictionary_substract(t_dictionary* dic1, t_dictionary* dic2){
	t_dictionary* dictionary_substract = dictionary_create();

	void substract_key_and_values(char* key, void* value){
		int subtracted_value = value;

		if (dictionary_has_key(dic2, key)) {
			subtracted_value -= (int) dictionary_get(dic2, key);
		}

		if(subtracted_value > 0) {
			dictionary_put(dictionary_substract, key, subtracted_value);
		}
	}

	dictionary_iterator(dic1, substract_key_and_values);

	return dictionary_substract;
}

t_list* dictionary_keys_to_list(t_dictionary* dictionary){
	t_list* list = list_create();

	void add_key_to_list(char* key, void* value){
		list_add(list, key);
	}

	dictionary_iterator(dictionary, (void*) add_key_to_list);

	return list;
}

void remover_decrementar_value_en_diccionario(t_dictionary* dic, char* key) {
	if (dictionary_has_key(dic, key)) {
		if (dictionary_get(dic, key) == 1)
			dictionary_remove(dic, key);
		else
			dictionary_decrement_value(dic, key);
	}
}

static void internal_dictionary_clean(t_dictionary *self) {
	int table_index;

	for (table_index = 0; table_index < self->table_max_size; table_index++) {
		t_hash_element *element = self->elements[table_index];
		t_hash_element *next_element = NULL;

		while (element != NULL) {

			next_element = element->next;

			free(element->key);;

			element = next_element;
		}

		self->elements[table_index] = NULL;
	}

	self->table_current_size = 0;
	self->elements_amount = 0;
}

void dictionary_destroy_keys(t_dictionary* self){
	dictionary_clean(self);
	free(self->elements);
	free(self);

}
