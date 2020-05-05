#include "test_lista_ready.h"

t_list* ready;

void agregar_tests_lista_ready() {
	CU_pSuite suite_configuracion = CU_add_suite("Cargar TCB a Ready", NULL,
	NULL);

	CU_add_test(suite_configuracion,
			"Pasar TCB a Ready cambia su estado a READY",
			pasar_a_ready_cambia_estado_a_ready);

	CU_add_test(suite_configuracion, "Pasar TCB a Ready lo agrega a READY",
			pasar_a_ready_agrega_tcb_a_ready);
}

void pasar_a_ready_cambia_estado_a_ready() {
	inicializar_listas();

	tcb_entrenador* tcb = tcb_generico();
	CU_ASSERT_EQUAL(tcb->estado, NEW);

	pasar_a_ready(tcb);
	CU_ASSERT_EQUAL(tcb->estado, READY);
	//TODO: liberar memoria
}

void pasar_a_ready_agrega_tcb_a_ready() {
	inicializar_listas();

	tcb_entrenador* tcb = tcb_generico();

	pasar_a_ready(tcb);
	tcb_entrenador* tcb_en_ready = list_get(ready, 0);

	CU_ASSERT_PTR_EQUAL(tcb_en_ready, tcb);
	//TODO: liberar memoria
}

tcb_entrenador* tcb_generico() {
	tcb_entrenador* tcb = malloc(sizeof(tcb_entrenador));
	tcb->estado = NEW;

	return tcb;
}
