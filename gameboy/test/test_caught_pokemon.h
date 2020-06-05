/*
 * test_caught_pokemon.h
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#ifndef TEST_TEST_CAUGHT_POKEMON_H_
#define TEST_TEST_CAUGHT_POKEMON_H_

#include "test_utils.h"

void empaquetar_y_desempaquetar_caught_pokemon();
void serializar_y_deserializar_caught_pokemon();
void agregar_tests_caught_pokemon();

void test_deserializar_buffer_caught_pokemon(void* mensaje_serializado,
		int estado_actual, int id_correlativo);

#endif /* TEST_TEST_CAUGHT_POKEMON_H_ */
