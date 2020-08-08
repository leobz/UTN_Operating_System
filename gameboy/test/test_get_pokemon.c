/*
 * test_get_pokemon.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "test_get_pokemon.h"

void empaquetar_y_desempaquetar_get_pokemon() {
	char* pokemon = "pikachu";

	t_paquete *paquete = crear_paquete(GET_POKEMON, buffer_get_pokemon(pokemon),0,0);

	t_mensaje_get *mensaje_get = deserializar_mensaje_get_pokemon(
			paquete->buffer);

	CU_ASSERT_EQUAL(mensaje_get->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_get->pokemon, pokemon);

	eliminar_paquete(paquete);
	eliminar_mensaje_get(mensaje_get);

}

void serializar_y_deserializar_get_pokemon() {
	char* pokemon = "pikachu";
	int id_correlativo = 1;
	int bytes;

	void* mensaje_serializado = serializar_get_pokemon(&bytes, pokemon,0,id_correlativo);

	test_deserializar_buffer_get_pokemon(mensaje_serializado, pokemon);

}

void test_deserializar_buffer_get_pokemon(void* mensaje_serializado,
		char* pokemon) {

	t_paquete* paquete = crear_paquete_desde_mensaje(mensaje_serializado);
	t_mensaje_get* mensaje_get;

	// ESTA ES LA FUNCION QUE SE TESTEA
	mensaje_get = deserializar_mensaje_get_pokemon(paquete->buffer);

	CU_ASSERT_EQUAL(paquete->codigo_operacion, GET_POKEMON);
	CU_ASSERT_EQUAL(mensaje_get->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_get->pokemon, pokemon);

	eliminar_paquete(paquete);
	eliminar_mensaje_appeared(mensaje_get);
}

void agregar_tests_get_pokemon() {
	CU_pSuite suite_get_pokemon = CU_add_suite("Mensajes Get Pokemon",
	NULL, NULL);

	CU_add_test(suite_get_pokemon, "Empaquetar y desempaquetar Get Pokemon",
			empaquetar_y_desempaquetar_get_pokemon);

	CU_add_test(suite_get_pokemon, "Serializar y deserializar Get Pokemon",
			serializar_y_deserializar_get_pokemon);
}
