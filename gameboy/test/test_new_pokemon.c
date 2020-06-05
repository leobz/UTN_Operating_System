/*
 * test_new_pokemon.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "test_new_pokemon.h"

void empaquetar_y_desempaquetar_new_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;
	int cantidad = 3;
	int id_correlativo = 1;

	t_paquete *paquete = crear_paquete(NEW_POKEMON,
			buffer_new_pokemon(pokemon, pos_x, pos_y, cantidad), 0);

	t_mensaje_new *mensaje_new = deserializar_mensaje_new_pokemon(
			paquete->buffer);

	CU_ASSERT_EQUAL(mensaje_new->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_new->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_new->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_new->posy, pos_y);
	CU_ASSERT_EQUAL(mensaje_new->cantidad, cantidad);

	eliminar_paquete(paquete);
	eliminar_mensaje_new(mensaje_new);

}

void serializar_y_deserializar_new_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;
	int cantidad = 3;
	int id_correlativo = 1;
	int bytes;

	void* mensaje_serializado = serializar_new_pokemon(&bytes, pokemon, pos_x,
			pos_y, cantidad, id_correlativo);

	test_deserializar_buffer_new_pokemon(mensaje_serializado, pokemon, pos_x,
			pos_y, cantidad, id_correlativo);

}

void test_deserializar_buffer_new_pokemon(void* mensaje_serializado,
		char* pokemon, int pos_x, int pos_y, int cantidad, int id_correlativo) {

	t_paquete* paquete = crear_paquete_desde_mensaje(mensaje_serializado);
	t_mensaje_new* mensaje_new;

	// ESTA ES LA FUNCION QUE SE TESTEA
	mensaje_new = deserializar_mensaje_new_pokemon(paquete->buffer);

	CU_ASSERT_EQUAL(paquete->codigo_operacion, NEW_POKEMON);
	CU_ASSERT_EQUAL(mensaje_new->length_pokemon, strlen(pokemon) + 1);
	CU_ASSERT_STRING_EQUAL(mensaje_new->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_new->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_new->posy, pos_y);
	CU_ASSERT_EQUAL(mensaje_new->cantidad, cantidad);

	eliminar_paquete(paquete);
	eliminar_mensaje_appeared(mensaje_new);
}

void agregar_tests_new_pokemon() {
	CU_pSuite suite_new_pokemon = CU_add_suite("Mensajes New Pokemon",
			NULL, NULL);

	CU_add_test(suite_new_pokemon,
			"Empaquetar y desempaquetar New Pokemon",
			empaquetar_y_desempaquetar_new_pokemon);

	CU_add_test(suite_new_pokemon,
			"Serializar y deserializar New Pokemon",
			serializar_y_deserializar_new_pokemon);
}
