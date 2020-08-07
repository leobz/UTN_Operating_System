#include "team.h"

// CARGAR TEAM_CONFIG
t_team_config *cargar_team_config(char *path_archivo) {
	t_config *config;
	t_team_config *team_config;

	config = leer_config(path_archivo);
	team_config = malloc(sizeof(t_team_config));

	parsear_team_config(team_config, config);
	destruir_config(config);

	return team_config;
}

void parsear_team_config(t_team_config *team_config, t_config *config) {

	char* obtener_path_logger(char* path_logger){
		if (strcmp(path_logger, "DEFAULT") == 0) {
			char* path = strdup("team.log");
			return path;
		}
		else {
			char* path = strdup(path_logger);
			return path;
		}
	}


	team_config->posiciones_entrenadores = obtener_lista_de_posiciones(config,
			"POSICIONES_ENTRENADORES");
	team_config->pokemon_entrenadores = obtener_lista_de_pokemones(config,
			"POKEMON_ENTRENADORES");
	team_config->objetivos_entrenadores = obtener_lista_de_pokemones(config,
			"OBJETIVOS_ENTRENADORES");
	team_config->cantidad_entrenadores = list_size(
			team_config->objetivos_entrenadores);
	team_config->tiempoDeReconexion = config_get_int_value(config,
			"TIEMPO_RECONEXION");
	team_config->retardo_ciclo_cpu = config_get_double_value(config,
			"RETARDO_CICLO_CPU");
	team_config->algoritmo_de_planificacion = strdup(
			config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
	team_config->quantum = config_get_int_value(config, "QUANTUM");
	team_config->alpha = config_get_double_value(config, "ALPHA");
	team_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	team_config->ip_team = strdup(
				config_get_string_value(config, "IP_TEAM"));
	team_config->estimacion_inicial = config_get_double_value(config,
			"ESTIMACION_INICIAL");
	team_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));
	team_config->log_file = strdup(config_get_string_value(config, "LOG_FILE"));
	team_config->puerto_team =  strdup(config_get_string_value(config, "PUERTO_TEAM"));
	team_config->id_proceso=config_get_int_value(config,"ID_PROCESO");
	team_config->path_logger = obtener_path_logger(config_get_string_value(config, "LOG_FILE"));
}

char** separar_por_pipe(char* string_con_pipes) {
	return string_split(string_con_pipes, "|");
}

t_posicion* strings_to_posicion(char** string_posicion) {
	t_posicion* posicion = malloc(sizeof(t_posicion));
	posicion->x = atoi(string_posicion[0]);
	posicion->y = atoi(string_posicion[1]);
	return posicion;
}

t_list* obtener_lista_de_posiciones(t_config *config, char* clave) {
	t_list* posiciones = list_create;

	char** strings_de_posiciones = config_get_array_value(config, clave);

	posiciones = strings_to_list(strings_de_posiciones);
	posiciones = list_map_free(posiciones, separar_por_pipe);
	posiciones = list_map_free(posiciones, strings_to_posicion);

	return posiciones;
}

t_list* obtener_lista_de_pokemones(t_config *config, char* clave) {
	t_list* lista_de_pokemones = list_create();

	char** strings_de_pokemones = config_get_array_value(config, clave);

	lista_de_pokemones = strings_to_list(strings_de_pokemones);
	lista_de_pokemones = list_map_free(lista_de_pokemones, separar_por_pipe);
	lista_de_pokemones = list_map_free(lista_de_pokemones,
			strings_to_dictionary);

	return lista_de_pokemones;
}

void destruir_team_config(t_team_config *team_config) {
	free(team_config->posiciones_entrenadores);
	free(team_config->pokemon_entrenadores);
	free(team_config->objetivos_entrenadores);
	free(team_config->algoritmo_de_planificacion);
	free(team_config->ip_broker);
	free(team_config->ip_team);
	free(team_config->puerto_broker);
	free(team_config->puerto_team);
	free(team_config);
}

