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
			"Ejecutar intercambio de pokemon entre dos entrenadores",
			test_ejecutar_intercambio);

	CU_add_test(suite_configuracion,
			"Ejecutar las acciones correspondientes post intercambio para un entrenador que no haya cumplido su objetivo",
			test_ejecutar_acciones_post_intercambio_para_tcb_sin_objetivo_cumplido);

	CU_add_test(suite_configuracion,
			"Ejecutar las acciones correspondientes post intercambio para un entrenador que haya cumplido su objetivo",
			test_ejecutar_acciones_post_intercambio_para_tcb_con_objetivo_cumplido);
  
	CU_add_test(suite_configuracion,
      "Despachar deadlock carga rafaga de TCB y lo envia a ready",
			test_despachar_deadlock_carga_la_rafaga_del_tcb_y_lo_envia_a_ready);

	CU_add_test(suite_configuracion,
			"Detectar deadlock recursivamente",
			test_detectar_dealock_recursivo);

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
	t_tcb_entrenador* tcb_anterior = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_actual = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_a_intercambiar = malloc(sizeof(t_tcb_entrenador));

	t_dictionary* tcb_anterior_objetivos = dictionary_create();
	t_dictionary* tcb_anterior_pokemones_capturados = dictionary_create();

	t_dictionary* tcb_actual_objetivos = dictionary_create();
	t_dictionary* tcb_actual_pokemones_capturados = dictionary_create();

	t_dictionary* tcb_a_intercambiar_objetivos = dictionary_create();
	t_dictionary* tcb_a_intercambiar_pokemones_capturados = dictionary_create();

	t_list* lista_deadlock = list_create();
	t_deadlock* deadlock = NULL;

	/*
	 * Tcb actual:
	 * - Pokemones necesarios: "Lapras"
	 * - Pokemones no necesarios: "Spearow"
	*/

	dictionary_put(tcb_actual_objetivos, "Charmander", 1);
	dictionary_put(tcb_actual_objetivos, "Lapras", 2);

	dictionary_put(tcb_actual_pokemones_capturados, "Spearow", 1);
	dictionary_put(tcb_actual_pokemones_capturados, "Charmander", 1);
	dictionary_put(tcb_actual_pokemones_capturados, "Lapras", 1);

	tcb_actual->estado_tcb = DEADLOCK;
	tcb_actual->objetivos = tcb_actual_objetivos;
	tcb_actual->pokemones_capturados = tcb_actual_pokemones_capturados;

	/*
	 * Tcb anterior:
	 * - Pokemones necesarios: "Spearow"
	 * - Pokemones no necesarios: "Abra", "Muk"
	*/

	dictionary_put(tcb_anterior_objetivos, "Pidgey", 1);
	dictionary_put(tcb_anterior_objetivos, "Spearow", 2);

	dictionary_put(tcb_anterior_pokemones_capturados, "Muk", 1);
	dictionary_put(tcb_anterior_pokemones_capturados, "Abra", 1);
	dictionary_put(tcb_anterior_pokemones_capturados, "Pidgey", 1);

	tcb_anterior->estado_tcb = DEADLOCK;
	tcb_anterior->objetivos = tcb_anterior_objetivos;
	tcb_anterior->pokemones_capturados = tcb_anterior_pokemones_capturados;


	/*
	 * Tcb a intercambiar:
	 * - Pokemones necesarios: "Abra"
	 * - Pokemones no necesarios: "Lapras"
	*/

	dictionary_put(tcb_a_intercambiar_objetivos, "Abra", 1);
	dictionary_put(tcb_a_intercambiar_objetivos, "Caterpie", 1);

	dictionary_put(tcb_a_intercambiar_pokemones_capturados, "Lapras", 1);
	dictionary_put(tcb_a_intercambiar_pokemones_capturados, "Caterpie", 1);

	tcb_a_intercambiar->estado_tcb = DEADLOCK;
	tcb_a_intercambiar->objetivos = tcb_a_intercambiar_objetivos;
	tcb_a_intercambiar->pokemones_capturados = tcb_a_intercambiar_pokemones_capturados;

	list_add(lista_deadlock, tcb_actual);
	list_add(lista_deadlock, tcb_anterior);
	list_add(lista_deadlock, tcb_a_intercambiar);

	deadlock = crear_deadlock(lista_deadlock);

	CU_ASSERT_TRUE_FATAL(deadlock != NULL);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_1, tcb_actual);
	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_2, tcb_a_intercambiar);
	CU_ASSERT_EQUAL_FATAL(tcb_actual->entrenador_a_intercambiar, tcb_a_intercambiar);
	CU_ASSERT_EQUAL_FATAL(tcb_a_intercambiar->entrenador_a_intercambiar, tcb_actual);
	CU_ASSERT_STRING_EQUAL_FATAL(tcb_actual->pokemon_a_dar_en_intercambio, "Spearow");
	CU_ASSERT_STRING_EQUAL_FATAL(tcb_a_intercambiar->pokemon_a_dar_en_intercambio, "Lapras");

	free(deadlock);
	dictionary_clean(tcb_a_intercambiar_pokemones_capturados);
	dictionary_destroy(tcb_a_intercambiar_pokemones_capturados);
	dictionary_clean(tcb_a_intercambiar_objetivos);
	dictionary_destroy(tcb_a_intercambiar_objetivos);
	dictionary_clean(tcb_actual_pokemones_capturados);
	dictionary_destroy(tcb_actual_pokemones_capturados);
	dictionary_clean(tcb_actual_objetivos);
	dictionary_destroy(tcb_actual_objetivos);
	dictionary_clean(tcb_anterior_pokemones_capturados);
	dictionary_destroy(tcb_anterior_pokemones_capturados);
	dictionary_clean(tcb_anterior_objetivos);
	dictionary_destroy(tcb_anterior_objetivos);
	list_clean(lista_deadlock);
	list_destroy(lista_deadlock);
	free(tcb_a_intercambiar);
	free(tcb_actual);
	free(tcb_anterior);
}

