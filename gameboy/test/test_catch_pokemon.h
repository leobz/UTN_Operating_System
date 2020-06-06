/*
 * test_catch_pokemon.h
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#ifndef TEST_TEST_CATCH_POKEMON_H_
#define TEST_TEST_CATCH_POKEMON_H_

#include "test_utils.h"

void empaquetar_y_desempaquetar_catch_pokemon();
void serializar_y_deserializar_catch_pokemon();
void agregar_tests_catch_pokemon();

void test_deserializar_buffer_catch_pokemon(void* mensaje_serializado,
		char* pokemon, int pos_x, int pos_y);

#endif /* TEST_TEST_CATCH_POKEMON_H_ */
