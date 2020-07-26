#include "test_unblocked_a_ready.h"

void test_se_asigna_pokemon_a_tcb(t_tcb_entrenador* tcb) {
	CU_ASSERT_STRING_EQUAL(tcb->pokemon_a_capturar->pokemon, "Pikachu");
	CU_ASSERT_EQUAL(tcb->pokemon_a_capturar->posicion->x, 5);
	CU_ASSERT_EQUAL(tcb->pokemon_a_capturar->posicion->y, 6);
}

void test_pasar_tcb_a_ready_si_hay_pokemones_en_mapa() {
	t_mensaje_appeared* mensaje = create_mensaje_appeared("Pikachu", 5, 6);
	t_tcb_entrenador* tcb = tcb_generico(1);
	list_clean(ready);
	list_clean(unblocked);


	agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje);
	list_add(unblocked, tcb);
	pasar_tcb_a_ready_si_hay_pokemones_en_mapa(tcb);

	// Se elimina de Unblocked y pasa a Ready
	CU_ASSERT_EQUAL(list_size(unblocked), 0);
	CU_ASSERT_EQUAL(list_size(ready), 1);

	CU_ASSERT_PTR_EQUAL(tcb, list_first(ready));
	CU_ASSERT_EQUAL(tcb->estado_tcb, READY);

	test_se_asigna_pokemon_a_tcb(tcb);
	free(mensaje);
}


void test_no_pasar_tcb_a_ready_si_no_hay_pokemones_en_mapa() {
	t_tcb_entrenador* tcb = tcb_generico(1);
	list_clean(ready);
	list_clean(unblocked);

	list_add(unblocked, tcb);
	pasar_tcb_a_ready_si_hay_pokemones_en_mapa(tcb);

	// Se conserva en Unblocked y no pasa a Ready
	CU_ASSERT_EQUAL(list_size(ready), 0);
	CU_ASSERT_EQUAL(list_size(unblocked), 1);
}

void agregar_tests_unblocked_a_ready() {
	CU_pSuite suite_configuracion = CU_add_suite("Unblocked a ready", NULL,
	NULL);

	CU_add_test(suite_configuracion, "Pasar a ready si hay pokemon en el mapa",
			test_pasar_tcb_a_ready_si_hay_pokemones_en_mapa);

	CU_add_test(suite_configuracion, "No Pasar a ready si no hay pokemon en el mapa",
			test_no_pasar_tcb_a_ready_si_no_hay_pokemones_en_mapa);


}
