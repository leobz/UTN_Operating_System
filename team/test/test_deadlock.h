#ifndef TEST_TEST_DEADLOCK_H_
#define TEST_TEST_DEADLOCK_H_

#include "CUnit/Basic.h"
#include "../planificacion.h"
#include "common_testing.h"
#include "test_rafagas.h"

void agregar_tests_deadlock();

void test_pokemones_necesitados();
void test_pokemones_no_necesitados();
void test_list_intersection();
void test_crear_deadlock();
void test_detectar_deadlock();
void test_despachar_deadlock_carga_la_rafaga_del_tcb_y_lo_envia_a_ready();

void test_ejecutar_intercambio();
void test_ejecutar_acciones_post_intercambio_para_tcb_a_intercambiar_sin_objetivo_cumplido();
void test_ejecutar_acciones_post_intercambio_para_tcb_con_objetivo_cumplido();

#endif /* TEST_TEST_DEADLOCK_H_ */
