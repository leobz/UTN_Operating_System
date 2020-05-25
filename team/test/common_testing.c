#include "common_testing.h"

t_tcb_entrenador* tcb_generico(int tid) {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	tcb->estado_tcb = NEW;
	tcb->tid = tid;
	tcb->rafaga = queue_create();

	return tcb;
}

t_posicion* posicion_generica() {
	t_posicion* posicion;
	posicion->x = 1;
	posicion->y = 1;
	return posicion;
}

t_posicion* posicion(int x, int y) {
	t_posicion* posicion;
	posicion->x = x;
	posicion->y = y;
	return posicion;
}
