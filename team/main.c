#include "main.h"
#include "test/testing.h"

t_list* ready;
t_list* new;
t_list* blocked;

// TODO: ALGORITMO_PLANIFICACION se debe cargar por configuracion, pero como no tengo
// esa parte lo hardcodeo. Borrar esto al obtener configuracion
ALGORITMO_PLANIFICACION = FIFO;

int main(int argc, char ** argv) {
	inicializar_listas();

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0) {
			correrTests();
		}
	}

	return 0;
}

void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
}

void pasar_a_ready(tcb_entrenador* tcb) {
	// TODO: implementar ordenamiento por cercania
	list_add(ready, tcb);
	tcb->estado = READY;
}

tcb_entrenador* siguiente_tcb_a_ejecutar() {
	tcb_entrenador* siguiente_tcb;
	switch (ALGORITMO_PLANIFICACION) {
	case FIFO:
		siguiente_tcb = list_pop_first(ready);
		break;
	case RR:
		// TODO
		break;
	case SJF:
		// TODO
		break;
	}
	return siguiente_tcb;
}

void cargar_rafaga(tcb_entrenador* tcb) {
	switch (tcb->estado) {
	case READY:
		cargar_rafaga_captura(tcb);
		break;
	case READY_TO_EXCHANGE:
		cargar_rafaga_intercambio(tcb);
		break;
	default:
		printf("ERROR: No se pudo cargar la rafaga para el TID: %d\n", tcb->tid);
		printf("ERROR: El estado %d no es soportado", tcb->estado);
		exit(-1);
	}
}

void cargar_rafaga_captura(tcb_entrenador* tcb) {
	// TODO
}

void cargar_rafaga_intercambio(tcb_entrenador* tcb) {
	// TODO
}
