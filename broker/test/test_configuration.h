/*
 * test_configuration.h
 *
 *  Created on: May 10, 2020
 *      Author: diego
 */

#ifndef TEST_TEST_CONFIGURATION_H_
#define TEST_TEST_CONFIGURATION_H_

#include "CUnit/Basic.h"
#include "../broker/server.h"

void cargar_configuracion_ip();
void cargar_configuracion_puertos();
void agregar_tests_configuracion();

#endif /* TEST_TEST_CONFIGURATION_H_ */
