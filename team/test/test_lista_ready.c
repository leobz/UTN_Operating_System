#include "test_lista_ready.h"

t_list* ready;

void agregar_tests_lista_ready() {
	CU_pSuite suite_configuracion = CU_add_suite("TCBs en Ready", NULL,
	NULL);

	CU_add_test(suite_configuracion,
			"Al pasar TCB a Ready: TCB cambia su estado_tcb a READY",
			pasar_a_ready_cambia_estado_tcb_a_ready);

	CU_add_test(suite_configuracion,
			"Al pasar TCB a Ready: TCB se agrega a lista Ready",
			pasar_a_ready_agrega_tcb_a_ready);

	CU_add_test(suite_configuracion,
			"Al planificar el siguiente entrenador a ejecutar: TCB se extrae de Ready",
			siguiente_entrenador_a_ejecutar_extrae_tcb_de_ready);

}

void siguiente_entrenador_a_ejecutar_extrae_tcb_de_ready() {
	t_tcb_entrenador* tcb_1 = tcb_generico(1);

	pasar_a_ready(tcb_1);
	CU_ASSERT_PTR_EQUAL(list_get(ready, 0), tcb_1);

	t_tcb_entrenador* tcb_sig = siguiente_tcb_a_ejecutar();

	CU_ASSERT_PTR_NOT_EQUAL(list_get(ready, 0), tcb_1);
	CU_ASSERT_PTR_EQUAL(tcb_sig, tcb_1)
	//TODO: liberar memoria
}

void pasar_a_ready_cambia_estado_tcb_a_ready() {
	t_tcb_entrenador* tcb = tcb_generico(NULL);
	CU_ASSERT_EQUAL(tcb->estado_tcb, NEW);

	pasar_a_ready(tcb);
	CU_ASSERT_EQUAL(tcb->estado_tcb, READY);
	//TODO: liberar memoria
}

void pasar_a_ready_agrega_tcb_a_ready() {
	t_tcb_entrenador* tcb = tcb_generico(NULL);

	pasar_a_ready(tcb);
	t_tcb_entrenador* tcb_en_ready = list_get(ready, 0);

	CU_ASSERT_PTR_EQUAL(tcb_en_ready, tcb);
	//TODO: liberar memoria
}

