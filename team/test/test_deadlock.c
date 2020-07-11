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

	CU_add_test(suite_configuracion,
			"Obtener una lista con los pokemones en comun entre dos listas",
			test_list_intersection);

	CU_add_test(suite_configuracion,
			"Generar estructura deadlock con tcbs y pokemones",
			test_crear_deadlock);

	CU_add_test(suite_configuracion,
			"Detectar posible deadlock de un tcb",
			test_detectar_deadlock);
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

void test_list_intersection(){
	t_list* primeros_pokemones = list_create();
	t_list* segundos_pokemones = list_create();
	t_list* inserseccion_pokemones;

	list_add(primeros_pokemones, "Pikachu");
	list_add(primeros_pokemones, "Squirtle");
	list_add(primeros_pokemones, "Charmander");

	list_add(segundos_pokemones, "Ditto");
	list_add(segundos_pokemones, "Charmander");
	list_add(segundos_pokemones, "Abra");
	list_add(segundos_pokemones, "Pikachu");

	inserseccion_pokemones = list_intersection(primeros_pokemones, segundos_pokemones);

	CU_ASSERT_EQUAL_FATAL(list_size(inserseccion_pokemones), 2);
	CU_ASSERT_STRING_EQUAL_FATAL(list_get(inserseccion_pokemones, 0), "Pikachu");
	CU_ASSERT_STRING_EQUAL_FATAL(list_get(inserseccion_pokemones, 1), "Charmander");

	list_clean(primeros_pokemones);
	list_destroy(primeros_pokemones);
	list_clean(segundos_pokemones);
	list_destroy(segundos_pokemones);
	list_clean(inserseccion_pokemones);
	list_destroy(inserseccion_pokemones);
}

void test_crear_deadlock() {
	t_tcb_entrenador* primer_tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* segundo_tcb = malloc(sizeof(t_tcb_entrenador));
	t_list* primeros_pokemones = list_create();
	t_list* segundos_pokemones = list_create();
	t_deadlock* deadlock = NULL;

	list_add(primeros_pokemones, "Pikachu");
	list_add(primeros_pokemones, "Alakazam");

	list_add(segundos_pokemones, "Mew");
	list_add(segundos_pokemones, "Arbok");

	deadlock = crear_deadlock(primer_tcb, segundo_tcb, primeros_pokemones, segundos_pokemones);

	CU_ASSERT_TRUE_FATAL(deadlock != NULL);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_1, primer_tcb);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_2, segundo_tcb);
	CU_ASSERT_EQUAL_FATAL(primer_tcb->estado_tcb, DEADLOCK);
	CU_ASSERT_EQUAL_FATAL(segundo_tcb->estado_tcb, DEADLOCK);
	CU_ASSERT_EQUAL_FATAL(primer_tcb->entrenador_a_intercambiar, segundo_tcb);
	CU_ASSERT_EQUAL_FATAL(segundo_tcb->entrenador_a_intercambiar, primer_tcb);
	CU_ASSERT_STRING_EQUAL_FATAL(primer_tcb->pokemon_a_dar_en_intercambio, list_get(primeros_pokemones, 0));
	CU_ASSERT_STRING_EQUAL_FATAL(segundo_tcb->pokemon_a_dar_en_intercambio, list_get(segundos_pokemones, 0));

	free(deadlock);
	list_clean(primeros_pokemones);
	list_destroy(primeros_pokemones);
	list_clean(segundos_pokemones);
	list_destroy(segundos_pokemones);
	free(segundo_tcb);
	free(primer_tcb);
}

void test_detectar_deadlock() {
	t_tcb_entrenador* primer_tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* segundo_tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tercer_tcb = malloc(sizeof(t_tcb_entrenador));

	t_dictionary* primer_tcb_objetivos = dictionary_create();
	t_dictionary* primer_tcb_pokemones_capturados = dictionary_create();
	t_dictionary* tercer_tcb_objetivos = dictionary_create();
	t_dictionary* tercer_tcb_pokemones_capturados = dictionary_create();

	t_deadlock* deadlock = NULL;

	/*
	 * Primer tcb:
	 * - Pokemones necesarios: "Squirtle"
	 * - Pokemones no necesarios: "Alakazam"
	*/

	dictionary_put(primer_tcb_objetivos, "Pikachu", 1);
	dictionary_put(primer_tcb_objetivos, "Squirtle", 2);

	dictionary_put(primer_tcb_pokemones_capturados, "Alakazam", 2);
	dictionary_put(primer_tcb_pokemones_capturados, "Pikachu", 1);

	primer_tcb->estado_tcb = READY_TO_EXCHANGE;
	primer_tcb->objetivos = primer_tcb_objetivos;
	primer_tcb->pokemones_capturados = primer_tcb_pokemones_capturados;

	/*
	 * Segundo tcb: No se tomará en cuenta debido a su estado DEADLOCK
	*/

	segundo_tcb->estado_tcb = DEADLOCK;

	/*
	 * Tercer tcb:
	 * - Pokemones necesarios: "Alakazam"
	 * - Pokemones no necesarios: "Squirtle", "Pidgey"
	*/

	dictionary_put(tercer_tcb_objetivos, "Charmander", 1);
	dictionary_put(tercer_tcb_objetivos, "Alakazam", 2);

	dictionary_put(tercer_tcb_pokemones_capturados, "Squirtle", 1);
	dictionary_put(tercer_tcb_pokemones_capturados, "Charmander", 1);
	dictionary_put(tercer_tcb_pokemones_capturados, "Pidgey", 1);

	tercer_tcb->estado_tcb = READY_TO_EXCHANGE;
	tercer_tcb->objetivos = tercer_tcb_objetivos;
	tercer_tcb->pokemones_capturados = tercer_tcb_pokemones_capturados;

	list_add(ready_to_exchange, primer_tcb);
	list_add(ready_to_exchange, segundo_tcb);
	list_add(ready_to_exchange, tercer_tcb);

	deadlock = detectar_deadlock(primer_tcb);

	CU_ASSERT_TRUE_FATAL(deadlock != NULL);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_1, primer_tcb);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_2, tercer_tcb);
	CU_ASSERT_EQUAL_FATAL(primer_tcb->estado_tcb, DEADLOCK);
	CU_ASSERT_EQUAL_FATAL(tercer_tcb->estado_tcb, DEADLOCK);
	CU_ASSERT_EQUAL_FATAL(primer_tcb->entrenador_a_intercambiar, tercer_tcb);
	CU_ASSERT_EQUAL_FATAL(tercer_tcb->entrenador_a_intercambiar, primer_tcb);
	CU_ASSERT_STRING_EQUAL_FATAL(primer_tcb->pokemon_a_dar_en_intercambio, "Alakazam");
	CU_ASSERT_STRING_EQUAL_FATAL(tercer_tcb->pokemon_a_dar_en_intercambio, "Squirtle");

	free(deadlock);
	dictionary_clean(tercer_tcb_pokemones_capturados);
	dictionary_destroy(tercer_tcb_pokemones_capturados);
	dictionary_clean(tercer_tcb_objetivos);
	dictionary_destroy(tercer_tcb_objetivos);
	dictionary_clean(primer_tcb_pokemones_capturados);
	dictionary_destroy(primer_tcb_pokemones_capturados);
	dictionary_clean(primer_tcb_objetivos);
	dictionary_destroy(primer_tcb_objetivos);
	free(tercer_tcb);
	free(segundo_tcb);
	free(primer_tcb);
}
