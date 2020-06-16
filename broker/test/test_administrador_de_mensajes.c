#include "test_administrador_de_mensajes.h"

void agregar_tests_administrador_de_mensaje() {
	CU_pSuite suite_configuracion = CU_add_suite("Administrador de mensaje", NULL, NULL);

	CU_add_test(suite_configuracion,
			"Adminstrador guarda mensaje en cache",
			test_guardar_mensaje_en_cache);

}

void test_guardar_mensaje_en_cache() {

}
