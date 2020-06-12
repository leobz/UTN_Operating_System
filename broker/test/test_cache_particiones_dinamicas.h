#ifndef TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_
#define TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_

#include "CUnit/Basic.h"
#include "../broker/main.h"

t_broker_config* broker_config;

void agregar_tests_particiones_dinamicas();
void inicaliza_cache_con_particion_libre();
void test_guardar_un_payload();
void test_leer_payload_desde_particion();

void assert_primer_particion_esta_libre();
void assert_particion_esta_libre(t_particion_dinamica*);
void assert_particion_tiene_el_tamanio(t_particion_dinamica*,int);

#endif /* TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_ */
