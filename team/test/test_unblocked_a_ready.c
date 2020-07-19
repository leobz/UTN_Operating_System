#include "test_unblocked_a_ready.h"


void test_pasar_tcb_a_ready_si_hay_pokemones_en_mapa() {
	t_mensaje_appeared* mensaje = create_mensaje_appeared("Pikachu", 5, 6);
	agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje);

	t_tcb_entrenador* tcb_1 = tcb_generico(1);
	list_add(unblocked, tcb_1);

	pasar_tcb_a_ready_si_hay_pokemones_en_mapa(tcb_1);

}

void agregar_tests_unblocked_a_ready() {
	CU_pSuite suite_configuracion = CU_add_suite("Unblocked a ready", NULL,
	NULL);

	CU_add_test(suite_configuracion, "Pasar a ready si hay pokemon en el mapa",
			test_pasar_tcb_a_ready_si_hay_pokemones_en_mapa);
}
