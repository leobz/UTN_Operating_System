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

	deadlock_actual = NULL;

	pthread_mutex_init(&mutex_planificador, NULL);
	pthread_mutex_init(&mutex_manejar_deadlock, NULL);
	pthread_mutex_init(&mutex_lista_new, NULL);
	pthread_mutex_init(&mutex_mapa, NULL);

}

void inicializar_diccionarios(t_team_config* team_config) {
	inicializar_objetivo_global(team_config);
	inicializar_pokemones_atrapados(team_config);
	inicializar_pokemones_en_mapa();

	enviaron_get = dictionary_create();

	inicializar_pokemones_planificados();
	inicializar_tcbs_enviaron_catch();
}

void inicializar_hilos_tcbs() {
	void cargar_semaforo_y_thread_al_tcb(t_tcb_entrenador* entrenador) {
		pthread_t thread_tcb;
		pthread_create(&thread_tcb, NULL, (void*)ejecutar_tcb, entrenador);
		pthread_detach(thread_tcb);
		entrenador->entrenador = &thread_tcb;
	}

	list_iterate(new, (void*)cargar_semaforo_y_thread_al_tcb);
}

void iniciar_planificador() {
	finalizo_planificador = false;
	inicializar_hilos_tcbs();
	pthread_create(&planificador, NULL, (void*) planificar, NULL);
	pthread_detach(planificador);
}

void inicializar_metricas() {
	metricas = malloc(sizeof(t_metricas));

	metricas->cantidad_ciclos_CPU_totales = 0;
	metricas->cantidad_cambios_contexto = 0;
	metricas->cantidad_ciclos_CPU_entrenador = dictionary_create();
	metricas->cantidad_deadlocks_producidos = 0;
	metricas->cantidad_deadlocks_resueltos = 0;
}

void desbloquear_ejecucion_tcb(t_tcb_entrenador* tcb_exec) {
	sem_post(tcb_exec->semaforo);
}

void planificar() {
	t_tcb_entrenador* tcb_exec;
	tcb_exec = NULL;
	pthread_mutex_init(&mutex_tcb_exec, NULL);

	while (1) {
		pthread_mutex_lock(&mutex_planificador);

		if(finalizo_planificador){
			break;
		}

		if (!list_is_empty(ready) && (tcb_exec == NULL)){
			pthread_mutex_lock(&mutex_tcb_exec);

			if(finalizo_planificador){
				break;
			}


			tcb_exec = siguiente_tcb_a_ejecutar();
			pasar_a_exec(tcb_exec);
			desbloquear_ejecucion_tcb(tcb_exec);
			tcb_exec = NULL;
		}
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
		siguiente_tcb = obtener_tcb_menor_proxima_estimacion_sjf_cd();
		break;
	case SJF_SD:
		siguiente_tcb = obtener_tcb_menor_proxima_estimacion();
		break;
	}
	return siguiente_tcb;
}

t_tcb_entrenador* obtener_tcb_menor_proxima_estimacion_sjf_cd() {
	t_tcb_entrenador* tcb_menor_estimacion = NULL;
	double estimacion_anterior = 0;

	void elegir_tcb_menor_proxima_estimacion(t_tcb_entrenador* tcb) {
		if (tcb_menor_estimacion == NULL)
			tcb_menor_estimacion = tcb;
		else if (tiene_menor_proxima_estimacion_sjf_cd(tcb, tcb_menor_estimacion))
			tcb_menor_estimacion = tcb;
	}

	list_iterate(ready, (void*)elegir_tcb_menor_proxima_estimacion);

	if (tcb_menor_estimacion != NULL && tcb_menor_estimacion->necesita_nueva_estimacion) {
		estimacion_anterior = obtener_proxima_estimacion(tcb_menor_estimacion);
		tcb_menor_estimacion->estimacion_anterior = estimacion_anterior;
		tcb_menor_estimacion->estimacion_remanente = estimacion_anterior;
		tcb_menor_estimacion->necesita_nueva_estimacion = false;
		tcb_menor_estimacion->rafaga_anterior = queue_size(tcb_menor_estimacion->rafaga);
	}

	return tcb_menor_estimacion;
}

