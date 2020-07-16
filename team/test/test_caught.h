#ifndef TEST_TEST_CAUGHT_H_
#define TEST_TEST_CAUGHT_H_

#include "CUnit/Basic.h"
#include "../planificacion.h"
#include "common_testing.h"
#include "test_appeared.h"

void agregar_tests_caught();

void test_se_elimino_pokemon_del_mapa();
void test_se_elimino_pokemon_a_planificados();
void test_se_pasa_tcb_a_cola_post_caught();

#endif /* TEST_TEST_CAUGHT_H_ */
