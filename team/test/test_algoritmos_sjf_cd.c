#include "test_algoritmos_sjf.h"

void agregar_tests_algoritmos_sjf_cd() {
	CU_pSuite suite_configuracion = CU_add_suite("Algoritmos de planificacion SJF SD", NULL, NULL);

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


