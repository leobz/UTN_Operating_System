#ifndef TEST_TEST_BUDDY_SYSTEM_H_
#define TEST_TEST_BUDDY_SYSTEM_H_

#include "CUnit/Basic.h"

#include "../cache.h"

void inicializar_test_bs();
void finalizar_test_bs();

void agregar_tests_buddy_system();
void test_obtener_tamanio_real_memoria_cache_segun_config();
void test_obtener_tamanio_particion_necesario_segun_tamanio_mensaje();
void test_hojas_libres_para_guardar_mensaje();
void test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_FF();
void test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_BF();
void test_division_particion_elegida_acorde_a_tamanio_mensaje();
void test_agregar_mensaje_a_memoria_cache_bs();
void test_leer_mensaje_de_memoria_cache_bs();

#endif /* TEST_TEST_BUDDY_SYSTEM_H_ */
