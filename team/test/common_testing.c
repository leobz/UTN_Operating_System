#include "common_testing.h"

tcb_entrenador* tcb_generico(int tid) {
	tcb_entrenador* tcb = malloc(sizeof(tcb_entrenador));
	tcb->estado = NEW;
	tcb->tid = tid;

	return tcb;
}