// OBJETIVO GLOBAL

void inicializar_objetivo_global(t_team_config* team_config) {
	objetivo_global = sum_dictionaries_values(
			team_config->objetivos_entrenadores);
}

void inicializar_pokemones_atrapados(t_team_config* team_config) {
	pokemones_atrapados = sum_dictionaries_values(
			team_config->pokemon_entrenadores);
}

void inicializar_pokemones_en_mapa() {
	pokemones_en_mapa = dictionary_create();
}

void inicializar_pokemones_planificados() {
	pokemones_planificados = dictionary_create();
}

void inicializar_tcbs_enviaron_catch() {
	enviaron_catch = dictionary_create();
}

t_dictionary* inicializar_pokemones_capturados(t_team_config* team_config, int* i) {
	if (list_get(team_config->pokemon_entrenadores, *i) != NULL) {
		return list_get(team_config->pokemon_entrenadores, *i);
	}
	else {
		return dictionary_create();
	}
}

// CARGA DE TCBs

void destroy_tcb_entrenador_full(t_tcb_entrenador* tcb) {
	queue_destroy(tcb->rafaga);
	free(tcb->posicion);
	dictionary_destroy(tcb->objetivos);
	dictionary_destroy(tcb->pokemones_capturados); //
}

void destroy_all_tcbs() {
	//list_iterate(l_exit, destroy_tcb_entrenador_full);
}

void crear_tcb_entrenadores(t_team_config* team_config) {
	int cant_entrenadores = team_config->cantidad_entrenadores;

	for (int i = 0; i < cant_entrenadores; i++) {
		t_tcb_entrenador* entrenador = malloc(sizeof(t_tcb_entrenador));

		entrenador->posicion = list_get(team_config->posiciones_entrenadores,
				i);
		entrenador->objetivos = list_get(team_config->objetivos_entrenadores,
				i);
		entrenador->pokemones_capturados = inicializar_pokemones_capturados(
				team_config, &i);
		entrenador->estado_tcb = NEW;
		entrenador->rafaga = queue_create();
		entrenador->pokemones_max = sum_dictionary_values(entrenador->objetivos);
		entrenador->tid = i;
		entrenador->rafaga_anterior = 0;
		entrenador->estimacion_anterior = team_config->estimacion_inicial;
		entrenador->estimacion_remanente = team_config->estimacion_inicial;
		entrenador->necesita_nueva_estimacion = true;
		entrenador->les_puede_dar = list_create();
		entrenador->finalizo = false;

		dictionary_put(metricas->cantidad_ciclos_CPU_entrenador, pasar_a_char(entrenador->tid), 0);

		list_add(new, entrenador);
	}
}

// PROCESAR MENSAJES RECIBIDOS
int obtener_cantidad_global_por_pokemon(char* pokemon) {
	return dictionary_get(objetivo_global, pokemon);
}

bool existe_pokemon_en_objetivo_global(char* pokemon) {
	return dictionary_has_key(objetivo_global, pokemon);
}

void destruir_objetivo_global() {
	dictionary_destroy(objetivo_global);
}

void agregar_pokemon_a_mapa(char* pokemon, t_list* lista_posiciones) {
	dictionary_put(pokemones_en_mapa, pokemon, lista_posiciones);
}

bool existe_pokemon_en_mapa(char* pokemon) {
	return dictionary_has_key(pokemones_en_mapa, pokemon);
}

t_list* obtener_lista_posiciones_by_pokemon_requerido(char* pokemon) {
	return dictionary_get(pokemones_en_mapa, pokemon);
}

void destruir_posicion(t_posicion* posicion) {
	free(posicion);
}

void destruir_pokemon(t_pokemon* pokemon) {
	free(pokemon->posicion);
	free(pokemon);
}

