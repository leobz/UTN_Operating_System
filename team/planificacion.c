#include "planificacion.h"

#include "test/testing.h"

t_dictionary* enviaron_catch;

void inicializar_listas() {
	ready = list_create();
	blocked = list_create();
	new = list_create();
	unblocked = list_create();
	ready_to_exchange = list_create();
	l_exit = list_create();

	pthread_mutex_init(&mutex_lista_ready, NULL);
	pthread_mutex_init(&mutex_manejar_deadlock, NULL);
}

void inicializar_diccionarios(t_team_config* team_config) {
	inicializar_objetivo_global(team_config);
	inicializar_pokemones_atrapados(team_config);
	inicializar_pokemones_en_mapa();
	inicializar_pokemones_planificados();
	inicializar_tcbs_enviaron_catch();
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
		siguiente_tcb = obtener_tcb_menor_proxima_estimacion();
		break;
	}
	return siguiente_tcb;
}

t_tcb_entrenador* obtener_tcb_menor_proxima_estimacion() {
	t_tcb_entrenador* tcb_menor_estimacion = NULL;
	double estimacion_anterior = 0;

	void elegir_tcb_menor_proxima_estimacion(t_tcb_entrenador* tcb) {
		if (tcb_menor_estimacion == NULL)
			tcb_menor_estimacion = tcb;
		else if (tiene_menor_proxima_estimacion(tcb, tcb_menor_estimacion))
			tcb_menor_estimacion = tcb;
	}

	list_iterate(ready, (void*)elegir_tcb_menor_proxima_estimacion);

	estimacion_anterior = obtener_proxima_estimacion(tcb_menor_estimacion);
	tcb_menor_estimacion->estimacion_anterior = estimacion_anterior;
	tcb_menor_estimacion->rafaga_anterior = queue_size(tcb_menor_estimacion->rafaga);

	return tcb_menor_estimacion;
}

bool tiene_menor_proxima_estimacion(t_tcb_entrenador* tcb_posible_menor_estimacion, t_tcb_entrenador* tcb_actual_menor_estimacion){
	return obtener_proxima_estimacion(tcb_posible_menor_estimacion) < obtener_proxima_estimacion(tcb_actual_menor_estimacion);
}

double obtener_proxima_estimacion(t_tcb_entrenador* tcb) {
	return team_config->alpha * tcb->rafaga_anterior + (1 - team_config->alpha) * tcb->estimacion_anterior;
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
			ejecutar_rafaga(tcb);
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
		sleep(team_config->retardo_ciclo_cpu);
		break;
	case MOVERSE_A_ENTRENADOR:
		actualizar_posicion_entrenador_intercambio(tcb);

		log_info(logger, "[INSTRUCCION] TID:%d, MOVIMIENTO Posición:(%d, %d)", tcb->tid,
				tcb->posicion->x, tcb->posicion->y);
		sleep(team_config->retardo_ciclo_cpu);
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
		sleep(team_config->retardo_ciclo_cpu);
		break;
	case INTERCAMBIAR:
		log_info(logger, "[INSTRUCCION] INTERCAMBIO, TID entrenador: %d, TID entrenador a intercambiar: %d",
						tcb->tid,
						tcb->entrenador_a_intercambiar->tid);
		t_tcb_entrenador* entrenador = tcb;
		t_tcb_entrenador* entrenador_a_intercambiar = tcb->entrenador_a_intercambiar;

		ejecutar_intercambio(tcb);

		ejecutar_acciones_post_intercambio(entrenador_a_intercambiar, true);
		ejecutar_acciones_post_intercambio(entrenador, false);

		sleep(team_config->retardo_ciclo_cpu * 5);
		break;
	}
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
	cargar_rafaga_movimiento_a_entrenador(tcb, tcb->entrenador_a_intercambiar->posicion);
	cargar_instruccion(tcb, INTERCAMBIAR);
}

void cargar_rafaga_movimiento(t_tcb_entrenador* tcb,
		t_posicion* posicion_pokemon) {
	int movimientos = 0;

	while (movimientos < distancia_entre(tcb->posicion, posicion_pokemon)) {
		cargar_instruccion(tcb, MOVERSE);
		movimientos++;
	}
}