void test_ejecutar_intercambio() {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_a_intercambiar = malloc(sizeof(t_tcb_entrenador));
	t_dictionary* tcb_capturados = dictionary_create();
	t_dictionary* tcb_a_intercambiar_capturados = dictionary_create();

	dictionary_put(tcb_capturados, "Alakazam", 1);
	dictionary_put(tcb_a_intercambiar_capturados, "Pidgey", 2);

	tcb->pokemones_capturados = tcb_capturados;
	tcb->pokemon_a_dar_en_intercambio = "Alakazam";
	tcb->entrenador_a_intercambiar = tcb_a_intercambiar;

	tcb_a_intercambiar->pokemones_capturados = tcb_a_intercambiar_capturados;
	tcb_a_intercambiar->pokemon_a_dar_en_intercambio = "Pidgey";
	tcb_a_intercambiar->entrenador_a_intercambiar = tcb;

	ejecutar_intercambio(tcb);

	CU_ASSERT_EQUAL_FATAL(dictionary_get(tcb->pokemones_capturados, "Pidgey"), 1);
	CU_ASSERT_FALSE_FATAL(dictionary_has_key(tcb->pokemones_capturados, "Alakazam"));
	CU_ASSERT_TRUE_FATAL(tcb->entrenador_a_intercambiar == NULL);
	CU_ASSERT_TRUE_FATAL(tcb->pokemon_a_dar_en_intercambio == NULL);

	CU_ASSERT_EQUAL_FATAL(dictionary_get(tcb_a_intercambiar->pokemones_capturados, "Alakazam"), 1);
	CU_ASSERT_EQUAL_FATAL(dictionary_get(tcb_a_intercambiar->pokemones_capturados, "Pidgey"), 1);
	CU_ASSERT_TRUE_FATAL(tcb_a_intercambiar->entrenador_a_intercambiar == NULL);
	CU_ASSERT_TRUE_FATAL(tcb_a_intercambiar->pokemon_a_dar_en_intercambio == NULL);

	dictionary_clean(tcb_a_intercambiar_capturados);
	dictionary_destroy(tcb_a_intercambiar_capturados);
	dictionary_clean(tcb_capturados);
	dictionary_destroy(tcb_capturados);
	free(tcb_a_intercambiar);
	free(tcb);
}

