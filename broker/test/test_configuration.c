/*
 * test_configuration.c
 *
 *  Created on: May 10, 2020
 *      Author: diego
 */

#include "test_configuration.h"

void cargar_configuracion_ip() {
	t_broker_config *gameboy_config = cargar_gameboy_config(
			"gameboy.config.sample");

	CU_ASSERT_STRING_EQUAL(gameboy_config->ip_broker, "127.0.0.1");


	destruir_gameboy_config(gameboy_config);
}

void cargar_configuracion_puertos() {
	t_broker_config *gameboy_config = cargar_gameboy_config(
			"gameboy.config.sample");

	CU_ASSERT_STRING_EQUAL(gameboy_config->puerto_broker, "4444");

	destruir_gameboy_config(gameboy_config);
}

void agregar_tests_configuracion(){
	CU_pSuite suite_configuracion = CU_add_suite("Cargar configuracion del Broker", NULL, NULL);

	CU_add_test(suite_configuracion, "Cargar direcciones IP", cargar_configuracion_ip);
	CU_add_test(suite_configuracion, "Cargar puertos", cargar_configuracion_puertos);
}

