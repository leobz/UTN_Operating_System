#include "test_caught.h"

void agregar_tests_caught(){
	CU_pSuite suite_configuracion = CU_add_suite("Acciones de CAUGHT", NULL, NULL);

	CU_add_test(suite_configuracion,
				"Al llegar Caught se elimina el pokemon del mapa",
				test_se_elimino_pokemon_del_mapa);

	CU_add_test(suite_configuracion,
				"Al llegar Caught se elimina el pokemon de los planificados",
				test_se_elimino_pokemon_a_planificados);

	CU_add_test(suite_configuracion,
				"Al llegar Caught se saca pasa al entrenador a una cola post_caught",
				test_se_pasa_tcb_a_cola_post_caught);

}

void test_se_elimino_pokemon_del_mapa() {
	mensaje = inicializar_test_appeared();
	t_tcb_entrenador* tcb = list_first(ready);

	aplicar_acciones_caught(tcb);

	t_list* lista_posiciones_pokemon = dictionary_get(pokemones_en_mapa,mensaje->pokemon);
	CU_ASSERT_EQUAL(list_size(lista_posiciones_pokemon), 0);
}

void test_se_elimino_pokemon_a_planificados() {
	mensaje = inicializar_test_appeared();
	t_tcb_entrenador* tcb = list_first(ready);

	aplicar_acciones_caught(tcb);

	int planificados = dictionary_get(pokemones_planificados, mensaje->pokemon);
	CU_ASSERT_EQUAL(planificados, 0);
}

void test_se_pasa_tcb_a_cola_post_caught(){
	mensaje = inicializar_test_appeared();
	t_tcb_entrenador* tcb = list_first(ready);

	aplicar_acciones_caught(tcb);

	CU_ASSERT_EQUAL(tcb->estado_tcb, UNBLOCKED);
}