void test_ejecutar_acciones_post_intercambio_para_tcb_sin_objetivo_cumplido() {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	t_dictionary* objetivos = dictionary_create();
	t_dictionary* capturados = dictionary_create();
	t_posicion* posicion = malloc(sizeof(t_posicion));

	ready = list_create();
	deadlock_actual = list_create();
	ready_to_exchange = list_create();

	posicion->x = 1;
	posicion->y = 1;

	dictionary_put(objetivos, "Pikachu", 1);
	dictionary_put(objetivos, "Pidgey", 2);

	dictionary_put(capturados, "Pikachu", 1);
	dictionary_put(capturados, "Pidgey", 1);
	dictionary_put(capturados, "Charizard", 1);

	tcb->tid = 1;
	tcb->estado_tcb = DEADLOCK;
	tcb->posicion = posicion;
	tcb->objetivos = objetivos;
	tcb->pokemones_capturados = capturados;

	list_add(ready, tcb);
	list_add(deadlock_actual, tcb);

	ejecutar_acciones_post_intercambio(tcb, false);

	CU_ASSERT_EQUAL_FATAL(list_size(ready), 0);
	CU_ASSERT_EQUAL_FATAL(list_size(deadlock_actual), 0);
	CU_ASSERT_EQUAL_FATAL(list_get(ready_to_exchange, 0), tcb);
	CU_ASSERT_EQUAL_FATAL(tcb->estado_tcb, READY_TO_EXCHANGE);

	list_clean(ready_to_exchange);
	list_clean(deadlock_actual);
	list_clean(ready);
	dictionary_clean(capturados);
	dictionary_destroy(capturados);
	dictionary_clean(objetivos);
	dictionary_destroy(objetivos);
	free(posicion);
	free(tcb);
}

void test_ejecutar_acciones_post_intercambio_para_tcb_con_objetivo_cumplido() {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	t_dictionary* objetivos = dictionary_create();
	t_dictionary* capturados = dictionary_create();
	t_posicion* posicion = malloc(sizeof(t_posicion));
	ready = list_create();
	deadlock_actual = list_create();
	l_exit = list_create();

	dictionary_put(objetivos, "Pikachu", 1);
	dictionary_put(objetivos, "Pidgey", 2);

	dictionary_put(capturados, "Pikachu", 1);
	dictionary_put(capturados, "Pidgey", 2);

	posicion->x = 1;
	posicion->y = 1;

	tcb->estado_tcb = DEADLOCK;
	tcb->posicion = posicion;
	tcb->objetivos = objetivos;
	tcb->pokemones_capturados = capturados;

	list_add(ready, tcb);
	list_add(deadlock_actual, tcb);

	ejecutar_acciones_post_intercambio(tcb, false);

	CU_ASSERT_EQUAL_FATAL(list_size(ready), 0);
	CU_ASSERT_EQUAL_FATAL(list_size(deadlock_actual), 0);
	CU_ASSERT_EQUAL_FATAL(list_get(l_exit, 0), tcb);
	CU_ASSERT_EQUAL_FATAL(tcb->estado_tcb, EXIT);

	list_clean(l_exit);
	list_clean(deadlock_actual);
	list_clean(ready);
	dictionary_clean(capturados);
	dictionary_destroy(capturados);
	dictionary_clean(objetivos);
	dictionary_destroy(objetivos);
	free(posicion);
	free(tcb);
}

