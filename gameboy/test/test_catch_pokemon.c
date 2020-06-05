/*
 * test_catch_pokemon.c
 *
 *  Created on: 3 jun. 2020
 *      Author: utnso
 */

#include "test_catch_pokemon.h"

// ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]


void empaquetar_y_desempaquetar_catch_pokemon() {

	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;

	t_paquete* paquete = crear_paquete(CATCH_POKEMON,
			buffer_catch_pokemon(pokemon, pos_x, pos_y), 0);

	t_mensaje_catch* mensaje_catch = deserializar_mensaje_catch_pokemon(
			paquete->buffer);

	CU_ASSERT_EQUAL(mensaje_catch->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_catch->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_catch->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_catch->posy, pos_y);

	eliminar_paquete(paquete);
	eliminar_mensaje_catch(mensaje_catch);
}

void serializar_y_deserializar_catch_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;
	int bytes;

	void* mensaje_serializado = serializar_catch_pokemon(&bytes, pokemon, pos_x,
			pos_y, 0);

	test_deserializar_buffer_catch_pokemon(mensaje_serializado, pokemon, pos_x,
			pos_y);

}

void test_deserializar_buffer_catch_pokemon(void* mensaje_serializado,
		char* pokemon, int pos_x, int pos_y) {
	t_paquete* paquete = crear_paquete_desde_mensaje(mensaje_serializado);
	t_mensaje_appeared* mensaje_catch;

	// ESTA ES LA FUNCION QUE SE TESTEA
	mensaje_catch = deserializar_mensaje_catch_pokemon(paquete->buffer);

	CU_ASSERT_EQUAL(paquete->codigo_operacion, CATCH_POKEMON);
	CU_ASSERT_EQUAL(mensaje_catch->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_catch->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_catch->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_catch->posy, pos_y);

	eliminar_paquete(paquete);
	eliminar_mensaje_catch(mensaje_catch);
}

void agregar_tests_catch_pokemon() {
	CU_pSuite suite_catch_pokemon = CU_add_suite("Mensajes Catch Pokemon",
	NULL, NULL);

	CU_add_test(suite_catch_pokemon, "Empaquetar y desempaquetar Catch Pokemon",
			empaquetar_y_desempaquetar_catch_pokemon);

	CU_add_test(suite_catch_pokemon, "Serializar y deserializar Catch Pokemon",
			serializar_y_deserializar_catch_pokemon);
}
