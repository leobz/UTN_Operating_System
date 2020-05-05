#include "main.h"
#include "test/testing.h"

t_list* ready;
t_list* new;
t_list* blocked;

int main(int argc, char ** argv) {
	inicializar_listas();

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0){
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