void test_despachar_deadlock_carga_la_rafaga_del_tcb_y_lo_envia_a_ready() {
	t_tcb_entrenador* tcb_actual = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_a_intercambiar = malloc(sizeof(t_tcb_entrenador));

	t_posicion* tcb_actual_posicion = malloc(sizeof(t_posicion));
	t_dictionary* tcb_actual_objetivos = dictionary_create();
	t_dictionary* tcb_actual_pokemones_capturados = dictionary_create();

	t_posicion* tcb_a_intercambiar_posicion = malloc(sizeof(t_posicion));
	t_dictionary* tcb_a_intercambiar_objetivos = dictionary_create();
	t_dictionary* tcb_a_intercambiar_pokemones_capturados = dictionary_create();

	t_list* lista_deadlock = list_create();

	/*
	 * Tcb actual:
	 * - Pokemones necesarios: "Spearow"
	 * - Pokemones no necesarios: "Abra", "Muk"
	*/

	dictionary_put(tcb_actual_objetivos, "Pidgey", 1);
	dictionary_put(tcb_actual_objetivos, "Spearow", 2);

	dictionary_put(tcb_actual_pokemones_capturados, "Muk", 1);
	dictionary_put(tcb_actual_pokemones_capturados, "Abra", 1);
	dictionary_put(tcb_actual_pokemones_capturados, "Pidgey", 1);

	tcb_actual_posicion->x = 0;
	tcb_actual_posicion->y = 0;

	tcb_actual->rafaga = queue_create();
	tcb_actual->posicion = tcb_actual_posicion;
	tcb_actual->estado_tcb = DEADLOCK;
	tcb_actual->objetivos = tcb_actual_objetivos;
	tcb_actual->pokemones_capturados = tcb_actual_pokemones_capturados;

	/*
	 * Tcb a intercambiar:
	 * - Pokemones necesarios: "Lapras"
	 * - Pokemones no necesarios: "Spearow"
	*/

	dictionary_put(tcb_a_intercambiar_objetivos, "Charmander", 1);
	dictionary_put(tcb_a_intercambiar_objetivos, "Lapras", 2);

	dictionary_put(tcb_a_intercambiar_pokemones_capturados, "Spearow", 1);
	dictionary_put(tcb_a_intercambiar_pokemones_capturados, "Charmander", 1);
	dictionary_put(tcb_a_intercambiar_pokemones_capturados, "Lapras", 1);

	tcb_a_intercambiar_posicion->x = 1;
	tcb_a_intercambiar_posicion->y = 1;

	tcb_a_intercambiar->rafaga = queue_create();
	tcb_a_intercambiar->posicion = tcb_a_intercambiar_posicion;
	tcb_a_intercambiar->estado_tcb = DEADLOCK;
	tcb_a_intercambiar->objetivos = tcb_a_intercambiar_objetivos;
	tcb_a_intercambiar->pokemones_capturados = tcb_a_intercambiar_pokemones_capturados;

	list_add(lista_deadlock, tcb_actual);
	list_add(lista_deadlock, tcb_a_intercambiar);

	t_deadlock* deadlock = crear_deadlock(lista_deadlock);
	ready = list_create();

	despachar_resolucion_de_deadlock(deadlock);

	CU_ASSERT_EQUAL(queue_size(deadlock->tcb_1->rafaga), 3);
	test_leer_instrucciones(deadlock->tcb_1, 2, MOVERSE_A_ENTRENADOR);
	CU_ASSERT_EQUAL(queue_peek(deadlock->tcb_1->rafaga), INTERCAMBIAR);

	CU_ASSERT_EQUAL_FATAL(deadlock->tcb_1, list_first(ready));

	list_clean(ready);
	list_clean(lista_deadlock);
	list_destroy(lista_deadlock);
	queue_clean(deadlock->tcb_1->rafaga);
	queue_destroy(deadlock->tcb_1->rafaga);
	queue_clean(deadlock->tcb_2->rafaga);
	queue_destroy(deadlock->tcb_2->rafaga);
	dictionary_clean(deadlock->tcb_1->pokemones_capturados);
	dictionary_destroy(deadlock->tcb_1->pokemones_capturados);
	dictionary_clean(deadlock->tcb_1->objetivos);
	dictionary_destroy(deadlock->tcb_1->objetivos);
	dictionary_clean(deadlock->tcb_2->pokemones_capturados);
	dictionary_destroy(deadlock->tcb_2->pokemones_capturados);
	dictionary_clean(deadlock->tcb_2->objetivos);
	dictionary_destroy(deadlock->tcb_2->objetivos);
	free(deadlock->tcb_1->posicion);
	free(deadlock->tcb_2->posicion);
	free(deadlock->tcb_1);
	free(deadlock->tcb_2);
	free(deadlock);
}

