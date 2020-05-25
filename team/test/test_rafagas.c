#include "test_rafagas.h"

void agregar_tests_rafagas() {
	CU_pSuite suite_configuracion = CU_add_suite("Rafagas", NULL,
	NULL);

	CU_add_test(suite_configuracion, "Cargar rafaga movimiento 1er Cuadrante",
			test_cargar_rafaga_movimiento_cuadrante_1);

	CU_add_test(suite_configuracion, "Cargar rafaga movimiento 3er a 1er Cuadrante",
			test_cargar_rafaga_movimiento_cuadrante_3_1);

	CU_add_test(suite_configuracion, "Cargar rafaga movimiento 1er a 3er Cuadrante",
			test_cargar_rafaga_movimiento_cuadrante_1_3);

	CU_add_test(suite_configuracion, "Cargar rafaga captura",
			test_cargar_rafaga_captura);

}

void testear_rafaga_movimiento(t_posicion* inicio, t_posicion* destino,
		int movimientos_esperados) {
	t_tcb_entrenador* tcb = tcb_generico(NULL);
	tcb->posicion = inicio;

	cargar_rafaga_movimiento(tcb, destino);

	CU_ASSERT_EQUAL(queue_size(tcb->rafaga), movimientos_esperados);

	test_leer_instrucciones(tcb, movimientos_esperados, MOVERSE);
}

void test_leer_instrucciones(t_tcb_entrenador* tcb, int cantidad, int instruccion) {
	for (int i = 0; i < cantidad; i++) {
		CU_ASSERT_EQUAL(queue_peek(tcb->rafaga), instruccion);
		queue_pop(tcb->rafaga);
	}
}


void test_cargar_rafaga_movimiento_cuadrante_1() {
	t_posicion* inicio = malloc(sizeof(t_posicion));
	t_posicion* destino= malloc(sizeof(t_posicion));

	inicio->x = 0;
	inicio->y = 0;

	destino->x = 1;
	destino->y = 1;

	testear_rafaga_movimiento(inicio, destino, 2);
}

void test_cargar_rafaga_movimiento_cuadrante_3_1() {
	t_posicion* inicio = malloc(sizeof(t_posicion));
	t_posicion* destino= malloc(sizeof(t_posicion));

	inicio->x = -1;
	inicio->y = -1;

	destino->x = 1;
	destino->y = 1;

	testear_rafaga_movimiento(inicio, destino, 4);

}

void test_cargar_rafaga_movimiento_cuadrante_1_3() {
	t_posicion* inicio = malloc(sizeof(t_posicion));
	t_posicion* destino= malloc(sizeof(t_posicion));

	inicio->x = 1;
	inicio->y = 1;

	destino->x = -1;
	destino->y = -1;

	testear_rafaga_movimiento(inicio, destino, 4);

}



void test_cargar_rafaga_captura() {
	t_posicion* inicio = malloc(sizeof(t_posicion));
	t_posicion* destino= malloc(sizeof(t_posicion));

	inicio->x = 0;
	inicio->y = 0;

	destino->x = 1;
	destino->y = 1;

	t_tcb_entrenador* tcb = tcb_generico(NULL);
	tcb->posicion = inicio;

	cargar_rafaga_captura(tcb, destino);

	CU_ASSERT_EQUAL(queue_size(tcb->rafaga), 3);

	test_leer_instrucciones(tcb, 2, MOVERSE);
	CU_ASSERT_EQUAL(queue_peek(tcb->rafaga), CATCH);

}
