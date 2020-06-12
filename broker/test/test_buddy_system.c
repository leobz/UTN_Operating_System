#include "test_buddy_system.h"

void agregar_tests_buddy_system() {
	CU_pSuite suite_configuracion = CU_add_suite("Memoria cache - Buddy System", NULL, NULL);

	CU_add_test(suite_configuracion,
			"En base al tamanio del mensaje, obtener el tamanio de particion necesaria",
			obtener_tamanio_particion_necesario_segun_tamanio_mensaje);
}

void obtener_tamanio_particion_necesario_segun_tamanio_mensaje() {
	int tamanio_mensaje = 70;
	int tamanio_particion_necesaria = 0;

	tamanio_particion_necesaria = obtener_tamanio_particion_necesaria(tamanio_mensaje);

	CU_ASSERT_EQUAL_FATAL(tamanio_particion_necesaria, 128);
}