void test_detectar_dealock_recursivo() {
	t_tcb_entrenador* tcb_1 = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_2 = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_3 = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* tcb_4 = malloc(sizeof(t_tcb_entrenador));

	t_dictionary* tcb_1_objetivos = dictionary_create();
	t_dictionary* tcb_1_capturados = dictionary_create();

	t_dictionary* tcb_2_objetivos = dictionary_create();
	t_dictionary* tcb_2_capturados = dictionary_create();

	t_dictionary* tcb_3_objetivos = dictionary_create();
	t_dictionary* tcb_3_capturados = dictionary_create();

	t_dictionary* tcb_4_objetivos = dictionary_create();
	t_dictionary* tcb_4_capturados = dictionary_create();

	t_deadlock* deadlock = NULL;

	/*
	 * Primer tcb:
	 * - Pokemones necesarios: "2 Pikachu"
	 * - Pokemones no necesarios: "Charmander"
	*/

	dictionary_put(tcb_1_objetivos, "Pikachu", 2);

	dictionary_put(tcb_1_capturados, "Pikachu", 1);
	dictionary_put(tcb_1_capturados, "Charmander", 1);

	tcb_1->tid = 1;
	tcb_1->estado_tcb = READY_TO_EXCHANGE;
	tcb_1->objetivos = tcb_1_objetivos;
	tcb_1->pokemones_capturados = tcb_1_capturados;

	/*
	 * Segundo tcb:
	 * - Pokemones necesarios: "2 Charmander"
	 * - Pokemones no necesarios: "Squirtle"
	*/

	dictionary_put(tcb_2_objetivos, "Charmander", 2);

	dictionary_put(tcb_2_capturados, "Charmander", 1);
	dictionary_put(tcb_2_capturados, "Squirtle", 1);

	tcb_2->tid = 2;
	tcb_2->estado_tcb = READY_TO_EXCHANGE;
	tcb_2->objetivos = tcb_2_objetivos;
	tcb_2->pokemones_capturados = tcb_2_capturados;


	/*
	 * Tercer tcb:
	 * - Pokemones necesarios: "2 Squirtle"
	 * - Pokemones no necesarios: "Pikachu"
	*/

	dictionary_put(tcb_3_objetivos, "Squirtle", 2);

	dictionary_put(tcb_3_capturados, "Squirtle", 1);
	dictionary_put(tcb_3_capturados, "Pikachu", 1);

	tcb_3->tid = 3;
	tcb_3->estado_tcb = READY_TO_EXCHANGE;
	tcb_3->objetivos = tcb_3_objetivos;
	tcb_3->pokemones_capturados = tcb_3_capturados;

	/*
	 * Cuarto tcb:
	 * - Pokemones necesarios: "2 Mew"
	 * - Pokemones no necesarios: "Pikachu"
	*/

	dictionary_put(tcb_4_objetivos, "Mew", 2);

	dictionary_put(tcb_4_capturados, "Mew", 1);
	dictionary_put(tcb_4_capturados, "Pikachu", 1);

	tcb_4->tid = 4;
	tcb_4->estado_tcb = READY_TO_EXCHANGE;
	tcb_4->objetivos = tcb_4_objetivos;
	tcb_4->pokemones_capturados = tcb_4_capturados;


	list_add(ready_to_exchange, tcb_1);
	list_add(ready_to_exchange, tcb_2);
	list_add(ready_to_exchange, tcb_3);
	list_add(ready_to_exchange, tcb_4);


	deadlock = detectar_deadlock_recursivo(tcb_1);


	CU_ASSERT_TRUE_FATAL(deadlock != NULL);
	CU_ASSERT_EQUAL_FATAL(list_size(deadlock), 3);
	CU_ASSERT_EQUAL_FATAL(list_get(deadlock, 0), tcb_1);
	CU_ASSERT_EQUAL_FATAL(list_get(deadlock, 1), tcb_2);
	CU_ASSERT_EQUAL_FATAL(list_get(deadlock, 2), tcb_3);
	CU_ASSERT_EQUAL_FATAL(tcb_1->nivel_de_grafo_en_deadlock, 0);
	CU_ASSERT_EQUAL_FATAL(tcb_2->nivel_de_grafo_en_deadlock, 1);
	CU_ASSERT_EQUAL_FATAL(tcb_3->nivel_de_grafo_en_deadlock, 2);

	list_clean(deadlock);
	list_destroy(deadlock);

	dictionary_clean(tcb_4_capturados);
	dictionary_destroy(tcb_4_capturados);
	dictionary_clean(tcb_4_objetivos);
	dictionary_destroy(tcb_4_objetivos);

	dictionary_clean(tcb_3_capturados);
	dictionary_destroy(tcb_3_capturados);
	dictionary_clean(tcb_3_objetivos);
	dictionary_destroy(tcb_3_objetivos);

	dictionary_clean(tcb_2_capturados);
	dictionary_destroy(tcb_2_capturados);
	dictionary_clean(tcb_2_objetivos);
	dictionary_destroy(tcb_2_objetivos);

	dictionary_clean(tcb_1_capturados);
	dictionary_destroy(tcb_1_capturados);
	dictionary_clean(tcb_1_objetivos);
	dictionary_destroy(tcb_1_objetivos);

	free(tcb_4);
	free(tcb_3);
	free(tcb_2);
	free(tcb_1);
}