void destruir_lista_posiciones(t_list* posiciones) {
	list_destroy_and_destroy_elements(posiciones, destruir_posicion);
}

void loggear_appeared_recibido(t_mensaje_appeared* mensaje_appeared) {
	log_info(logger, "[MSG_RECIBIDO] APPEARED_POKEMON: %s %d %d",
			mensaje_appeared->pokemon,
			mensaje_appeared->posx,
			mensaje_appeared->posy);
}

void loggear_localized_recibido(t_mensaje_localized* mensaje_localized, int id_correlativo) {
	log_info(logger, "[MSG_RECIBIDO] LOCALIZED_POKEMON: Pokemon:%s Cantidad:%d ID_Correlativo:%d",
			mensaje_localized->pokemon,
			mensaje_localized->cantidad_posiciones,
			id_correlativo);
}


void imprimir_pokemon_agregado(t_mensaje_appeared* mensaje) {
	t_list* de_la_especie_en_mapa = dictionary_get(pokemones_en_mapa,
			mensaje->pokemon);

	int cantidad = list_size(de_la_especie_en_mapa);
	t_posicion* posicion = list_get(de_la_especie_en_mapa, cantidad - 1);

	printf("[AGREGADO]: %s %d %d [TOTAL EN MAPA]: %d\n",
			mensaje->pokemon,
			posicion->x,
			posicion->y,
			cantidad);
}

bool planificacion_del_pokemon_esta_cubierta(char* pokemon) {
	int cantidad_de_especie_en_planificados = dictionary_get(pokemones_atrapados, pokemon);
	int cantidad_de_especie_en__atrapados = dictionary_get(pokemones_planificados, pokemon);

	int cantidad_de_especie_planificada_y_atrapada =
			cantidad_de_especie_en_planificados + cantidad_de_especie_en__atrapados;

	int cantidad_de_especie_en_objetivo_global =
			dictionary_get(objetivo_global, pokemon);

	return cantidad_de_especie_planificada_y_atrapada >= cantidad_de_especie_en_objetivo_global;
}

bool planificacion_del_pokemon_no_esta_cubierta(char* pokemon) {
	return !planificacion_del_pokemon_esta_cubierta(pokemon);
}

void pasar_a_ready_si_corresponde(t_mensaje_appeared* mensaje_appeared) {
	if (planificacion_del_pokemon_no_esta_cubierta(mensaje_appeared->pokemon)){
		dictionary_increment_value(pokemones_planificados, mensaje_appeared->pokemon);
		pasar_entrenador_a_ready_segun_cercania(mensaje_appeared);
	}

}