bool tiene_menor_proxima_estimacion_sjf_cd(t_tcb_entrenador* tcb, t_tcb_entrenador* tcb_menor_estimacion) {
	if (tcb->necesita_nueva_estimacion && tcb_menor_estimacion->necesita_nueva_estimacion)
		return tiene_menor_proxima_estimacion(tcb, tcb_menor_estimacion);
	else if (!tcb->necesita_nueva_estimacion && tcb_menor_estimacion->necesita_nueva_estimacion)
		return tcb->estimacion_remanente < obtener_proxima_estimacion(tcb_menor_estimacion);
	else if (tcb->necesita_nueva_estimacion && !tcb_menor_estimacion->necesita_nueva_estimacion)
		return obtener_proxima_estimacion(tcb) < tcb_menor_estimacion->estimacion_remanente;
	else
		return tcb->estimacion_remanente < tcb_menor_estimacion->estimacion_remanente;
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
	tcb_menor_estimacion->estimacion_remanente = estimacion_anterior;
	tcb_menor_estimacion->necesita_nueva_estimacion = false;
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

void ejecutar_rafaga_con_desalojo_sjf(t_tcb_entrenador* tcb) {
	while (!queue_is_empty(tcb->rafaga)) {
		ejecutar_instruccion(queue_peek(tcb->rafaga), tcb);
		queue_pop(tcb->rafaga);

		if (queue_is_empty(tcb->rafaga)){
			tcb->necesita_nueva_estimacion = true;
		}
		else {
			tcb->estimacion_remanente--;

			t_tcb_entrenador* tcb_en_ready_de_menor_rafaga = obtener_tcb_menor_proxima_estimacion_sjf_cd();
			if (tcb_en_ready_de_menor_rafaga != NULL && tiene_menor_proxima_estimacion_sjf_cd(tcb_en_ready_de_menor_rafaga, tcb)){
				break;
			}
		}
	}
}

void pasar_a_ready_si_esta_libre_y_hay_pokemon_en_mapa(t_tcb_entrenador* tcb) {
	if (list_include(unblocked, tcb)) {
		pasar_tcb_a_ready_si_hay_pokemones_en_mapa(tcb);
	}
}

void ejecutar_tcb(t_tcb_entrenador* tcb) {
	sem_t semaforo_tcb;
	inicializar_semaforo_tcb(tcb, &semaforo_tcb);

	while(true){
		sem_wait(tcb->semaforo);

		if (tcb->finalizo) {
			destroy_tcb_entrenador_full(tcb);
			break;
		}

		printf("TCB: %d -> Tamaño de rafaga: %d  ", tcb->tid, queue_size(tcb->rafaga));
		printf("Posicion del TCB: %d ->(%d, %d)\n", tcb->tid, tcb->posicion->x, tcb->posicion->y);

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
			ejecutar_rafaga_con_desalojo_sjf(tcb);
			break;
		case SJF_SD:
			ejecutar_rafaga(tcb);
			break;
		}

		pasar_a_ready_si_esta_libre_y_hay_pokemon_en_mapa(tcb);

		pthread_mutex_unlock(&mutex_tcb_exec);
	}
}

void ejecutar_instruccion(int instruccion, t_tcb_entrenador* tcb) {
	int cantidad_ciclos_instruccion = 1;

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
		log_info(logger, "[INSTRUCCION] INTERCAMBIO, TID: %d TID entrenador a intercambiar: %d",
						tcb->tid,
						tcb->entrenador_a_intercambiar->tid);
		t_tcb_entrenador* entrenador = tcb;
		t_tcb_entrenador* entrenador_a_intercambiar = tcb->entrenador_a_intercambiar;

		ejecutar_intercambio(tcb);
		cantidad_ciclos_instruccion = 5;
		sleep(team_config->retardo_ciclo_cpu * 5);

		ejecutar_acciones_post_intercambio(entrenador_a_intercambiar, true);
		ejecutar_acciones_post_intercambio(entrenador, false);

		continuar_o_manejar_deadlock();

		break;
	}

	metricas->cantidad_ciclos_CPU_totales += cantidad_ciclos_instruccion;
	char* tid = pasar_a_char(tcb->tid);
	dictionary_increment_value_in(metricas->cantidad_ciclos_CPU_entrenador, tid, cantidad_ciclos_instruccion);
	free(tid);
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

		char* id_correlativo = recibir_id_mensaje(conexion, pokemon->pokemon, CATCH_POKEMON);

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
		list_remove_element(ready_to_exchange, tcb);
		list_remove_element(deadlock_actual, tcb);
		pasar_a_exit(tcb);
	} else if (!es_entrenador_a_intercambiar){
		list_remove_element(ready, tcb);
		list_remove_element(deadlock_actual, tcb);
		pasar_a_cola(tcb, ready_to_exchange, READY_TO_EXCHANGE, string_motivo_intercambio());
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
	return dictionaries_are_equals(tcb->pokemones_capturados, tcb->objetivos) && dictionaries_are_equals(tcb->objetivos, tcb->pokemones_capturados);
}

