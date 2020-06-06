/*
 * test_get_pokemon.h
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#ifndef TEST_TEST_GET_POKEMON_H_
#define TEST_TEST_GET_POKEMON_H_

#include "test_utils.h"

void empaquetar_y_desempaquetar_get_pokemon();
void serializar_y_deserializar_get_pokemon();
void agregar_tests_get_pokemon();

void test_deserializar_buffer_get_pokemon(void* mensaje_serializado,
		char* pokemon);

#endif /* TEST_TEST_GET_POKEMON_H_ */
