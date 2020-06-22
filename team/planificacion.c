#include "planificacion.h"

#include "test/testing.h"

t_dictionary* enviaron_catch;

// TODO: ALGORITMO_PLANIFICACION, SLEEP_TIME  y SLEEP_TIME_CONEXION se deben cargar por configuracion,
//pero como no tengo esa parte lo hardcodeo. Borrar esto al obtener configuracion
ALGORITMO_PLANIFICACION = FIFO;

int SLEEP_TIME = 0;
int SLEEP_TIME_CONEXION = 10;

void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
	unblocked = list_create();
	deadlock = list_create();
	l_exit = list_create();
}

void inicializar_diccionarios() {
	enviaron_catch = dictionary_create();
}

void iniciar_planificador() {
	pthread_create(&planificador, NULL, (void*) planificar, NULL);
	pthread_detach(planificador);
}

void planificar() {
	t_tcb_entrenador* tcb_exec = (t_tcb_entrenador*) malloc(
			sizeof(t_tcb_entrenador));

	tcb_exec = NULL;

	while (1)
		//TODO: Poner semaforo en todos los hilos de ejecución que llamen a Ready
		if (!list_is_empty(ready) && (tcb_exec == NULL)){

			tcb_exec = siguiente_tcb_a_ejecutar();
			ejecutar_rafaga(tcb_exec);
			tcb_exec = NULL;
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
	cargar_rafaga_captura(tcb, pokemon->posicion);
	tcb->pokemon_a_capturar = pokemon;
}

void cargar_rafaga_captura(t_tcb_entrenador* tcb, t_posicion* posicion_pokemon) {
	cargar_rafaga_movimiento(tcb, posicion_pokemon);
	cargar_instruccion(tcb, CATCH);

}

void cargar_rafaga_intercambio(t_tcb_entrenador* tcb) {
	// TODO
}

void cargar_rafaga_movimiento(t_tcb_entrenador* tcb,
		t_posicion* posicion_pokemon) {
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

void actualizar_posicion(t_tcb_entrenador* tcb) {
	t_posicion* destino = tcb->pokemon_a_capturar->posicion;
	t_posicion* inicio = tcb->posicion;

	int delta_x = destino->x - inicio->x;
	int delta_y = destino->y - inicio->y;

	if (delta_x > 0) {
		inicio->x++;
	} else if (delta_x < 0) {
		inicio->x--;
	} else if (delta_x == 0) {
		if (delta_y > 0) {
			inicio->y++;
		} else if (delta_y < 0) {
			inicio->y--;
		}
	}

}

void ejecutar_rafaga(t_tcb_entrenador* tcb) {
	//TODO: Esta funcion la tiene que correr el hilo entrenador
	printf("Tamaño de rafaga: %d  ", queue_size(tcb->rafaga));
	printf("Posicion del TCB (%d, %d)\n", tcb->posicion->x, tcb->posicion->y);
	while (!queue_is_empty(tcb->rafaga)) {
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);
	}
}

void ejecutar_instruccion(int instruccion, t_tcb_entrenador* tcb) {
	switch (instruccion) {
	case MOVERSE:
		sleep(SLEEP_TIME);
		actualizar_posicion(tcb);

		log_info(logger, "[MOVIMIENTO] ID_ENTRENADOR:%d, POSICION:(%d, %d)", tcb->tid,
				tcb->posicion->x, tcb->posicion->y);
		break;
	case CATCH:
		log_info(logger, "[CATCH] POKEMON: %s, POSICION:(%d, %d)",
				tcb->pokemon_a_capturar->pokemon,
				tcb->pokemon_a_capturar->posicion->x,
				tcb->pokemon_a_capturar->posicion->y);
		//TODO: Este envio se tiene que hacer mediante un hilo, ya que hay que esperar
		// a que me devuelvan un id_correlativo y eso puede tardar
		ejecutar_catch(tcb);

		break;
	case INTERCAMBIAR:
		//TODO
		break;
	}
}


void lanzar_reintentar_conexion(int conexion){
	pthread_create(&reintentador_de_conexion, NULL, (void*) planificar, conexion);
	pthread_detach(reintentador_de_conexion);
}

void reintentar_conexion(int conexion) {
	//TODO: Preguntar el foro de Github si esto se va a usar para todos los mensajes
	// Es decir, usar la misma conexion para distintos mensajes

	while (conexion == -1) {

		log_info(logger,
				"[REINTENTO_COMUNICACION] Inicio de proceso de reintento de comunicación con el Broker.");
		sleep(SLEEP_TIME_CONEXION);
		conexion = crear_conexion(team_config->ip_broker,
				team_config->puerto_broker);

		if (conexion == -1)
			log_info(logger,
					"[RESULTADO_REINTENTO_COMUNICACION] Conexión con Broker no establecida.");
		else
			log_info(logger,
					"[RESULTADO_REINTENTO_COMUNICACION] Conexión con Broker establecida.");
	}
}

void ejecutar_catch(t_tcb_entrenador* tcb){
	t_pokemon* pokemon = tcb->pokemon_a_capturar;
	int conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);

	if (conexion == -1){

		//printf("ERROR al conectar con BROKER");
		//exit(1);
		// TODO: Preguntar si en foro si la mantenemos o no, caso contrario quitar
		// lanzar_reintentar_conexion(int conexion);
		confirmar_caught(tcb);
	}
	else{

		int bytes;

		int pos_x = pokemon->posicion->x;
		int pos_y = pokemon->posicion->y;

		void *a_enviar = serializar_catch_pokemon(&bytes, pokemon->pokemon, pos_x, pos_y, 0,0);
		enviar_mensaje(conexion, a_enviar, bytes);

		char* id_correlativo = recibir_id_correlativo(conexion);

		agregar_a_enviaron_catch(id_correlativo, tcb);
		pasar_a_blocked(tcb);
		liberar_conexion(conexion);
	}

}

