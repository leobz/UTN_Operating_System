#include "test_algoritmos_planificacion.h"

void agregar_tests_algoritmos_planificacion() {
	CU_pSuite suite_configuracion = CU_add_suite("Algoritmos de planificacion", NULL, NULL);

	CU_add_test(suite_configuracion,
			"El primer tcb posee una proxima estimacion menor al segundo tcb",
			test_primer_tcb_posee_menor_proxima_estimacion_segundo_tcb);

	CU_add_test(suite_configuracion,
			"El primer tcb no posee una proxima estimacion menor al segundo tcb",
			test_primer_tcb_no_posee_menor_proxima_estimacion_segundo_tcb);

	CU_add_test(suite_configuracion,
			"Elegir el tcb con menor proxima estimacion asociado a la lista ready",
			test_obtener_tcb_menor_proxima_estimacion);

}

void finalizar_test() {
	void destruir_tcbs(t_tcb_entrenador* tcb) {
		free(tcb);
	}

	list_destroy_and_destroy_elements(ready, (void*) destruir_tcbs);
}

void test_primer_tcb_posee_menor_proxima_estimacion_segundo_tcb() {
	team_config->alpha = 0.3;

	t_tcb_entrenador* primer_tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* segundo_tcb = malloc(sizeof(t_tcb_entrenador));
	bool menor_estimacion_proxima;

	primer_tcb->rafaga_anterior = 2;
	primer_tcb->estimacion_anterior = 3.1;

	segundo_tcb->rafaga_anterior = 4;
	segundo_tcb->estimacion_anterior = 2.7;

	menor_estimacion_proxima = tiene_menor_proxima_estimacion(primer_tcb, segundo_tcb);

	CU_ASSERT_TRUE_FATAL(menor_estimacion_proxima);

	free(primer_tcb);
	free(segundo_tcb);
}

void test_primer_tcb_no_posee_menor_proxima_estimacion_segundo_tcb() {
	team_config->alpha = 0.6;

	t_tcb_entrenador* primer_tcb = malloc(sizeof(t_tcb_entrenador));
	t_tcb_entrenador* segundo_tcb = malloc(sizeof(t_tcb_entrenador));
	bool menor_estimacion_proxima;

	primer_tcb->rafaga_anterior = 2;
	primer_tcb->estimacion_anterior = 3.5;

	segundo_tcb->rafaga_anterior = 3;
	segundo_tcb->estimacion_anterior = 1;

	menor_estimacion_proxima = tiene_menor_proxima_estimacion(primer_tcb, segundo_tcb);

	CU_ASSERT_FALSE_FATAL(menor_estimacion_proxima);

	free(primer_tcb);
	free(segundo_tcb);
}

void test_obtener_tcb_menor_proxima_estimacion() {
	team_config->alpha = 0.8;

	t_tcb_entrenador* primer_tcb = tcb_generico(NULL);
	t_tcb_entrenador* segundo_tcb = tcb_generico(NULL);
	t_tcb_entrenador* tercer_tcb = tcb_generico(NULL);

	t_tcb_entrenador* tcb_menor_proxima_estimacion = NULL;

	primer_tcb->rafaga_anterior = 6;
	primer_tcb->estimacion_anterior = 3.5;

	segundo_tcb->rafaga_anterior = 2;
	segundo_tcb->estimacion_anterior = 2.3;

	tercer_tcb->rafaga_anterior = 1;
	tercer_tcb->estimacion_anterior = 4.4;

	list_clean(ready);
	list_add(ready, primer_tcb);
	list_add(ready, segundo_tcb);
	list_add(ready, tercer_tcb);

	tcb_menor_proxima_estimacion = obtener_tcb_menor_proxima_estimacion();

	CU_ASSERT_TRUE_FATAL(tcb_menor_proxima_estimacion != NULL);
	CU_ASSERT_EQUAL_FATAL(tcb_menor_proxima_estimacion, tercer_tcb);
	CU_ASSERT_EQUAL_FATAL(tcb_menor_proxima_estimacion->rafaga_anterior, queue_size(tcb_menor_proxima_estimacion->rafaga));
	CU_ASSERT_EQUAL_FATAL(tcb_menor_proxima_estimacion->estimacion_anterior, 1.68);

	finalizar_test();
}
