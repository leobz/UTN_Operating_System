#include "planificacion.h"

#include "test/testing.h"

t_dictionary* enviaron_catch;

void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
	unblocked = list_create();
	deadlock = list_create();
	l_exit = list_create();

	pthread_mutex_init(&mutex_lista_ready, NULL);
	pthread_mutex_init(&mutex_manejar_deadlock, NULL);
}

void inicializar_diccionarios(t_team_config* team_config) {
	inicializar_objetivo_global(team_config);
	inicializar_pokemones_atrapados(team_config);
	inicializar_pokemones_en_mapa();
	pokemones_planificados = dictionary_create();

	enviaron_catch = dictionary_create();
}

void inicializar_hilos_tcbs() {
	void cargar_semaforo_y_thread_al_tcb(t_tcb_entrenador* entrenador) {
		pthread_t thread_tcb;
		pthread_create(&thread_tcb, NULL, (void*)ejecutar_tcb, entrenador);
		entrenador->entrenador = &thread_tcb;
	}

	list_iterate(new, (void*)cargar_semaforo_y_thread_al_tcb);
}

void iniciar_planificador() {
	inicializar_hilos_tcbs();
	pthread_create(&planificador, NULL, (void*) planificar, NULL);
	pthread_detach(planificador);
}

void desbloquear_ejecucion_tcb(t_tcb_entrenador* tcb_exec) {
	sem_post(tcb_exec->semaforo);
}

void planificar() {
	t_tcb_entrenador* tcb_exec = (t_tcb_entrenador*) malloc(sizeof(t_tcb_entrenador));
	tcb_exec = NULL;
	pthread_mutex_init(&mutex_tcb_exec, NULL);

	while (1)
		if (!list_is_empty(ready) && (tcb_exec == NULL)){
			pthread_mutex_lock(&mutex_tcb_exec);
			tcb_exec = siguiente_tcb_a_ejecutar();
			pasar_a_exec(tcb_exec);
			desbloquear_ejecucion_tcb(tcb_exec);
			tcb_exec = NULL;

		}
}

t_tcb_entrenador* siguiente_tcb_a_ejecutar() {
	t_tcb_entrenador* siguiente_tcb;

	int algoritmo = string_to_algoritmo_de_planificacion(team_config->algoritmo_de_planificacion);

	switch (algoritmo) {
	case FIFO:
		siguiente_tcb = list_pop_first(ready);
		break;
	case RR:
		siguiente_tcb = list_pop_first(ready);
		break;
	case SJF_CD:
		// TODO
		break;
	case SJF_SD:
		//TODO
		break;
	}
	return siguiente_tcb;
}

void inicializar_semaforo_tcb(t_tcb_entrenador* tcb, sem_t* semaforo_tcb) {
	sem_init(semaforo_tcb, 0, 0);
	tcb->semaforo = semaforo_tcb;
}

void ejecutar_rafaga(t_tcb_entrenador* tcb) {
	while (!queue_is_empty(tcb->rafaga)) {
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);
	}
}

void ejecutar_rafaga_con_desalojo(t_tcb_entrenador* tcb) {
	int cantidad_de_instrucciones = 0;
	while (!queue_is_empty(tcb->rafaga)) {
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);
		cantidad_de_instrucciones++;
		if (cantidad_de_instrucciones >= team_config->quantum){
			cantidad_de_instrucciones = 0;
			break;
		}
	}
}

void ejecutar_tcb(t_tcb_entrenador* tcb) {
	sem_t semaforo_tcb;
	inicializar_semaforo_tcb(tcb, &semaforo_tcb);

	while(true){
		sem_wait(tcb->semaforo);

		printf("Tamaño de rafaga: %d  ", queue_size(tcb->rafaga));
		printf("Posicion del TCB (%d, %d)\n", tcb->posicion->x, tcb->posicion->y);

		int algoritmo = string_to_algoritmo_de_planificacion(team_config->algoritmo_de_planificacion);

		switch (algoritmo) {
		case FIFO:
			ejecutar_rafaga(tcb);
			break;
		case RR:
			ejecutar_rafaga_con_desalojo(tcb);
			if (!queue_is_empty(tcb->rafaga))
				pasar_a_ready(tcb, "QUANTUM");
			break;

		case SJF_CD:
			// TODO
			break;
		case SJF_SD:
			//TODO
			break;
		}
		pthread_mutex_unlock(&mutex_tcb_exec);

	}
}

