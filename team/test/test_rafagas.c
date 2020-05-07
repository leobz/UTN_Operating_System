#include "test_rafagas.h"

void agregar_tests_rafagas() {
	CU_pSuite suite_configuracion = CU_add_suite("Rafagas", NULL,
	NULL);

	CU_add_test(suite_configuracion, "Cargar rafaga movimiento",
			test_cargar_rafaga_movimiento);

}

void test_cargar_rafaga_movimiento() {
	tcb_entrenador* tcb = tcb_generico(NULL);
	tcb->posicion.x = 0;
	tcb->posicion.y = 0;

	t_posicion posicion_pikachu = posicion(2, 2);

	cargar_rafaga_movimiento(tcb, posicion_pikachu);

	CU_ASSERT_EQUAL(queue_size(tcb->rafaga), 4);

	for (int i = 0; i < 4; i++) {
		CU_ASSERT_EQUAL(queue_peek(tcb->rafaga), MOVERSE);
		queue_pop(tcb->rafaga);
	}

}
