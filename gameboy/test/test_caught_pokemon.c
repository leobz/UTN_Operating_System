/*
 * test_caught_pokemon.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "test_caught_pokemon.h"

void empaquetar_y_desempaquetar_caught_pokemon() {
	estado estado_actual = OK;

	t_paquete* paquete = crear_paquete(CAUGHT_POKEMON,
			buffer_caught_pokemon(estado_actual), 0);

	estado* mensaje_caught = deserializar_mensaje_caught_pokemon(
			paquete->buffer);

	CU_ASSERT_EQUAL(mensaje_caught, estado_actual);

	eliminar_paquete(paquete);
	// eliminar_mensaje_caught(mensaje_caught);

}

void serializar_y_deserializar_caught_pokemon() {
	estado estado = OK;
	int bytes;
	int id_correlativo;

	void* mensaje_serializado = serializar_caught_pokemon(&bytes, estado,
			id_correlativo);

	test_deserializar_buffer_caught_pokemon(mensaje_serializado, estado,
			id_correlativo);

}

void test_deserializar_buffer_caught_pokemon(void* mensaje_serializado,
		int estado_actual, int id_correlativo) {

	t_paquete* paquete = crear_paquete_desde_mensaje(mensaje_serializado);
	estado* mensaje_caught;

	// ESTA ES LA FUNCION QUE SE TESTEA
	mensaje_caught = deserializar_mensaje_caught_pokemon(paquete->buffer);

	CU_ASSERT_EQUAL(paquete->codigo_operacion, CAUGHT_POKEMON);
	CU_ASSERT_EQUAL(mensaje_caught, OK);

	eliminar_paquete(paquete);
}

void agregar_tests_caught_pokemon() {
	CU_pSuite suite_caught_pokemon = CU_add_suite("Mensajes caught Pokemon",
	NULL, NULL);

	CU_add_test(suite_caught_pokemon,
			"Empaquetar y desempaquetar Caught Pokemon",
			empaquetar_y_desempaquetar_caught_pokemon);

	CU_add_test(suite_caught_pokemon,
			"Serializar y deserializar Caught Pokemon",
			serializar_y_deserializar_caught_pokemon);
}