void ejecutar_instruccion(int instruccion, t_tcb_entrenador* tcb) {
	switch (instruccion) {
	case MOVERSE:
		actualizar_posicion(tcb);

		log_info(logger, "[INSTRUCCION] TID:%d, MOVIMIENTO Posición:(%d, %d)", tcb->tid,
				tcb->posicion->x, tcb->posicion->y);
		break;
	case CATCH:
		log_info(logger, "[INSTRUCCION] TID:%d, CATCH %s %d %d",
				tcb->tid,
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
	sleep(team_config->retardo_ciclo_cpu);
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
		sleep(team_config->tiempoDeReconexion);
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
		aplicar_acciones_caught(tcb);
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

void agregar_pokemon_a_atrapados(t_tcb_entrenador* tcb) {
	dictionary_increment_value(pokemones_atrapados, tcb->pokemon_a_capturar->pokemon);
}

void confirmar_caught(t_tcb_entrenador* tcb){
	agregar_pokemon_a_atrapados(tcb);
	asignar_pokemon(tcb);
	printf("[TCB-info] TID:%d Capturó pokemon. Total capturados:%d\n", tcb->tid, total_capturados(tcb));
}

char* recibir_id_correlativo(int socket_cliente) {
	t_paquete_socket* paquete =  recibir_mensajes(socket_cliente);

	int length = snprintf( NULL, 0, "%d", paquete->id_correlativo);
	char* id_correlativo_char = malloc( length + 1 );
	snprintf(id_correlativo_char, length + 1, "%d", paquete->id_correlativo);

	log_info(logger, "[MSG_RECIBIDO] ID_CORRELATIVO para CATCH:%s", id_correlativo_char);

	return id_correlativo_char;
}

void agregar_a_enviaron_catch(char* id_correlativo, t_tcb_entrenador* tcb) {
	dictionary_put(enviaron_catch, id_correlativo, tcb);
}

void pasar_a_cola(t_tcb_entrenador* tcb, t_list* lista,int cola_destino, char* motivo) {
	int estado_original = tcb->estado_tcb;
	tcb->estado_tcb = cola_destino;
	log_info(logger, "[CAMBIO DE COLA] TID:%d (%s->%s) (%d, %d) Motivo:%s",
			tcb->tid,
			cola_planificacion_a_string(estado_original),
			cola_planificacion_a_string(tcb->estado_tcb),
			tcb->posicion->x,
			tcb->posicion->y,
			motivo);
	list_add(lista, tcb);
}

void pasar_a_ready(t_tcb_entrenador* tcb, char* motivo) {
	pthread_mutex_lock(&mutex_lista_ready);
	pasar_a_cola(tcb, ready, READY, motivo);
	pthread_mutex_unlock(&mutex_lista_ready);
}

void pasar_a_exec(t_tcb_entrenador* tcb_exec) {
	tcb_exec->estado_tcb = EXEC;
	list_remove_element(ready, tcb_exec);

}

void pasar_a_blocked(t_tcb_entrenador* tcb) {
	list_add(blocked, tcb);
	tcb->estado_tcb = BLOCKED;
	log_info(logger,"[CAMBIO DE COLA] TID:%d Pasó a lista Blocked", tcb->tid);
}

void pasar_a_unblocked(t_tcb_entrenador* tcb) {
	list_add(unblocked, tcb);
	printf("[CAMBIO DE COLA] TID:%d Pasó a lista Unblocked\n", tcb->tid);
}

void pasar_a_exit(t_tcb_entrenador* tcb) {
	list_add(l_exit, tcb);
	printf("[CAMBIO DE COLA] TID:%d Pasó a lista Exit\n", tcb->tid);
}


t_deadlock* detectar_deadlock() {

}

void ejecutar_manejador_de_deadlocks() {
	pthread_mutex_lock(&mutex_manejar_deadlock);
	t_deadlock* deadlock = detectar_deadlock();
	pthread_mutex_unlock(&mutex_manejar_deadlock);
}

void pasar_a_deadlock(t_tcb_entrenador* tcb) {
	pthread_mutex_lock(&mutex_lista_ready);
	pasar_a_cola(tcb, deadlock, DEADLOCK, "Deadlock");
	pthread_mutex_unlock(&mutex_lista_ready);

	ejecutar_manejador_de_deadlocks();
}

int string_to_algoritmo_de_planificacion(char* algoritmo) {

	if (strcmp(algoritmo, "FIFO") == 0)
		return FIFO;

	else if (strcmp(algoritmo, "RR") == 0)
		return RR;

	else if (strcmp(algoritmo, "SJF-CD") == 0)
		return SJF_CD;

	else if (strcmp(algoritmo, "SJF-SD") == 0)
		return SJF_SD;
}

char* cola_planificacion_a_string(int cola_planificacion){
	switch (cola_planificacion) {
	case READY:
		return "Ready";
	case NEW:
		return "New";
	case BLOCKED:
		return "Blocked";
	case EXEC:
		return "Exec";
	case EXIT:
		return "Exit";
	case DEADLOCK:
		return "Deadlock";
	default:
		return "NULL";
	}
}