t_pokemon* obtener_pokemon_mas_cercano(t_tcb_entrenador* tcb) {
	t_posicion* posicion_tcb = tcb->posicion;
	t_pokemon* pokemon_mas_cercano = NULL;

	t_list* lista_pokemones_cercanos = list_create();

	void elegir_al_mas_cercano(char* pokemon, t_list* lista_posiciones){
		int distancia_cercana = 0;
		t_posicion* posicion_mas_cercana = NULL;

		void elegir_posicion_mas_cercana_de_la_especie(t_posicion* posicion){
			int nueva_distancia = 0;
			nueva_distancia = distancia_entre(posicion, posicion_tcb);

			if (posicion_mas_cercana == NULL){
				distancia_cercana = nueva_distancia;
				posicion_mas_cercana = posicion;
			}
			else{
				if (distancia_cercana > nueva_distancia){
					distancia_cercana = nueva_distancia;
					posicion_mas_cercana = posicion;
				}
			}
		}
		printf("Lista posiciones de especie : %s -> %d \n", pokemon, list_size(lista_posiciones));

		if(list_size(lista_posiciones) >0 ){
			list_iterate(lista_posiciones, elegir_posicion_mas_cercana_de_la_especie);

			t_pokemon* pokemon_cercano = malloc(sizeof(t_pokemon));
			pokemon_cercano->pokemon = strdup(pokemon);
			pokemon_cercano->posicion = malloc(sizeof(t_posicion));
			pokemon_cercano->posicion->x = posicion_mas_cercana->x;
			pokemon_cercano->posicion->y = posicion_mas_cercana->y;
//			pokemon_cercano->posicion = posicion_mas_cercana;

			list_add(lista_pokemones_cercanos, pokemon_cercano);
		}

	}

	dictionary_iterator(pokemones_en_mapa, elegir_al_mas_cercano);

	int posicion_mas_cercana = 0;
	int distancia_cercana = 0;

	void el_pokemon_mas_cercano(t_pokemon* pokemon){
		int nueva_distancia = 0;

		nueva_distancia = distancia_entre(pokemon->posicion, posicion_tcb);

		if (posicion_mas_cercana == NULL){
			distancia_cercana = nueva_distancia;
			pokemon_mas_cercano = pokemon;
		}
		else{
			if (distancia_cercana > nueva_distancia){
				distancia_cercana = nueva_distancia;
				pokemon_mas_cercano = pokemon;
			}
		}
	}


	printf("Tamaño de lista de pokemones cercanos : %d \n", list_size(lista_pokemones_cercanos));
	list_iterate(lista_pokemones_cercanos, el_pokemon_mas_cercano);

	list_remove_element(lista_pokemones_cercanos, pokemon_mas_cercano);
	list_destroy_and_destroy_elements(lista_pokemones_cercanos, destruir_pokemon);

	return pokemon_mas_cercano;

}

void despachar_entrenador_captura(t_tcb_entrenador* tcb, t_pokemon* pokemon) {
	// TODO:Invocar esta funcion al cargar siempre una rafaga captura, y no,
	// esperar hasta que se le asigne el pokemon mediante un caught
	t_posicion* copia_de_posicion = malloc(sizeof(t_posicion));
	copia_de_posicion->x = pokemon->posicion->x;
	copia_de_posicion->y = pokemon->posicion->y;


}

void pasar_tcb_a_ready_si_hay_pokemones_en_mapa(t_tcb_entrenador* tcb) {
	pthread_mutex_lock(&mutex_mapa);
	t_pokemon* pokemon = obtener_pokemon_mas_cercano(tcb);

	if (pokemon!= NULL) {
		printf("El pokemon mas cercano es: \n");
		printf("Pokemon: %s \n", pokemon->pokemon);
		printf("Posicion: %d, %d\n" , pokemon->posicion->x, pokemon->posicion->y);


		cargar_tcb_captura(tcb, pokemon);
		quitar_pokemon_de_mapa(tcb);

		printf("TCB %d tiene asignado un %s en posicion %d, %d\n",
				tcb->tid, tcb->pokemon_a_capturar->pokemon,
				tcb->pokemon_a_capturar->posicion->x, tcb->pokemon_a_capturar->posicion->y);


		char* motivo = string_motivo_captura(pokemon);
		pasar_a_ready(tcb, motivo);
		free(motivo);
		list_remove_element(new, tcb);
		list_remove_element(unblocked, tcb);
	}
	pthread_mutex_unlock(&mutex_mapa);

//	if (planificacion_del_pokemon_no_esta_cubierta(mensaje_appeared->pokemon)){
//		dictionary_increment_value(pokemones_planificados, mensaje_appeared->pokemon);
//		pasar_entrenador_a_ready_segun_cercania(mensaje_appeared);
//	}

}


