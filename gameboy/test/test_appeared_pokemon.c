#include "test_appeared_pokemon.h"

void empaquetar_y_desempaquetar_appeared_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;

	t_paquete *paquete = crear_paquete(APPEARED_POKEMON,
			buffer_appeared_pokemon(pokemon, pos_x, pos_y));

	// TODO: DESEMPAQUETAR PAQUETE y ASSERTS

	eliminar_paquete(paquete);
}
void serializar_y_deserializar_appeared_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;
	int bytes;

	void* mensaje_serializado = serializar_appeared_pokemon(&bytes, pokemon,
			pos_x, pos_y);

	// TODO: DESERIALIZAR MENSAJE y ASSERTS

}
void agregar_tests_appeared_pokemon() {
	CU_pSuite suite_appeared_pokemon = CU_add_suite("Mensajes Appeared Pokemon",
			NULL, NULL);

	CU_add_test(suite_appeared_pokemon,
			"Empaquetar y desempaquetar Appeared Pokemon",
			empaquetar_y_desempaquetar_appeared_pokemon);

	CU_add_test(suite_appeared_pokemon,
			"Serializar y deserializar Appeared Pokemon",
			serializar_y_deserializar_appeared_pokemon);

}

