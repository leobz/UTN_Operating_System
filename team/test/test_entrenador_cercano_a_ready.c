#include "test_entrenador_cercano_a_ready.h"

void agregar_tests_entrenador_cercano_a_ready() {
	CU_pSuite suite_configuracion = CU_add_suite("TCBs en Ready", NULL, NULL);

	CU_add_test(suite_configuracion,
			"Al encontrar el entrenador mas cercano al pokemon: TCB cambia su estado_tcb a READY",
			entrenador_cercano_cambia_estado_a_ready);
}



void entrenador_cercano_cambia_estado_a_ready() {
	t_mensaje_appeared* mensaje = create_mensaje_appeared("Pikachu", 5, 6);
	t_tcb_entrenador* entrenador = NULL;
	list_clean(ready);

	pasar_entrenador_a_ready_segun_cercania(mensaje);

	CU_ASSERT_EQUAL(list_size(ready), 1);

	entrenador = list_get(ready, 0);

	CU_ASSERT_TRUE(entrenador != NULL);
	CU_ASSERT_EQUAL(entrenador->estado_tcb, READY);
	// Si se cambia las posiciones de los entrenadores en el team.config, estos resultados van a fallar.
	CU_ASSERT_EQUAL(entrenador->posicion->x, 5);
	CU_ASSERT_EQUAL(entrenador->posicion->y, 5);

	// Vuelvo a poner entrenador en new (Para que no rompan otros test)
	list_add(new, entrenador);
	free(mensaje);
}