void procesar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared) {
	pthread_mutex_lock(&mutex_mapa);
	if (existe_pokemon_en_objetivo_global(mensaje_appeared->pokemon)) {
		agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje_appeared);
		pthread_mutex_unlock(&mutex_mapa);

		printf("[TEAM]Finalizo funcion agregar_pokemon_a_mapa_by_mensaje_appeared()\n");
		pthread_mutex_lock(&mutex_lista_new);
		pasar_a_ready_si_corresponde(mensaje_appeared);
		pthread_mutex_unlock(&mutex_lista_new);
		printf("[TEAM]Finalizo funcion pasar_a_ready_si_corresponde()\n");
	}
	else {
		pthread_mutex_unlock(&mutex_mapa);
	}

	printf("[TEAM]Inicio funcion eliminar_mensaje_appeared()\n");
	printf("[TEAM]Mensaje appeared ¿es NULL?: %d\n", mensaje_appeared == NULL);
	printf("[TEAM]Pokemon de mensaje appeared ¿es NULL?: %d\n", mensaje_appeared->pokemon == NULL);
	eliminar_mensaje_appeared(mensaje_appeared);
	printf("[TEAM]Finalizo funcion eliminar_mensaje_appeared()\n");
}

void loggear_recepcion_de_caught(t_mensaje_caught* mensaje_caught, char* id_correlativo) {
	log_info(logger,
			"[MSG_RECIBIDO] CAUGHT_POKEMON: ID_Correlativo:%s Resultado:%d",
			id_correlativo, mensaje_caught->resultado);
}

void debug_loggear_recepcion_de_caught(t_mensaje_caught* mensaje_caught, char* id_correlativo) {
	log_info(logger_debug,
			"[MSG_RECIBIDO] CAUGHT_POKEMON: ID_Correlativo:%s Resultado:%d",
			id_correlativo, mensaje_caught->resultado);
}

void debug_loggear_recepcion_de_localized(t_mensaje_localized* mensaje_localized, char* id_correlativo) {
	log_info(logger_debug,
			"[MSG_RECIBIDO] LOCALIZED_POKMEON: ID_Correlativo:%s Pokemon:%s",
			id_correlativo, mensaje_localized->pokemon);
}

void quitar_pokemon_de_planificados(t_tcb_entrenador* entrenador) {
	dictionary_decrement_value(pokemones_planificados,
			entrenador->pokemon_a_capturar->pokemon);
}

void quitar_pokemon_de_mapa(t_tcb_entrenador* entrenador) {
	t_pokemon* pokemon_a_eliminar = entrenador->pokemon_a_capturar;
	t_posicion* posicion_a_eliminar;
	t_list* posiciones_de_la_especie =
			dictionary_get(pokemones_en_mapa, pokemon_a_eliminar->pokemon);

	void posicion_victima(t_posicion* posicion) {
		if (posicion->x == pokemon_a_eliminar->posicion->x){
			if (posicion->y == pokemon_a_eliminar->posicion->y){
				posicion_a_eliminar = posicion;
			}
		}

	}

	list_iterate(posiciones_de_la_especie, posicion_victima);
	list_remove_element(posiciones_de_la_especie,posicion_a_eliminar);
}

void aplicar_acciones_caught(t_tcb_entrenador* entrenador) {
	quitar_pokemon_de_planificados(entrenador);
	definir_cola_post_caught(entrenador);

	free(entrenador->pokemon_a_capturar->posicion);
	free(entrenador->pokemon_a_capturar);
	entrenador->pokemon_a_capturar = NULL;
}

void procesar_mensaje_caught(t_paquete_socket* paquete) {
	t_mensaje_caught* mensaje_caught  = deserializar_mensaje_caught_pokemon(paquete->buffer);

	char* id_correlativo = pasar_a_char(paquete->id_correlativo);

	debug_loggear_recepcion_de_caught(mensaje_caught, id_correlativo);

	if (dictionary_has_key(enviaron_catch, id_correlativo)) {
		loggear_recepcion_de_caught(mensaje_caught, id_correlativo);
		t_tcb_entrenador* entrenador = dictionary_get(enviaron_catch, id_correlativo);

		if (mensaje_caught->resultado == 1){
			confirmar_caught(entrenador);
		}
		list_remove_element(blocked, entrenador);
		aplicar_acciones_caught(entrenador);
	}
	free(id_correlativo);

}

