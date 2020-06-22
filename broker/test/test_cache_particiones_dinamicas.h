#ifndef TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_
#define TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_

#include "CUnit/Basic.h"
#include "../broker/main.h"

t_broker_config* broker_config;

void agregar_tests_particiones_dinamicas();
void inicializar_test_con_particion_dinamica();
void test_inicializar_cache_crea_particion_libre();
void test_guardar_un_payload();
void test_leer_payload_desde_particion();
void test_guardar_varias_particiones_no_afecta_particiones_previas();
void test_guardar_crea_particion_intermedia();

t_particion_dinamica* guardar_string_en_particion(const char*);
void assert_primer_particion_esta_libre();
void assert_particion_esta_libre(t_particion_dinamica*);
void assert_particion_tiene_el_tamanio(t_particion_dinamica*,int);

#endif /* TEST_TEST_CACHE_PARTICIONES_DINAMICAS_H_ */
