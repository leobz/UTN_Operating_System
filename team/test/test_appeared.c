#include "test_appeared.h"

void agregar_tests_appeared(){
	CU_pSuite suite_configuracion = CU_add_suite("Acciones de APPEARED", NULL, NULL);

	CU_add_test(suite_configuracion,
				"Appeared agrega pokemon al mapa",
				test_se_agrega_pokemon_al_mapa);

	CU_add_test(suite_configuracion,
				"Appeared agrega pokemon a planificados",
				test_se_agrega_pokemon_a_planificados);
	CU_add_test(suite_configuracion,
				"Appeared pasa entrenador a Ready",
				test_tcb_paso_a_ready);

}

t_mensaje_appeared* inicializar_test_appeared() {
	list_clean(ready);
	temp = list_create();
	t_mensaje_appeared* mensaje = mock_appeared();
	return mensaje;
}

void test_se_agrega_pokemon_al_mapa() {
	mensaje = inicializar_test_appeared();

	t_list* lista_posiciones_pokemon = dictionary_get(pokemones_en_mapa, mensaje->pokemon);
	CU_ASSERT_EQUAL(list_size(lista_posiciones_pokemon), 1);
}

void test_se_agrega_pokemon_a_planificados() {
	mensaje = inicializar_test_appeared();

	int planificados = dictionary_get(pokemones_planificados, mensaje->pokemon);
	CU_ASSERT_EQUAL(planificados, 1);
}

void test_tcb_paso_a_ready() {
	mensaje = inicializar_test_appeared();

	CU_ASSERT_EQUAL(list_size(ready), 1);
}

t_mensaje_appeared* mock_mensaje_appeared() {
	t_mensaje_appeared* mensaje = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));
	mensaje->posx = 9;
	mensaje->posy = 9;
	mensaje->pokemon = strdup("pikachu");
	return mensaje;
}

void backupear_cola_new() {
	temp = list_duplicate(new);
}

void restaurar_cola_new() {
	new = temp;
}

t_mensaje_appeared* mock_appeared() {
	t_mensaje_appeared* mensaje = mock_mensaje_appeared();
	backupear_cola_new();
	mock_acciones_appeared(mensaje);
	restaurar_cola_new();

	return mensaje;
}

void vaciar_especie_en_mapa(t_mensaje_appeared* mensaje) {
	agregar_pokemon_a_mapa(mensaje->pokemon, list_create());
}

void vaciar_especie_en_planificados(t_mensaje_appeared* mensaje) {
	dictionary_remove(pokemones_planificados, mensaje->pokemon);
}

void mock_acciones_appeared(t_mensaje_appeared* mensaje) {
	vaciar_especie_en_mapa(mensaje);
	vaciar_especie_en_planificados(mensaje);
	agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje);
	dictionary_increment_value(pokemones_planificados, mensaje->pokemon);
	pasar_entrenador_a_ready_segun_cercania(mensaje);
}


