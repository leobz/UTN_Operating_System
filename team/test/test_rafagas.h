#ifndef TEST_TEST_RAFAGAS_H_
#define TEST_TEST_RAFAGAS_H_

#include "CUnit/Basic.h"

#include "../planificacion.h"
#include "common_testing.h"

void agregar_tests_rafagas();

void test_cargar_rafaga_movimiento_cuadrante_1();
void test_cargar_rafaga_movimiento_cuadrante_3_1();
void test_cargar_rafaga_movimiento_cuadrante_1_3();

void test_cargar_rafaga_captura();

void test_leer_instrucciones(tcb_entrenador* tcb, int cantidad, int instruccion);

#endif /* TEST_TEST_RAFAGAS_H_ */
