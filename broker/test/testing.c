/*
 * testing.c
 *
 *  Created on: May 10, 2020
 *      Author: diego
 */
#include "testing.h"

void correrTests() {
	CU_initialize_registry();

	//agregar_tests_configuracion();
	//agregar_tests_new_pokemon();
	agregar_tests_buddy_system();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

}

