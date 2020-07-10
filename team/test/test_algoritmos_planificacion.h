#ifndef TEST_TEST_ALGORITMOS_PLANIFICACION_H_
#define TEST_TEST_ALGORITMOS_PLANIFICACION_H_

#include "CUnit/Basic.h"
#include "../planificacion.h"
#include "common_testing.h"

void agregar_tests_algoritmos_planificacion();

void finalizar_test();

void test_primer_tcb_posee_menor_proxima_estimacion_segundo_tcb();
void test_primer_tcb_no_posee_menor_proxima_estimacion_segundo_tcb();
void test_obtener_tcb_menor_proxima_estimacion();

#endif /* TEST_TEST_ALGORITMOS_PLANIFICACION_H_ */
