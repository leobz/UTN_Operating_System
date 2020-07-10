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
#include "test_cache_particiones_dinamicas.h"

#define TAMANO_MEMORIA 50

t_broker_config* broker_configure;

void agregar_tests_algoritmos();
void inicializar_test_algoritmos_bf();
void inicializar_test_algoritmos_ff();
void iniciar_listas_y_diccionarios();
void iniciar_listas_diccionarios_colas(int cola);
t_adm_mensaje* iniciar_adm(t_adm_mensaje *adm,op_code cola);
void iniciar_envio_de_mensajes();
void liberar_listas_y_diccionarios();
void finalizar_test_algoritmos();

void test_eliminacion_y_consolidacion_dinamicas();
void test_bestfit_y_lru_dinamicas();
void test_firstfit_y_fifo_dinamicas();
void test_compactacion_dinamicas();

int contador_id_mensaje;
int contador_id_correlativo;

#endif /* BROKER_TEST_TEST_ALGORITMOS_DINAMICAS_H_ */
