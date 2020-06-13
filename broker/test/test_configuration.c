/*
 * test_configuration.c
 *
 *  Created on: May 10, 2020
 *      Author: diego
 */

#include "test_configuration.h"

void cargar_configuracion_ip() {
	t_broker_config *broker_config = cargar_broker_config(
			"broker.config.sample");

	CU_ASSERT_STRING_EQUAL(broker_config->ip_broker, "127.0.0.1");


	destruir_broker_config(broker_config);
}

void cargar_configuracion_puertos() {
	t_broker_config *broker_config = cargar_broker_config(
			"broker.config.sample");

	CU_ASSERT_STRING_EQUAL(broker_config->puerto_broker, "5001");

	destruir_broker_config(broker_config);
}

void agregar_tests_configuracion(){
	CU_pSuite suite_configuracion = CU_add_suite("Cargar configuracion del Broker", NULL, NULL);

	CU_add_test(suite_configuracion, "Cargar direcciones IP", cargar_configuracion_ip);
	CU_add_test(suite_configuracion, "Cargar puertos", cargar_configuracion_puertos);
}

