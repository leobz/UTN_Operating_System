#ifndef TEST_TEST_APPEARED_H_
#define TEST_TEST_APPEARED_H_

#include "CUnit/Basic.h"
#include "../planificacion.h"
#include "common_testing.h"

t_list* temp;
t_mensaje_appeared* mensaje;

void agregar_tests_appeared();
void test_se_agrega_pokemon_a_planificados();
t_mensaje_appeared* inicializar_test_appeared();
t_mensaje_appeared* mock_appeared();
t_mensaje_appeared* mock_mensaje_appeared();
void mock_acciones_appeared(t_mensaje_appeared* mensaje);
void test_tcb_paso_a_ready();


#endif /* TEST_TEST_APPEARED_H_ */