void procesar_mensaje_localized(t_paquete_socket* paquete) {
	t_mensaje_localized* mensaje_localized = deserializar_mensaje_localized_pokemon(paquete->buffer);
	loggear_localized_recibido(mensaje_localized, paquete->id_correlativo);

	for(int i = 0; i < mensaje_localized->cantidad_posiciones ; i++) {
		t_mensaje_appeared* mensaje_appeared = malloc(sizeof(t_mensaje_appeared));
		mensaje_appeared->length_pokemon = mensaje_localized->length_pokemon;
		mensaje_appeared->pokemon = strdup(mensaje_localized->pokemon);
		mensaje_appeared->posx = mensaje_localized->pos[i].posx;
		mensaje_appeared->posy = mensaje_localized->pos[i].posy;

		procesar_mensaje_appeared(mensaje_appeared);
	}

	eliminar_mensaje_localized(mensaje_localized);
}

void destruir_datos_generados(char* id_correlativo, t_mensaje_localized* mensaje_localized) {
	eliminar_mensaje_localized(mensaje_localized);
	free(id_correlativo);
}

bool localized_tiene_id_valido(char* id_correlativo) {
	return dictionary_has_key(enviaron_get, id_correlativo);
}

int localized_es_valido(t_paquete_socket* paquete) {
	bool localized_valido = false;
	t_mensaje_localized* mensaje_localized = deserializar_mensaje_localized_pokemon(paquete->buffer);
	char* id_correlativo = string_itoa(paquete->id_correlativo);

	debug_loggear_recepcion_de_localized(mensaje_localized, id_correlativo);

	if (localized_tiene_id_valido(id_correlativo)) {
		localized_valido = strcmp(dictionary_get(enviaron_get, id_correlativo), mensaje_localized->pokemon) == 0;

		if(localized_valido)
				dictionary_remove_and_destroy(enviaron_get, id_correlativo, (void*) free);
	}

	destruir_datos_generados(id_correlativo, mensaje_localized);
	return localized_valido;
}


void confirmar_recepcion(int id_mensajee,int sockett){
	printf("Enviando confirmacion al socket %d... \n", sockett);
	int offset = 0;
	int confirmacion = CONFIRMACION;
	int id_proceso=0;
	void* a_enviar = malloc(sizeof(int)*3);

	memcpy(a_enviar, &confirmacion, sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar+offset, &id_mensajee, sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar+offset, &team_config->id_proceso, sizeof(int));

	printf("ID proceso = %d\n", team_config->id_proceso);
	enviar_mensaje(sockett, a_enviar, sizeof(int)*3);
}

void enviar_confirmacion(int id,op_code confirmacion,int socket){
	int offset=0;

	void*enviar=malloc(sizeof(int)*3);
	memcpy(enviar,&confirmacion,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&team_config->id_proceso,sizeof(int)); //valor nulo pq no es un id_proceso

	enviar_mensaje(socket,enviar,sizeof(int)*3);
	//le devuelve al proceso emisor el id del mensaje
}

void procesar_mensaje_recibido_broker(t_paquete_socket* paquete) {

	preparar_confirmacion(paquete->id_mensaje);

	switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:;
			t_mensaje_appeared* mensaje_appeared = deserializar_mensaje_appeared_pokemon(paquete->buffer);
			loggear_appeared_recibido(mensaje_appeared);
			procesar_mensaje_appeared(mensaje_appeared);
			break;

		case CAUGHT_POKEMON:
			procesar_mensaje_caught(paquete);
			break;

		case LOCALIZED_POKEMON:

			if (localized_es_valido(paquete))
				procesar_mensaje_localized(paquete);
			break;

		default:
			pthread_exit(NULL);
			break;
	}
	liberar_paquete_socket(paquete);
}

