#include "../test/test_configuracion.h"
#include "../test/test_appeared_pokemon.h"

void correrTests() {
	CU_initialize_registry();

	agregar_tests_configuracion();
	agregar_tests_appeared_pokemon();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

}

