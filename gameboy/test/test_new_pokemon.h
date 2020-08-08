/*
 * test_new_pokemon.h
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#ifndef TEST_TEST_NEW_POKEMON_H_
#define TEST_TEST_NEW_POKEMON_H_

#include "test_utils.h"

void empaquetar_y_desempaquetar_new_pokemon();
void serializar_y_deserializar_new_pokemon();
void agregar_tests_new_pokemon();

void test_deserializar_buffer_new_pokemon(void* mensaje_serializado,
		char* pokemon, int pos_x, int pos_y, int cantidad, int id_correlativo);

#endif /* TEST_TEST_NEW_POKEMON_H_ */
