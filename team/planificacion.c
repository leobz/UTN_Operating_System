#include "planificacion.h"

#include "test/testing.h"

t_list* ready;
t_list* new;
t_list* blocked;

// TODO: ALGORITMO_PLANIFICACION y SLEEP_TIME se debe cargar por configuracion, pero como no tengo
// esa parte lo hardcodeo. Borrar esto al obtener configuracion
ALGORITMO_PLANIFICACION = FIFO;
SLEEP_TIME = 1;


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

void ejecutar_rafaga(t_tcb_entrenador* tcb){
	while (!queue_is_empty(tcb->rafaga))
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);
}

void ejecutar_instruccion(int instruccion, t_tcb_entrenador* tcb){
	switch (instruccion){
	case MOVERSE:
		sleep(SLEEP_TIME);
		printf("Moviendose un paso\n");
		break;
	case CATCH:
		enviar_mensaje_catch(tcb, tcb->pokemon_a_capturar);
		break;
	case INTERCAMBIAR:
		//TODO
		break;
	}
}

void enviar_mensaje_catch(t_tcb_entrenador* tcb, t_pokemon* pokemon){
	int conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);

	if (conexion == -1){
		printf("ERROR: Conexion con [Broker] no establecida");
		exit(-1);
	}

	int bytes;

	int pos_x = pokemon->posicion->x;
	int pos_y = pokemon->posicion->y;
	int id_correlativo = 0;

	void *a_enviar = serializar_catch_pokemon(&bytes, pokemon->pokemon, pos_x, pos_y, id_correlativo);
	enviar_mensaje(conexion, a_enviar, bytes);

	liberar_conexion(conexion);
}