void procesar_mensaje_recibido_cliente(t_paquete_socket* paquete) {

	confirmar_recepcion(paquete->id_mensaje,paquete->socket_cliente);

	switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:;
			t_mensaje_appeared* mensaje_appeared = deserializar_mensaje_appeared_pokemon(paquete->buffer);
			liberar_paquete_socket(paquete);
			loggear_appeared_recibido(mensaje_appeared);
			procesar_mensaje_appeared(mensaje_appeared);
			break;

		case CAUGHT_POKEMON:
			procesar_mensaje_caught(paquete);
			liberar_paquete_socket(paquete);
			break;

		case LOCALIZED_POKEMON:

			if (localized_es_valido(paquete))
				procesar_mensaje_localized(paquete);
				liberar_paquete_socket(paquete);
			break;

		default:
			pthread_exit(NULL);
			break;
	}
}

void preparar_confirmacion(int id_men){

	int conexion_corfirmacion = crear_conexion(team_config->ip_broker,team_config->puerto_broker);
	enviar_confirmacion(id_men,CONFIRMACION,conexion_corfirmacion);
	liberar_conexion(conexion_corfirmacion);
}


char* recibir_id_mensaje(int conexion, char* pokemon, int codigo_de_operacion) {
	t_paquete_socket* paquete =  recibir_mensajes(conexion);

	if (paquete->codigo_operacion == CONFIRMACION) {
		int length = snprintf( NULL, 0, "%d", paquete->id_mensaje);
		char* id_mensaje_char = malloc( length + 1 );
		snprintf(id_mensaje_char, length + 1, "%d", paquete->id_mensaje);
		log_info(logger, "[MSG_RECIBIDO] CONFIRMACION: %s Pokemon:%s ID_Correlativo:%s",
				op_code_to_string(codigo_de_operacion), pokemon, id_mensaje_char);
		return id_mensaje_char;
	}
	else
		return NULL;
}

void agregar_a_enviaron_get(char* id_mensaje, char* pokemon) {
	if (id_mensaje != NULL){
		dictionary_put(enviaron_get, id_mensaje, strdup(pokemon));
	}
}

void enviar_get_pokemon() {

	void enviar_get(char* key_pokemon, void* value) {
		int conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);

		if (conexion != -1){

			int bytes;
			void *a_enviar = serializar_get_pokemon(&bytes, key_pokemon, 0, 0);
			enviar_mensaje(conexion, a_enviar, bytes);
			char* id_mensaje = recibir_id_mensaje(conexion, key_pokemon, GET_POKEMON);
			agregar_a_enviaron_get(id_mensaje, key_pokemon);

			liberar_conexion(conexion);
		}
		else {
			log_error(logger, "[CONEXION] No se pudo enviar msg GET_POKEMON: %s, se realizará acción por default", key_pokemon);
		}
	}

	sem_wait(&sem_get);
	dictionary_iterator(objetivo_global, (void*) enviar_get);
}

void agregar_pokemon_a_mapa_by_mensaje_appeared(t_mensaje_appeared* mensaje) {
	t_list* lista_posiciones;

	if (existe_pokemon_en_mapa(mensaje->pokemon))
		lista_posiciones = obtener_lista_posiciones_by_pokemon_requerido(mensaje->pokemon);
	else
		lista_posiciones = list_create();

	t_posicion* posicion = (t_posicion*) malloc(sizeof(t_posicion));

	posicion->x = mensaje->posx;
	posicion->y = mensaje->posy;

	list_add(lista_posiciones, posicion);

	agregar_pokemon_a_mapa(mensaje->pokemon, lista_posiciones);
	imprimir_pokemon_agregado(mensaje);
}

