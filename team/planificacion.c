#include "planificacion.h"

#include "test/testing.h"

t_list* ready;
t_list* new;
t_list* blocked;

// TODO: ALGORITMO_PLANIFICACION se debe cargar por configuracion, pero como no tengo
// esa parte lo hardcodeo. Borrar esto al obtener configuracion
ALGORITMO_PLANIFICACION = FIFO;


void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
}

void pasar_a_ready(t_tcb_entrenador* tcb) {
	// TODO: implementar ordenamiento por cercania
	list_add(ready, tcb);
	tcb->estado_tcb = READY;
}

t_tcb_entrenador* siguiente_tcb_a_ejecutar() {
	t_tcb_entrenador* siguiente_tcb;
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

void cargar_instruccion(t_tcb_entrenador* tcb, int instruccion) {
	queue_push(tcb->rafaga, instruccion);
}

void cargar_rafaga_captura(t_tcb_entrenador* tcb, t_posicion* posicion_pokemon) {
	cargar_rafaga_movimiento(tcb, posicion_pokemon);
	cargar_instruccion(tcb, CATCH);

}

void cargar_rafaga_intercambio(t_tcb_entrenador* tcb) {
	// TODO
}

void cargar_rafaga_movimiento(t_tcb_entrenador* tcb, t_posicion* posicion_pokemon) {
	int movimientos = 0;

	while (movimientos < distancia_entre(tcb->posicion, posicion_pokemon)) {
		cargar_instruccion(tcb, MOVERSE);
		movimientos++;
	}
}

int distancia_entre(t_posicion* inicio, t_posicion* destino) {
	int delta_x = destino->x - inicio->x;
	int delta_y = destino->y - inicio->y;

	return fabs(delta_x + delta_y);
}

void cargar_tcb_captura(t_tcb_entrenador* tcb, t_pokemon* pokemon) {
	cargar_rafaga_captura(tcb, pokemon->posicion );
	tcb->pokemon_a_capturar = pokemon;
}
