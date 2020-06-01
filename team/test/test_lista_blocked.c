#include "test_lista_blocked.h"

void agregar_tests_lista_blocked() {
	CU_pSuite suite_configuracion = CU_add_suite(
			"TCB pasa a lista blocked", NULL,
			NULL);

	CU_add_test(suite_configuracion,
			"La lista blocked no queda vacia al pasar un entrenador a la lista blocked",
			lista_blocked_no_vacia_al_pasar_entrenador_a_blocked);

	CU_add_test(suite_configuracion,
			"El entrenador cambia de estado a BLOCKED al pasar a la lista blocked",
			cambio_estado_al_pasar_entrenador_a_blocked);

	CU_add_test(suite_configuracion,
			"El entrenador es el mismo que se pasa a la lista blocked",
			mismo_entrenador_al_pasar_entrenador_a_blocked);
}

void lista_blocked_no_vacia_al_pasar_entrenador_a_blocked() {
	t_tcb_entrenador* tcb = tcb_generico(1);
	list_clean(blocked);

	pasar_a_blocked(tcb);

	CU_ASSERT_EQUAL_FATAL(list_size(blocked), 1);
	free(tcb);
}

void cambio_estado_al_pasar_entrenador_a_blocked() {
	t_tcb_entrenador* tcb = tcb_generico(1);
	list_clean(blocked);

	pasar_a_blocked(tcb);

	CU_ASSERT_EQUAL_FATAL(tcb->estado_tcb, BLOCKED);
	free(tcb);
}

void mismo_entrenador_al_pasar_entrenador_a_blocked() {
	t_tcb_entrenador* tcb = tcb_generico(1);
	list_clean(blocked);

	pasar_a_blocked(tcb);

	CU_ASSERT_EQUAL_FATAL(list_get(blocked, 0), tcb);
	free(tcb);
}