char* string_maximo_permitido(t_tcb_entrenador* tcb) {
	int length = strlen("Capturó máximo permitido()") + 10;
	char* string_maximo_permitido = malloc(length);
	snprintf(string_maximo_permitido, length, "Capturó máximo permitido(%d)", tcb->pokemones_max);
	return string_maximo_permitido;
}

char* string_motivo_intercambio() {
	int length = strlen("Finalizó intercambio con tcb") + 10;
	char* motivo_intercambio = malloc(length);
	snprintf(motivo_intercambio, length, "Finalizó intercambio con tcb");
	return motivo_intercambio;
}

void definir_cola_post_caught(t_tcb_entrenador* tcb) {
	if (capturo_maximo_permitido(tcb)) {
		if (cumplio_objetivo(tcb)) {
			printf("[TCB-info] TID:%d Cumplió objetivo\n", tcb->tid);
			pasar_a_exit(tcb);
		} else {
			char* motivo_maximo_permitido = string_maximo_permitido(tcb);
			pasar_a_ready_to_exchange(tcb, motivo_maximo_permitido);
			free(motivo_maximo_permitido);
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
	metricas->cantidad_cambios_contexto++;
	pasar_a_cola(tcb, ready, READY, motivo);
	pthread_mutex_unlock(&mutex_planificador);
}

void pasar_a_exec(t_tcb_entrenador* tcb_exec) {
	tcb_exec->estado_tcb = EXEC;
	list_remove_element(ready, tcb_exec);
	metricas->cantidad_cambios_contexto++;
}

void pasar_a_blocked(t_tcb_entrenador* tcb) {
	pasar_a_cola(tcb, blocked, BLOCKED, "Espera confirmacion Caught");
	metricas->cantidad_cambios_contexto++;
	pthread_mutex_unlock(&mutex_planificador);
}

void pasar_a_unblocked(t_tcb_entrenador* tcb) {
	pasar_a_cola(tcb, unblocked, UNBLOCKED, "Capturó pokemon y puede seguir capturando");
	metricas->cantidad_cambios_contexto++;
	pthread_mutex_unlock(&mutex_planificador);
}

void finalizar_hilo_tcb(t_tcb_entrenador* tcb) {
	tcb->finalizo = true;
	sem_post(tcb->semaforo);
}

void finalizar_hilo_planificador() {
	finalizo_planificador = true;
	pthread_mutex_unlock(&mutex_planificador);
	pthread_mutex_unlock(&mutex_tcb_exec);

}

void debug_loggear_estado_exit() {
	bool todos_tcbs_estado_exit(t_tcb_entrenador* tcb) {
		return tcb->estado_tcb == EXIT;
	}


	log_info(logger_debug, "Objetivo Global:");
	debug_loggear_diccionario(objetivo_global);
	log_info(logger_debug, "Total de Pokemones Atrapados:");
	debug_loggear_diccionario(pokemones_atrapados);
	log_info(logger_debug, "Todos en exit? : %d", todos_los_entrenadores_exit());
	if(todos_los_entrenadores_exit != true){
		log_info(logger_debug, "  Todos los TCBs en lista exit con estado Exit?: %d",
				list_all_satisfy(l_exit, (void*) todos_tcbs_estado_exit));

		log_info(logger_debug, "  Tamaño de lista new: %d", list_size(new));
		log_info(logger_debug, "  Tamaño de lista ready: %d", list_size(ready));
		log_info(logger_debug, "  Tamaño de lista blocked: %d", list_size(blocked));
		log_info(logger_debug, "  Tamaño de lista ready_to_exchange: %d", list_size(ready_to_exchange));
		log_info(logger_debug, "  Tamaño de lista l_exit: %d", list_size(l_exit));

	}
}

void pasar_a_exit(t_tcb_entrenador* tcb) {
	pasar_a_cola(tcb, l_exit, EXIT, "Cumplió Objetivo");
	list_remove_element(ready,tcb);
	list_remove_element(blocked, tcb);
	list_remove_element(unblocked, tcb);
	list_remove_element(ready_to_exchange, tcb);
	list_remove_element(new, tcb);

	finalizar_hilo_tcb(tcb);

	debug_loggear_estado_exit();
	metricas->cantidad_cambios_contexto++;
	if (dictionaries_are_equals(objetivo_global, pokemones_atrapados) && todos_los_entrenadores_exit()){
		log_info(logger,"[FIN DEL PROCESO] ¡Team cumplió objetivo!");
		// TODO: Cerrar conexiones
		// TODO: Finalizar proceso

		if (deadlock_actual != NULL) {
			if (list_size(deadlock_actual) == 0 || list_size(deadlock_actual) == 1){
				metricas->cantidad_deadlocks_resueltos++;
			}
		}

		log_info(logger, "[METRICAS] Cantidad de ciclos de CPU totales: %d", metricas->cantidad_ciclos_CPU_totales);
		log_info(logger, "[METRICAS] Cantidad de cambios de contexto: %d", metricas->cantidad_cambios_contexto);
		void imprimir_ciclos_tcbs(char* key, void* value){
			log_info(logger, "[METRICAS] TID: %s, cantidad ciclos de CPU: %d", key, (int)value);
		}

		dictionary_iterator(metricas->cantidad_ciclos_CPU_entrenador, (void*) imprimir_ciclos_tcbs);

		log_info(logger, "[METRICAS] Cantidad de deadlocks producidos: %d", metricas->cantidad_deadlocks_producidos);
		log_info(logger, "[METRICAS] Cantidad de deadlocks resueltos: %d", metricas->cantidad_deadlocks_resueltos);

		team_cumplio_objetivo = true;
		finalizar_hilo_planificador();
		finalizar_team(team_config);
	}

	pthread_mutex_unlock(&mutex_planificador);
}

bool todos_los_entrenadores_exit() {

	bool todos_tcbs_estado_exit(t_tcb_entrenador* tcb) {
		return tcb->estado_tcb == EXIT;
	}

	return list_size(new) == 0
		&& list_size(ready) == 0
		&& list_size(blocked) == 0
		&& list_size(unblocked) == 0
		&& list_size(ready_to_exchange) == 0
		&& list_size(l_exit) == team_config->cantidad_entrenadores
		&& list_all_satisfy(l_exit, (void*) todos_tcbs_estado_exit);
}

t_list* pokemones_necesitados(t_tcb_entrenador* tcb) {
	t_dictionary* necesitados =  dictionary_substract(tcb->objetivos, tcb->pokemones_capturados);
	t_list* necesitados_lista = dictionary_keys_to_list(necesitados);

	dictionary_destroy_keys(necesitados);

	return necesitados_lista;
}

t_list* pokemones_no_necesitados(t_tcb_entrenador* tcb) {
	t_dictionary* no_necesitados =  dictionary_substract(tcb->pokemones_capturados, tcb->objetivos);
	t_list* no_necesitados_lista = dictionary_keys_to_list(no_necesitados);

	dictionary_destroy(no_necesitados);
	//free(no_necesitados->elements);
	//free(no_necesitados);

	return no_necesitados_lista;
}

t_deadlock* crear_deadlock(t_list* lista_deadlock){
	t_deadlock* deadlock = malloc(sizeof(t_deadlock));

	t_tcb_entrenador* tcb_anterior = list_get(lista_deadlock,1);
	t_tcb_entrenador* tcb_actual = list_get(lista_deadlock,0);
	t_tcb_entrenador* tcb_a_intercambiar = list_get(lista_deadlock,list_size(lista_deadlock)-1);

	t_list* no_necesita_actual = pokemones_no_necesitados(tcb_actual);
	t_list* necesita_actual =  pokemones_necesitados(tcb_actual);
	t_list* necesita_anterior = pokemones_necesitados(tcb_anterior);
	t_list* no_necesita_a_intercambiar = pokemones_no_necesitados(tcb_a_intercambiar);

	t_list* tcb_actual_pokemones_a_intercambiar = list_intersection_strings(no_necesita_actual, necesita_anterior);
	t_list* tcb_a_intercambiar_pokemones_a_intercambiar = list_intersection_strings(no_necesita_a_intercambiar, necesita_actual);

	tcb_actual->entrenador_a_intercambiar = tcb_a_intercambiar;
	tcb_actual->pokemon_a_dar_en_intercambio = list_pop_first(tcb_actual_pokemones_a_intercambiar);

	tcb_a_intercambiar->entrenador_a_intercambiar = tcb_actual;
	tcb_a_intercambiar->pokemon_a_dar_en_intercambio = list_pop_first(tcb_a_intercambiar_pokemones_a_intercambiar);

	deadlock->tcb_1 = tcb_actual;
	deadlock->tcb_2 = tcb_a_intercambiar;

	list_destroy_and_destroy_elements(tcb_actual_pokemones_a_intercambiar, (void *)free);
	list_destroy_and_destroy_elements(tcb_a_intercambiar_pokemones_a_intercambiar, (void *)free);
	list_destroy_and_destroy_elements(no_necesita_actual, (void *)free);
	list_destroy_and_destroy_elements(necesita_actual, (void *)free);
	list_destroy_and_destroy_elements(necesita_anterior, (void *)free);
	list_destroy_and_destroy_elements(no_necesita_a_intercambiar, (void *)free);





	return deadlock;
}

//TODO: Este metodo debe devolver una lista con todos los involucrados en la espera circular
t_list* detectar_deadlock_recursivo(t_tcb_entrenador* tcb_que_llega) {
	log_info(logger, "[DEADLOCK] Inicio de detección de deadlock");

	t_tcb_entrenador* tcb_actual = tcb_que_llega;
	t_list* tcbs_en_niveles_de_grafo = list_create();
	t_list* en_deadlock = NULL;

	bool le_puede_dar(t_tcb_entrenador* tcb_que_puede_recibir) {
		if (tcb_que_puede_recibir->estado_tcb != DEADLOCK) {

			t_list* no_necesita_actual = pokemones_no_necesitados(tcb_actual);
			t_list* necesita_anterior = pokemones_necesitados(tcb_que_puede_recibir);
			t_list* pokemones_que_puede_recibir =
					list_intersection_strings(no_necesita_actual, necesita_anterior);

			list_destroy_and_destroy_elements(no_necesita_actual, (void*) free);
			list_destroy_and_destroy_elements(necesita_anterior, (void*) free);

			if (list_size(pokemones_que_puede_recibir) > 0) {
				list_destroy_and_destroy_elements(pokemones_que_puede_recibir, (void*) free);
				return true;
			}
			else {
				list_destroy_and_destroy_elements(pokemones_que_puede_recibir, (void*) free);
				return false;
			}
		}
		else
			return false;

	}

	tcb_actual->nivel_de_grafo_en_deadlock = 0;
	tcb_actual->les_puede_dar = list_filter(ready_to_exchange, le_puede_dar);
	t_list* les_puedo_dar  = tcb_actual->les_puede_dar;


	void aumentar_nivel_de_grafo(t_tcb_entrenador* tcb_iterado) {
		tcb_iterado->nivel_de_grafo_en_deadlock = tcb_actual->nivel_de_grafo_en_deadlock + 1;
	}

	void hay_espera_circular(t_tcb_entrenador* tcb_iterado) {
		tcb_actual = tcb_iterado;
		list_add_in_index(tcbs_en_niveles_de_grafo, tcb_iterado->nivel_de_grafo_en_deadlock, tcb_iterado);
		if (tcb_iterado == tcb_que_llega) {
		 	en_deadlock = list_take(tcbs_en_niveles_de_grafo, tcb_iterado->nivel_de_grafo_en_deadlock);
		 	list_destroy(tcbs_en_niveles_de_grafo);
		 	tcb_iterado->nivel_de_grafo_en_deadlock = 0;
		 }
		 else {
				tcb_iterado->les_puede_dar = list_filter(ready_to_exchange, le_puede_dar);
				t_list* les_puedo_dar = tcb_iterado->les_puede_dar;


				 if (les_puedo_dar > 0){
					list_iterate(les_puedo_dar, (void*) aumentar_nivel_de_grafo);
				 	list_iterate(les_puedo_dar, (void*) hay_espera_circular);
				 }
				 free(les_puedo_dar);
		 }
	}

	list_add_in_index(tcbs_en_niveles_de_grafo, tcb_actual->nivel_de_grafo_en_deadlock, tcb_actual);

	list_iterate(les_puedo_dar, aumentar_nivel_de_grafo);
	list_iterate(les_puedo_dar, hay_espera_circular);

	free(les_puedo_dar);

	return en_deadlock;
}

void despachar_resolucion_de_deadlock(t_deadlock* deadlock) {
	cargar_rafaga_intercambio(deadlock->tcb_1);

	list_remove_element(ready_to_exchange, deadlock->tcb_1);
	pasar_a_ready(deadlock->tcb_1, "Entrenador va a intercambiar");
}

void loggear_deteccion_de_deadlock(t_list* lista_deadlock) {
	log_info(logger,
			"[DEADLOCK] Se detectó espera circular entre % d TCBs.",
			list_size(lista_deadlock));
}

void loggear_tcbs_para_intercambiar_deadlock(t_deadlock* deadlock) {
	log_info(logger,
			"[DEADLOCK] El entrenador TID: %d se moverá a la posición (%d,%d) del entrenador TID: %d para intercambiar los pokemones: %s-%s.",
			deadlock->tcb_1->tid,
			deadlock->tcb_2->posicion->x,
			deadlock->tcb_2->posicion->y,
			deadlock->tcb_2->tid,
			deadlock->tcb_1->pokemon_a_dar_en_intercambio,
			deadlock->tcb_2->pokemon_a_dar_en_intercambio);
}

void ejecutar_manejador_de_deadlocks(t_tcb_entrenador* tcb) {
	pthread_mutex_lock(&mutex_manejar_deadlock);

	if (deadlock_actual == NULL) {

		t_list* lista_deadlock = detectar_deadlock_recursivo(tcb);

		if(lista_deadlock != NULL) {

			void cambiar_estado_tcb(t_tcb_entrenador* tcb ){
				tcb->estado_tcb = DEADLOCK;
			}

			list_iterate(lista_deadlock, cambiar_estado_tcb);

			deadlock_actual = lista_deadlock;

			t_deadlock* deadlock = crear_deadlock(deadlock_actual);
			despachar_resolucion_de_deadlock(deadlock);


			metricas->cantidad_deadlocks_producidos++;
			loggear_deteccion_de_deadlock(lista_deadlock);
			loggear_tcbs_para_intercambiar_deadlock(deadlock);

			free(deadlock);
		}
		else {
			log_info(logger,"[DEADLOCK] No se detectó espera circular entre los TCBs");
		}

	}

	pthread_mutex_unlock(&mutex_manejar_deadlock);
}

void continuar_o_manejar_deadlock() {
	if (!team_cumplio_objetivo){
		if (list_size(deadlock_actual) == 0){
			list_destroy(deadlock_actual);
			deadlock_actual = NULL;

			metricas->cantidad_deadlocks_resueltos++;
			if (list_size(ready_to_exchange) > 0) {
				t_tcb_entrenador* siguiente_tcb = list_first(ready_to_exchange);

				ejecutar_manejador_de_deadlocks(siguiente_tcb);
			}
		} else if (list_size(deadlock_actual) == 1){
			t_tcb_entrenador* unico_tcb = list_remove(deadlock_actual, 0);

			unico_tcb->estado_tcb = READY_TO_EXCHANGE;

			list_destroy(deadlock_actual);
			deadlock_actual = NULL;

			metricas->cantidad_deadlocks_resueltos++;
			if (list_size(ready_to_exchange) > 0) {
				t_tcb_entrenador* siguiente_tcb = list_first(ready_to_exchange);

				ejecutar_manejador_de_deadlocks(siguiente_tcb);
			}
		} else {
			t_deadlock* deadlock = crear_deadlock(deadlock_actual);
			despachar_resolucion_de_deadlock(deadlock);

			loggear_tcbs_para_intercambiar_deadlock(deadlock);
			free(deadlock);
		}
	}
}

void pasar_a_ready_to_exchange(t_tcb_entrenador* tcb, char* motivo) {
	pasar_a_cola(tcb, ready_to_exchange, READY_TO_EXCHANGE, motivo);

	metricas->cantidad_cambios_contexto++;
	ejecutar_manejador_de_deadlocks(tcb);
	pthread_mutex_unlock(&mutex_planificador);
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

void destruir_metricas() {
	dictionary_clean(metricas->cantidad_ciclos_CPU_entrenador);
	dictionary_destroy(metricas->cantidad_ciclos_CPU_entrenador);

	free(metricas);
}
