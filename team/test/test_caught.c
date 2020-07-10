#include "test_caught.h"

t_mensaje_appeared* mock_mensaje_appeared() {
	t_mensaje_appeared* mensaje = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));
	mensaje->posx = 9;
	mensaje->posy = 9;
	mensaje->pokemon = strdup("pikachu");
	return mensaje;
}

void vaciar_especie_en_mapa(t_mensaje_appeared* mensaje) {
	agregar_pokemon_a_mapa(mensaje->pokemon, list_create());
}

void inicalizar_test() {
	list_clean(ready);
}

void test_se_elimino_pokemon_del_mapa(t_tcb_entrenador* tcb, t_mensaje_appeared* mensaje) {
	t_list* lista_posiciones_pokemon = dictionary_get(pokemones_en_mapa,mensaje->pokemon);

	CU_ASSERT_EQUAL(list_size(lista_posiciones_pokemon), 0);
}

void test_se_agrega_pokemon_al_mapa(t_mensaje_appeared* mensaje) {
	t_list* lista_posiciones_pokemon = dictionary_get(pokemones_en_mapa,
			mensaje->pokemon);
	CU_ASSERT_EQUAL(list_size(lista_posiciones_pokemon), 1);
}

void test_se_agrega_pokemon_a_planificados(t_mensaje_appeared* mensaje) {
	int planificados = dictionary_get(pokemones_planificados, mensaje->pokemon);
	CU_ASSERT_EQUAL(planificados, 1);
}

void test_se_elimino_pokemon_a_planificados(t_mensaje_appeared* mensaje) {
	int planificados = dictionary_get(pokemones_planificados, mensaje->pokemon);
	CU_ASSERT_EQUAL(planificados, 0);
}

void test_tcb_paso_a_ready() {
	CU_ASSERT_EQUAL(list_size(ready), 1);
}

void mock_acciones_appeared(t_mensaje_appeared* mensaje) {
	vaciar_especie_en_mapa(mensaje);
	agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje);
	dictionary_increment_value(pokemones_planificados, mensaje->pokemon);
	pasar_entrenador_a_ready_segun_cercania(mensaje);
}

void test_se_elimina_pokemon_del_mapa(){
	inicalizar_test();

	t_mensaje_appeared* mensaje = mock_mensaje_appeared();
	mock_acciones_appeared(mensaje);

	test_se_agrega_pokemon_al_mapa(mensaje);
	test_se_agrega_pokemon_a_planificados(mensaje);
	test_tcb_paso_a_ready();

	t_tcb_entrenador* tcb = list_first(ready);
	aplicar_acciones_caught(tcb);

	test_se_elimino_pokemon_del_mapa(tcb, mensaje);
	test_se_elimino_pokemon_a_planificados(mensaje);
	CU_ASSERT_EQUAL(tcb->estado_tcb, DEADLOCK);
}

void agregar_tests_caught(){
	CU_pSuite suite_configuracion = CU_add_suite("Acciones de CAUGHT", NULL, NULL);

	CU_add_test(suite_configuracion,
				"Al llegar Caught se elimina el pokemon del mapa",
				test_se_elimina_pokemon_del_mapa);
}


