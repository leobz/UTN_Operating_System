#ifndef TEST_TEST_ADMINISTRADOR_DE_MENSAJES_H_
#define TEST_TEST_ADMINISTRADOR_DE_MENSAJES_H_

#include "CUnit/Basic.h"
#include "../administracion-de-mensajes.h"
#include "test_cache_particiones_dinamicas.h"

void agregar_tests_administrador_de_mensaje();
void test_guardar_mensaje_en_cache_dinamica();
t_mensaje* mensaje_generico(char*payload_string,int payload_string_size);
void inicializar_test_con_administracion_de_mensajes();
#endif /* TEST_TEST_ADMINISTRADOR_DE_MENSAJES_H_ */
