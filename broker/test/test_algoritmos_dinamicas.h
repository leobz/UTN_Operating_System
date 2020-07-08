/*
 * test_algoritmos_dinamicas.h
 *
 *  Created on: Jul 8, 2020
 *      Author: diego
 */

#ifndef BROKER_TEST_TEST_ALGORITMOS_DINAMICAS_H_
#define BROKER_TEST_TEST_ALGORITMOS_DINAMICAS_H_

#include "CUnit/Basic.h"
#include "../broker/main.h"

#define TAMANIO_MEMORIA 100

t_broker_config* broker_configure;

void agregar_tests_algoritmos();
void inicializar_test_algoritmos();
void finalizar_test_algoritmos();


#endif /* BROKER_TEST_TEST_ALGORITMOS_DINAMICAS_H_ */
