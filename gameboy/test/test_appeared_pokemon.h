#ifndef TEST_TEST_APPEARED_POKEMON_H_
#define TEST_TEST_APPEARED_POKEMON_H_

#include "CUnit/Basic.h"
#include "../gameboy.h"

void empaquetar_y_desempaquetar_appeared_pokemon();
void serializar_y_deserializar_appeared_pokemon();
void agregar_tests_appeared_pokemon();

void test_deserializar_buffer_appeared_pokemon(void* mensaje_serializado, char* pokemon, int pos_x, int pos_y);

#endif /* TEST_TEST_APPEARED_POKEMON_H_ */
