#ifndef TEST_TEST_MOVIMIENTO_ENTRENADOR_H_
#define TEST_TEST_MOVIMIENTO_ENTRENADOR_H_

#include "CUnit/Basic.h"

#include "../planificacion.h"
#include "common_testing.h"

void agregar_tests_movimiento_entrenador();
void entrenador_y_pokemon_a_capturar_misma_posicion();
void entrenador_se_mueve_a_derecha_hacia_pokemon_a_capturar_en_x();
void entrenador_se_mueve_a_izquierda_hacia_pokemon_a_capturar_en_x();
void entrenador_se_mueve_arriba_hacia_pokemon_a_capturar_en_y();
void entrenador_se_mueve_abajo_hacia_pokemon_a_capturar_en_y();

#endif /* TEST_TEST_MOVIMIENTO_ENTRENADOR_H_ */
