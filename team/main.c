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

	// Cuando aparece
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

void cargar_rafaga(tcb_entrenador* tcb, t_posicion posicion_pokemon) {
	// TODO: Necesito pasarle por parametro a esta funcion el mapa de pokemons disponibles
	// Cuando se tenga, se agrega como parametro de cargar_rafaga
	switch (tcb->estado) {
	case READY:
		cargar_rafaga_captura(tcb, posicion_pokemon);
		break;
	case READY_TO_EXCHANGE:
		cargar_rafaga_intercambio(tcb);
		break;
	default:
		printf("ERROR: No se pudo cargar la rafaga para el TID: %d\n",
				tcb->tid);
		printf("ERROR: El estado %d no es soportado", tcb->estado);
		exit(-1);
	}
}

void cargar_instruccion(tcb_entrenador* tcb, int instruccion){
	queue_push(tcb->rafaga, instruccion);
}

void cargar_rafaga_captura(tcb_entrenador* tcb, t_posicion posicion_pokemon) {
	cargar_rafaga_movimiento(tcb, posicion_pokemon);
	cargar_instruccion(tcb, CATCH);

}

void cargar_rafaga_intercambio(tcb_entrenador* tcb) {
	// TODO
}

void cargar_rafaga_movimiento(tcb_entrenador* tcb, t_posicion posicion_pokemon) {
	int movimientos = 0;

	while (movimientos < distancia_entre(tcb->posicion, posicion_pokemon)) {
		cargar_instruccion(tcb, MOVERSE);
		movimientos++;
	}
}

int distancia_entre(t_posicion inicio, t_posicion destino){
	int delta_x = destino.x - inicio.x;
	int delta_y = destino.y - inicio.y;

	return fabs(delta_x + delta_y);
}
