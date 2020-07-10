#include "test_deadlock.h"

void agregar_tests_deadlock() {
	CU_pSuite suite_configuracion = CU_add_suite("TCBs: Deadlock", NULL,
	NULL);

	CU_add_test(suite_configuracion,
			"Obtener los pokemones que el tcb necesita",
			test_pokemones_necesitados);

	CU_add_test(suite_configuracion,
			"Obtener los pokemones que el tcb no necesita",
			test_pokemones_no_necesitados);
}

void test_pokemones_necesitados() {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	t_dictionary* objetivos_tcb = dictionary_create();
	t_dictionary* pokemones_capturados_tcb = dictionary_create();
	t_list* dif_pokemones;

	dictionary_put(objetivos_tcb, "Pikachu", 1);
	dictionary_put(objetivos_tcb, "Squirtle", 2);

	dictionary_put(pokemones_capturados_tcb, "Pikachu", 1);
	dictionary_put(pokemones_capturados_tcb, "Alakazam", 2);

	tcb->objetivos = objetivos_tcb;
	tcb->pokemones_capturados = pokemones_capturados_tcb;

	dif_pokemones = pokemones_necesitados(tcb);

	CU_ASSERT_EQUAL_FATAL(list_size(dif_pokemones), 1);
	CU_ASSERT_STRING_EQUAL(list_get(dif_pokemones, 0), "Squirtle");

	list_clean(dif_pokemones);
	list_destroy(dif_pokemones);
	dictionary_clean(pokemones_capturados_tcb);
	dictionary_destroy(pokemones_capturados_tcb);
	dictionary_clean(objetivos_tcb);
	dictionary_destroy(objetivos_tcb);
	free(tcb);
}

void test_pokemones_no_necesitados() {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	t_dictionary* objetivos_tcb = dictionary_create();
	t_dictionary* pokemones_capturados_tcb = dictionary_create();
	t_list* dif_pokemones;

	dictionary_put(objetivos_tcb, "Pikachu", 1);
	dictionary_put(objetivos_tcb, "Squirtle", 2);

	dictionary_put(pokemones_capturados_tcb, "Pikachu", 2);
	dictionary_put(pokemones_capturados_tcb, "Squirtle", 1);

	tcb->objetivos = objetivos_tcb;
	tcb->pokemones_capturados = pokemones_capturados_tcb;

	dif_pokemones = pokemones_no_necesitados(tcb);

	CU_ASSERT_EQUAL_FATAL(list_size(dif_pokemones), 1);
	CU_ASSERT_STRING_EQUAL(list_get(dif_pokemones, 0), "Pikachu");

	list_clean(dif_pokemones);
	list_destroy(dif_pokemones);
	dictionary_clean(pokemones_capturados_tcb);
	dictionary_destroy(pokemones_capturados_tcb);
	dictionary_clean(objetivos_tcb);
	dictionary_destroy(objetivos_tcb);
	free(tcb);
}
