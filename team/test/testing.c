#include "CUnit/Basic.h"

void correrTests() {
	CU_initialize_registry();
	agregar_tests_lista_ready();
	agregar_tests_rafagas();
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

}

