#include "planificacion.h"

#include "test/testing.h"

t_dictionary* enviaron_catch;

// TODO: ALGORITMO_PLANIFICACION, SLEEP_TIME  y SLEEP_TIME_CONEXION se deben cargar por configuracion,
//pero como no tengo esa parte lo hardcodeo. Borrar esto al obtener configuracion
ALGORITMO_PLANIFICACION = FIFO;
SLEEP_TIME = 1;
SLEEP_TIME_CONEXION = 10;


void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
	unblocked = list_create();
}

void inicializar_diccionarios(){
	enviaron_catch = dictionary_create();
}

void iniciar_planificador(){
	pthread_create(&planificador,NULL,(void*)planificar, NULL);
	pthread_detach(planificador);
}

void planificar(){
	t_tcb_entrenador* tcb_exec = (t_tcb_entrenador*) malloc(sizeof(t_tcb_entrenador));

	while(1)
		//TODO: Poner semaforo en todos los hilos de ejecución que llamen a Ready
		if (!list_is_empty(ready)){
			tcb_exec = siguiente_tcb_a_ejecutar();
			ejecutar_rafaga(tcb_exec);
		}
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

void cargar_tcb_captura(t_tcb_entrenador* tcb, t_pokemon* pokemon) {
	cargar_rafaga_captura(tcb, pokemon->posicion );
	tcb->pokemon_a_capturar = pokemon;
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

void cargar_instruccion(t_tcb_entrenador* tcb, int instruccion) {
	queue_push(tcb->rafaga, instruccion);
}

int distancia_entre(t_posicion* inicio, t_posicion* destino) {
	int delta_x = fabs(destino->x - inicio->x);
	int delta_y = fabs(destino->y - inicio->y);

	return delta_x + delta_y;
}

void actualizar_posicion(t_tcb_entrenador* tcb){
	t_posicion* destino = tcb->pokemon_a_capturar->posicion;
	t_posicion* inicio = tcb->posicion;

	int delta_x = destino->x - inicio->x;
	int delta_y = destino->y - inicio->y;

	if (delta_x > 0){
		inicio->x++;
	}
	else if( delta_x < 0){
		inicio->x--;
	}
	else if( delta_x == 0){
		if( delta_y > 0){
			inicio->y++;
		}
		else if(delta_y < 0){
			inicio->y--;
		}
	}

}

void ejecutar_rafaga(t_tcb_entrenador* tcb){
	//TODO: Esta funcion la tiene que correr el hilo entrenador
	printf("Tamaño de rafaga: %d  ", queue_size(tcb->rafaga));
	printf("Posicion del TCB (%d, %d)\n", tcb->posicion->x, tcb->posicion->y);
	while (!queue_is_empty(tcb->rafaga)){
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);
	}
}

void ejecutar_instruccion(int instruccion, t_tcb_entrenador* tcb){
	switch (instruccion){
	case MOVERSE:
		sleep(SLEEP_TIME);
		actualizar_posicion(tcb);
		//TODO: Pasar a formato log (es requisito del tp)
		printf("[EXEC] TID:%d Instruccion:MOVERSE  Posicion:(%d, %d)\n",
				tcb->tid,
				tcb->posicion->x,
				tcb->posicion->y);
		break;
	case CATCH:
		//TODO: Pasar a formato log (es requisito del tp)
		printf("[EXEC] TID:%d Instruccion:CATCH\n", tcb->tid);
		//TODO: Este envio se tiene que hacer mediante un hilo, ya que hay que esperar
		// a que me devuelvan un id_correlativo y eso puede tardar
		enviar_mensaje_catch(tcb, tcb->pokemon_a_capturar);
		pasar_a_blocked(tcb);

		break;
	case INTERCAMBIAR:
		//TODO
		break;
	}
}

void enviar_mensaje_catch(t_tcb_entrenador* tcb, t_pokemon* pokemon){
	int conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);

	while (conexion == -1) {
		//TODO: Pasar a formato log (es requisito del tp)
		printf("ERROR: Conexion con [Broker] no establecida. Reintentando...");
		sleep(SLEEP_TIME_CONEXION);
		conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);
	}

	int bytes;

	int pos_x = pokemon->posicion->x;
	int pos_y = pokemon->posicion->y;

	void *a_enviar = serializar_catch_pokemon(&bytes, pokemon->pokemon, pos_x, pos_y, 0);
	enviar_mensaje(conexion, a_enviar, bytes);

	char* id_correlativo = recibir_id_correlativo(conexion);

	agregar_a_enviaron_catch(id_correlativo, tcb);
	liberar_conexion(conexion);
}


char* recibir_id_correlativo(int socket_cliente) {
	int id_correlativo_int;
	char id_correlativo_char[10];

	recv(socket_cliente, &id_correlativo_int, sizeof(int), 0);

	snprintf(id_correlativo_char, 10, "%d", id_correlativo_int);

	return id_correlativo_char;
}

void agregar_a_enviaron_catch(char* id_correlativo, t_tcb_entrenador* tcb){
	dictionary_put(enviaron_catch, id_correlativo, tcb);
}

void pasar_a_ready(t_tcb_entrenador* tcb) {
	list_add(ready, tcb);
	tcb->estado_tcb = READY;
}

void pasar_a_blocked(t_tcb_entrenador* tcb) {
	list_add(blocked, tcb);
	tcb->estado_tcb = BLOCKED;
}
