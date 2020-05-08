#include "test_configuracion.h"

void cargar_configuracion_ip() {
	t_gameboy_config *gameboy_config = cargar_gameboy_config(
			"gameboy.config.sample");

	CU_ASSERT_STRING_EQUAL(gameboy_config->ip_broker, "127.0.0.1");
	CU_ASSERT_STRING_EQUAL(gameboy_config->ip_team, "127.0.0.2");
	CU_ASSERT_STRING_EQUAL(gameboy_config->ip_gamecard, "127.0.0.3");

	destruir_gameboy_config(gameboy_config);
}

void cargar_configuracion_puertos() {
	t_gameboy_config *gameboy_config = cargar_gameboy_config(
			"gameboy.config.sample");

	CU_ASSERT_STRING_EQUAL(gameboy_config->puerto_broker, "5003");
	CU_ASSERT_STRING_EQUAL(gameboy_config->puerto_team, "5002");
	CU_ASSERT_STRING_EQUAL(gameboy_config->puerto_gamecard, "5001");

	destruir_gameboy_config(gameboy_config);
}

void agregar_tests_configuracion(){
	CU_pSuite suite_configuracion = CU_add_suite("Cargar configuracion del Gameboy", NULL, NULL);

	CU_add_test(suite_configuracion, "Cargar direcciones IP", cargar_configuracion_ip);
	CU_add_test(suite_configuracion, "Cargar puertos", cargar_configuracion_puertos);
}