int total_capturados(t_tcb_entrenador* tcb) {
	return sum_dictionary_values(tcb->pokemones_capturados);
}

int capturo_maximo_permitido(t_tcb_entrenador* tcb) {
	return tcb->pokemones_max == total_capturados(tcb);
}

void asignar_pokemon(t_tcb_entrenador* tcb) {
	dictionary_increment_value(
			tcb->pokemones_capturados,
			tcb->pokemon_a_capturar->pokemon);

	tcb->pokemon_a_capturar = NULL;
}

int cumplio_objetivo(t_tcb_entrenador* tcb) {
	//TODO: Cambiar nombre por "dictionarie_include()" o algo asi
	return dictionaries_are_equals(tcb->pokemones_capturados, tcb->objetivos);
}

void definir_cola_post_caught(t_tcb_entrenador* tcb) {
	if (capturo_maximo_permitido(tcb)) {
		printf("[TCB-info] TID:%d Capturó máximo permitido(%d)\n", tcb->tid, tcb->pokemones_max);

		if (cumplio_objetivo(tcb)) {
			printf("[TCB-info] TID:%d Cumplió objetivo\n", tcb->tid);
			pasar_a_exit(tcb);
		} else {
			pasar_a_deadlock(tcb);
		}
	} else {
		pasar_a_unblocked(tcb);
	}
}

void confirmar_caught(t_tcb_entrenador* tcb){
	asignar_pokemon(tcb);
	printf("[TCB-info] TID:%d Capturó pokemon. Total capturados:%d\n", tcb->tid, total_capturados(tcb));
	definir_cola_post_caught(tcb);
}

char* recibir_id_correlativo(int socket_cliente) {
	t_paquete_socket* paquete =  recibir_mensaje_servidor(socket_cliente);

	int length = snprintf( NULL, 0, "%d", paquete->id_correlativo);
	char* id_correlativo_char = malloc( length + 1 );
	snprintf(id_correlativo_char, length + 1, "%d", paquete->id_correlativo);

	log_info(logger, "[MSG_RECIBIDO] ID_CORRELATIVO para CATCH:%s", id_correlativo_char);

	return id_correlativo_char;
}

void agregar_a_enviaron_catch(char* id_correlativo, t_tcb_entrenador* tcb) {
	dictionary_put(enviaron_catch, id_correlativo, tcb);
}

void pasar_a_ready(t_tcb_entrenador* tcb) {
	list_add(ready, tcb);
	tcb->estado_tcb = READY;
}

void pasar_a_blocked(t_tcb_entrenador* tcb) {
	list_add(blocked, tcb);
	tcb->estado_tcb = BLOCKED;
	log_info(logger,"[TCB-info] TID:%d Pasó a lista Blocked\n", tcb->tid);
}

void pasar_a_unblocked(t_tcb_entrenador* tcb) {
	list_add(unblocked, tcb);
	printf("[TCB-info] TID:%d Pasó a lista Unblocked\n", tcb->tid);
}

void pasar_a_exit(t_tcb_entrenador* tcb) {
	list_add(l_exit, tcb);
	printf("[TCB-info] TID:%d Pasó a lista Exit\n", tcb->tid);
}

void pasar_a_deadlock(t_tcb_entrenador* tcb) {
	list_add(deadlock, tcb);
	printf("[TCB-info] TID:%d Pasó a lista Deadlock\n", tcb->tid);
}