t_tcb_entrenador* elegir_entrenador_cercano_a_posicion(
		t_tcb_entrenador* tcb_1, t_tcb_entrenador* tcb_2, t_posicion* posicion){

		int distancia_tcb_1;
		int distancia_tcb_2;

		if (tcb_1 != NULL && tcb_2 != NULL) {
			distancia_tcb_1 = distancia_entre(tcb_1->posicion, posicion);
			distancia_tcb_2 = distancia_entre(tcb_2->posicion, posicion);

			if (distancia_tcb_1 < distancia_tcb_2) {
				return tcb_1;
			}
			else {
				return tcb_2;
			}
		}

		if(tcb_1 != NULL && tcb_2 == NULL) {
			return tcb_1;
		}

		if (tcb_2 != NULL && tcb_1 == NULL) {
			return tcb_2;
		}

		if (tcb_2 == NULL && tcb_1 == NULL) {
			printf("ERROR, NO HAY TCBS DISPONIBLES ACTUALMENTE PARA IR A LA POSICION (%d, %d)\n", posicion->x, posicion->y);
			return NULL;
		}
}

char* string_motivo_captura(t_pokemon* pokemon) {
	int length = strlen("CAPTURA de (,)") + 30;
	char* string_motivo_captura = malloc(length);
	snprintf(
			string_motivo_captura, length, "CAPTURA de %s (%d,%d)",
			pokemon->pokemon, pokemon->posicion->x, pokemon->posicion->y
			);
	return string_motivo_captura;
}

void pasar_entrenador_a_ready_segun_cercania(t_mensaje_appeared* mensaje){
	int distancia_cercana = 0;
	t_tcb_entrenador* entrenador_cercano_ready = NULL;
	t_tcb_entrenador* entrenador_cercano_unblocked = NULL;


	t_posicion* posicion_pokemon = (t_posicion*) malloc(sizeof(t_posicion));
	posicion_pokemon->x = mensaje->posx;
	posicion_pokemon->y = mensaje->posy;

	void elegir_entrenador_cercano_ready(t_tcb_entrenador* entrenador){
		int nueva_distancia = 0;

		nueva_distancia = distancia_entre(entrenador->posicion, posicion_pokemon);

		if (entrenador_cercano_ready == NULL){
			distancia_cercana = nueva_distancia;
			entrenador_cercano_ready = entrenador;
		}
		else{
			if (distancia_cercana > nueva_distancia){
				distancia_cercana = nueva_distancia;
				entrenador_cercano_ready = entrenador;
			}
		}
	}

	void elegir_entrenador_cercano_unblocked(t_tcb_entrenador* entrenador){
			int nueva_distancia = 0;

			nueva_distancia = distancia_entre(entrenador->posicion, posicion_pokemon);

			if (entrenador_cercano_unblocked == NULL){
				distancia_cercana = nueva_distancia;
				entrenador_cercano_unblocked = entrenador;
			}
			else{
				if (distancia_cercana > nueva_distancia){
					distancia_cercana = nueva_distancia;
					entrenador_cercano_unblocked = entrenador;
				}
			}
	}

	list_iterate(unblocked, elegir_entrenador_cercano_unblocked);
	list_iterate(new, elegir_entrenador_cercano_ready);

	t_pokemon* pokemon = malloc(sizeof(t_pokemon));
	pokemon->pokemon = strdup(mensaje->pokemon);
	pokemon->posicion = posicion_pokemon;

	t_tcb_entrenador* entrenador_cercano =
			elegir_entrenador_cercano_a_posicion(entrenador_cercano_ready, entrenador_cercano_unblocked, posicion_pokemon);

	if (entrenador_cercano != NULL) {
		cargar_tcb_captura(entrenador_cercano, pokemon);
		quitar_pokemon_de_mapa(entrenador_cercano);
		char* motivo_captura = string_motivo_captura(pokemon);
		pasar_a_ready(entrenador_cercano, motivo_captura);
		free(motivo_captura);
		list_remove_element(new, entrenador_cercano);
		list_remove_element(unblocked, entrenador_cercano);
	}
	else{
	}

}
