#ifndef TEST_TEST_DEADLOCK_H_
#define TEST_TEST_DEADLOCK_H_

#include "CUnit/Basic.h"
#include "../planificacion.h"
#include "common_testing.h"

void agregar_tests_deadlock();

void test_pokemones_necesitados();
void test_pokemones_no_necesitados();
void test_list_intersection();
void test_crear_deadlock();
void test_detectar_deadlock();

#endif /* TEST_TEST_DEADLOCK_H_ */