void cargar_rafaga_movimiento_a_entrenador(t_tcb_entrenador* tcb,
		t_posicion* posicion_tcb_a_intercambiar) {
	int movimientos = 0;

	while (movimientos < distancia_entre(tcb->posicion, posicion_tcb_a_intercambiar)) {
		cargar_instruccion(tcb, MOVERSE_A_ENTRENADOR);
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

void actualizar_posicion_entrenador_intercambio(t_tcb_entrenador* tcb) {
	t_posicion* destino = tcb->entrenador_a_intercambiar->posicion;
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

void ejecutar_intercambio(t_tcb_entrenador* tcb) {
	t_tcb_entrenador* entrenador = tcb;
	t_tcb_entrenador* entrenador_a_intercambiar = tcb->entrenador_a_intercambiar;

	dictionary_increment_value(entrenador->pokemones_capturados, entrenador_a_intercambiar->pokemon_a_dar_en_intercambio);
	dictionary_increment_value(entrenador_a_intercambiar->pokemones_capturados, entrenador->pokemon_a_dar_en_intercambio);

	remover_decrementar_value_en_diccionario(entrenador->pokemones_capturados, entrenador->pokemon_a_dar_en_intercambio);
	remover_decrementar_value_en_diccionario(entrenador_a_intercambiar->pokemones_capturados, entrenador_a_intercambiar->pokemon_a_dar_en_intercambio);

	entrenador->entrenador_a_intercambiar = NULL;
	entrenador->pokemon_a_dar_en_intercambio = NULL;

	entrenador_a_intercambiar->entrenador_a_intercambiar = NULL;
	entrenador_a_intercambiar->pokemon_a_dar_en_intercambio = NULL;
}

void ejecutar_acciones_post_intercambio(t_tcb_entrenador* tcb, bool es_entrenador_a_intercambiar) {
	if (cumplio_objetivo(tcb)) {
		list_remove_element(ready, tcb);
		pasar_a_exit(tcb);
	} else {
		if (es_entrenador_a_intercambiar)
			tcb->estado_tcb = READY_TO_EXCHANGE;
		else{
			list_remove_element(ready, tcb);
			pasar_a_ready_to_exchange(tcb, string_maximo_permitido(tcb));
		}
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

char* string_maximo_permitido(t_tcb_entrenador* tcb) {
	int length = strlen("Capturó máximo permitido()") + 10;
	char* string_maximo_permitido = malloc(length);
	snprintf(string_maximo_permitido, length, "Capturó máximo permitido(%d)", tcb->pokemones_max);
	return string_maximo_permitido;
}

void definir_cola_post_caught(t_tcb_entrenador* tcb) {
	if (capturo_maximo_permitido(tcb)) {
		if (cumplio_objetivo(tcb)) {
			printf("[TCB-info] TID:%d Cumplió objetivo\n", tcb->tid);
			pasar_a_exit(tcb);
		} else {
			pasar_a_ready_to_exchange(tcb, string_maximo_permitido(tcb));
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

	log_info(logger, "[CAPTURA] TID:%d Capturó pokemon. Total capturados:%d", tcb->tid, total_capturados(tcb));
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
	pasar_a_cola(tcb, unblocked, UNBLOCKED, "Capturó pokemon y puede seguir capturando");
}

void pasar_a_exit(t_tcb_entrenador* tcb) {
	pasar_a_cola(tcb, l_exit, EXIT, "Cumplió Objetivo");
	if (dictionaries_are_equals(objetivo_global, pokemones_atrapados)){
		log_info(logger,"[FIN DEL PROCESO] ¡Team cumplió objetivo!");
		// TODO: Mostar metricas
		// TODO: Cerrar conexiones
		// TODO: Finalizar proceso
	}
}

t_list* pokemones_necesitados(t_tcb_entrenador* tcb) {
	t_dictionary* necesitados =  dictionary_substract(tcb->objetivos, tcb->pokemones_capturados);
	return dictionary_keys_to_list(necesitados);
}

t_list* pokemones_no_necesitados(t_tcb_entrenador* tcb) {
	t_dictionary* necesitados =  dictionary_substract(tcb->pokemones_capturados, tcb->objetivos);
	return dictionary_keys_to_list(necesitados);
}

t_deadlock* crear_deadlock
(t_tcb_entrenador* tcb_1, t_tcb_entrenador* tcb_2, t_list* puede_dar_tcb_1, t_list* puede_dar_tcb_2){
	t_deadlock* deadlock = malloc(sizeof(t_deadlock));

	deadlock->tcb_1 = tcb_1;
	deadlock->tcb_2 = tcb_2;

	deadlock->tcb_1->entrenador_a_intercambiar = deadlock->tcb_2;
	deadlock->tcb_1->pokemon_a_dar_en_intercambio = list_first(puede_dar_tcb_1);
	deadlock->tcb_1->estado_tcb = DEADLOCK;
	deadlock->tcb_2->estado_tcb = DEADLOCK;

	deadlock->tcb_2->entrenador_a_intercambiar = deadlock->tcb_1;
	deadlock->tcb_2->pokemon_a_dar_en_intercambio = list_first(puede_dar_tcb_2);


	return deadlock;
}

t_deadlock* detectar_deadlock(t_tcb_entrenador* tcb_1) {
	log_info(logger, "[DEADLOCK] Inicio de detección de deadlock");

	t_deadlock* deadlock = NULL;
	t_list* necesitados_tcb_1 = pokemones_necesitados(tcb_1);
	t_list* no_necesitados_tcb_1 = pokemones_no_necesitados(tcb_1);

	t_deadlock* detectar_espera_circular(t_tcb_entrenador* tcb_2) {
		t_deadlock* deadlock = NULL;

		t_list* puede_dar_tcb_1 = list_intersection(pokemones_necesitados(tcb_2), no_necesitados_tcb_1);
		t_list* puede_dar_tcb_2 = list_intersection(pokemones_no_necesitados(tcb_2), necesitados_tcb_1);

		if(list_size(puede_dar_tcb_1) > 0 && list_size(puede_dar_tcb_2) >0)  {
			deadlock = crear_deadlock(tcb_1, tcb_2, puede_dar_tcb_1, puede_dar_tcb_2);
		}

		return deadlock;
	}

	for(int i = 0; i< list_size(ready_to_exchange); i++) {

		t_tcb_entrenador* potencial_cambiador = list_get(ready_to_exchange, i);

		if (tcb_1 != potencial_cambiador && potencial_cambiador->estado_tcb != DEADLOCK) {
			deadlock = detectar_espera_circular(potencial_cambiador);
		}

		if(deadlock != NULL){ break; }
	}

	return deadlock;
}

void despachar_resolucion_de_deadlock(t_deadlock* deadlock) {
	cargar_rafaga_intercambio(deadlock->tcb_1);
	pasar_a_ready(deadlock->tcb_1, "Entrenador va a intercambiar");
}

void loggear_deteccion_de_deadlock(t_deadlock* deadlock) {
	log_info(logger,
			"[DEADLOCK] Se detectó espera circular entre TCB: %d -> Pokemon: %s y TCB: %d -> Pokemon: %s",
			deadlock->tcb_1->tid,
			deadlock->tcb_1->pokemon_a_dar_en_intercambio,
			deadlock->tcb_2->tid,
			deadlock->tcb_2->pokemon_a_dar_en_intercambio);
}

void ejecutar_manejador_de_deadlocks(t_tcb_entrenador* tcb) {
	pthread_mutex_lock(&mutex_manejar_deadlock);
	t_deadlock* deadlock = detectar_deadlock(tcb);

	if (deadlock != NULL) {
		loggear_deteccion_de_deadlock(deadlock);
		despachar_resolucion_de_deadlock(deadlock);
	}
	else
	{
		log_info(logger,"[DEADLOCK] No se detectó espera circular entre los TCBs");
	}

	free(deadlock);
	pthread_mutex_unlock(&mutex_manejar_deadlock);
}

void pasar_a_ready_to_exchange(t_tcb_entrenador* tcb, char* motivo) {
	pasar_a_cola(tcb, ready_to_exchange, READY_TO_EXCHANGE, motivo);

	ejecutar_manejador_de_deadlocks(tcb);
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
	case UNBLOCKED:
		return "Unblocked";
	case READY_TO_EXCHANGE:
		return "Ready to Exchange";
	default:
		return "NULL";
	}
}
