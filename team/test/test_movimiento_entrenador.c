#include "test_movimiento_entrenador.h"

void agregar_tests_movimiento_entrenador() {
	CU_pSuite suite_configuracion = CU_add_suite(
			"TCB cercano se mueve una posicion hacia pokemon a capturar", NULL,
			NULL);

	CU_add_test(suite_configuracion,
			"El entrenador cercano y pokemon a capturar en misma posicion: no hay movimiento",
			entrenador_y_pokemon_a_capturar_misma_posicion);

	CU_add_test(suite_configuracion,
			"El entrenador cercano se mueve una posicion en X a la derecha acercandose a la posicion del pokemon a capturar",
			entrenador_se_mueve_a_derecha_hacia_pokemon_a_capturar_en_x);

	CU_add_test(suite_configuracion,
			"El entrenador cercano se mueve una posicion en X a la izquierda acercandose a la posicion del pokemon a capturar",
			entrenador_se_mueve_a_izquierda_hacia_pokemon_a_capturar_en_x);

	CU_add_test(suite_configuracion,
			"El entrenador cercano se mueve una posicion en Y arriba acercandose a la posicion del pokemon a capturar",
			entrenador_se_mueve_arriba_hacia_pokemon_a_capturar_en_y);

	CU_add_test(suite_configuracion,
			"El entrenador cercano se mueve una posicion en Y abajo acercandose a la posicion del pokemon a capturar",
			entrenador_se_mueve_abajo_hacia_pokemon_a_capturar_en_y);
}

void entrenador_y_pokemon_a_capturar_misma_posicion() {
	t_tcb_entrenador* entrenador = create_tcb_entrenador(5, 5, 5, 5);

	actualizar_posicion(entrenador);

	CU_ASSERT_EQUAL(entrenador->posicion->x,
			entrenador->pokemon_a_capturar->posicion->x);
	CU_ASSERT_EQUAL(entrenador->posicion->y,
			entrenador->pokemon_a_capturar->posicion->y);

	destroy_tcb_entrenador(entrenador);
}

void entrenador_se_mueve_a_derecha_hacia_pokemon_a_capturar_en_x() {
	t_tcb_entrenador* entrenador = create_tcb_entrenador(5, 5, 7, 5);
	int entrenador_pos_x = entrenador->posicion->x;

	actualizar_posicion(entrenador);

	CU_ASSERT_EQUAL(entrenador->posicion->x, entrenador_pos_x + 1);

	destroy_tcb_entrenador(entrenador);
}

void entrenador_se_mueve_a_izquierda_hacia_pokemon_a_capturar_en_x() {
	t_tcb_entrenador* entrenador = create_tcb_entrenador(7, 5, 5, 5);
	int entrenador_pos_x = entrenador->posicion->x;

	actualizar_posicion(entrenador);

	CU_ASSERT_EQUAL(entrenador->posicion->x, entrenador_pos_x - 1);

	destroy_tcb_entrenador(entrenador);
}

void entrenador_se_mueve_arriba_hacia_pokemon_a_capturar_en_y() {
	t_tcb_entrenador* entrenador = create_tcb_entrenador(5, 5, 5, 7);
	int entrenador_pos_y = entrenador->posicion->y;

	actualizar_posicion(entrenador);

	CU_ASSERT_EQUAL(entrenador->posicion->y, entrenador_pos_y + 1);

	destroy_tcb_entrenador(entrenador);
}

void entrenador_se_mueve_abajo_hacia_pokemon_a_capturar_en_y() {
	t_tcb_entrenador* entrenador = create_tcb_entrenador(5, 7, 5, 5);
	int entrenador_pos_y = entrenador->posicion->y;

	actualizar_posicion(entrenador);

	CU_ASSERT_EQUAL(entrenador->posicion->y, entrenador_pos_y - 1);

	destroy_tcb_entrenador(entrenador);
}
