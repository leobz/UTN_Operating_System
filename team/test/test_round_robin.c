#include "test_round_robin.h"

void agregar_tests_round_robin() {
	CU_pSuite suite_configuracion = CU_add_suite("Round Robin", NULL,
	NULL);

	CU_add_test(suite_configuracion,
			"Quantum corta la ejecucion y envia TCB a ready",
			quantum_quita_tcb_de_ejecucion);


}

t_pokemon* crear_pokemon_generico() {
	t_pokemon* pokemon = malloc(sizeof(t_pokemon));
	pokemon->pokemon = strdup("Pikachu");
	pokemon->posicion = malloc(sizeof(t_posicion));
	pokemon->posicion->x = 0;
	pokemon->posicion->y = 0;
	return pokemon;
}

void quantum_quita_tcb_de_ejecucion() {
	team_config->algoritmo_de_planificacion = strdup("RR");
	team_config->retardo_ciclo_cpu = 1;
	list_clean(ready);
	iniciar_planificador();

	t_tcb_entrenador* entrenador = list_get(new, 0);
	t_pokemon* pokemon = crear_pokemon_generico();

	cargar_tcb_captura(entrenador, pokemon);
	int tamanio_rafaga_antes_de_ejecutar = queue_size(entrenador->rafaga);

	CU_ASSERT_EQUAL(list_size(ready), 0);

	pasar_a_ready(entrenador, "");
	sleep(1);

	CU_ASSERT_EQUAL(entrenador->estado_tcb, READY);
	CU_ASSERT_EQUAL(queue_size(entrenador->rafaga), tamanio_rafaga_antes_de_ejecutar - team_config->quantum);
	CU_ASSERT_EQUAL(list_size(ready), 1);
}
