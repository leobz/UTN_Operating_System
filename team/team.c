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
	team_config->retardo_ciclo_cpu = config_get_int_value(config,
			"RETARDO_CICLO_CPU");
	team_config->algoritmo_de_planificacion = strdup(
			config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
	team_config->quantum = config_get_int_value(config, "QUANTUM");
	team_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	team_config->estimacion_inicial = config_get_int_value(config,
			"ESTIMACION_INICIAL");
	team_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));
	team_config->log_file = strdup(config_get_string_value(config, "LOG_FILE"));
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
	free(team_config->log_file);
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



// CARGA DE TCBs

void crear_tcb_entrenadores(t_team_config* team_config) {
	int cant_entrenadores = team_config->cantidad_entrenadores;

	for (int i = 0; i < cant_entrenadores; i++) {
		t_tcb_entrenador* entrenador = malloc(sizeof(t_tcb_entrenador));
		t_posicion* posicion = malloc(sizeof(t_posicion));
		t_dictionary* objetivo = dictionary_create();
		t_dictionary* pokemones_capturados = dictionary_create();

		entrenador->posicion = list_get(team_config->posiciones_entrenadores,
				i);
		entrenador->objetivos = list_get(team_config->objetivos_entrenadores,
				i);
		entrenador->pokemones_capturados = list_get(
				team_config->pokemon_entrenadores, i);
		entrenador->estado_tcb = NEW;
		entrenador->rafaga = queue_create();
		entrenador->pokemones_max = sum_dictionary_values(entrenador->objetivos);
		entrenador->tid = i;

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

void inicializar_pokemones_en_mapa() {
	pokemones_en_mapa = dictionary_create();
}

void agregar_pokemon_a_mapa(char* pokemon,
		t_list* lista_posiciones) {
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

void destruir_lista_posiciones(t_list* posiciones) {
	list_destroy_and_destroy_elements(posiciones, destruir_posicion);
}

void loggear_appeared_recibido(t_mensaje_appeared* mensaje_appeared) {
	log_info(logger, "[MSG_RECIBIDO] APPEARED_POKEMON: %s %d %d",
			mensaje_appeared->pokemon,
			mensaje_appeared->posx,
			mensaje_appeared->posy);
}

void imprimir_pokemon_agregado(t_mensaje_appeared* mensaje) {
	t_list* de_la_especie_en_mapa = dictionary_get(pokemones_en_mapa,
			mensaje->pokemon);

	int cantidad = list_size(de_la_especie_en_mapa);
	t_posicion* posicion = list_get(de_la_especie_en_mapa, cantidad - 1);

	printf("[AGREGADO]: %s %d %d [TOTAL]: %d\n",
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
	if (planificacion_del_pokemon_no_esta_cubierta(mensaje_appeared->pokemon))
		dictionary_increment_value(pokemones_planificados, mensaje_appeared->pokemon);
		pasar_entrenador_a_ready_segun_cercania(mensaje_appeared);

}

void procesar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared, t_paquete_socket* paquete) {
	mensaje_appeared = deserializar_mensaje_appeared_pokemon(paquete->buffer);
	loggear_appeared_recibido(mensaje_appeared);

	if (existe_pokemon_en_objetivo_global(mensaje_appeared->pokemon)) {
		agregar_pokemon_a_mapa_by_mensaje_appeared(mensaje_appeared);
		pasar_a_ready_si_corresponde(mensaje_appeared);
	}
}

void procesar_mensaje_caught(t_paquete_socket* paquete) {
	t_mensaje_caught* mensaje_caught  = deserializar_mensaje_caught_pokemon(paquete->buffer);

	char* id_correlativo = pasar_a_char(mensaje_caught->id_correlativo);
//
//	if (dictionary_has_key(enviaron_catch, id_correlativo)) {
//		log_info(logger, "[MSG_RECIBIDO] CAUGHT_POKEMON: id_correlativo:%d resultado:%d",
//					mensaje_caught->id_correlativo,
//					mensaje_caught->resultado);
//
//		t_tcb_entrenador* entrenador = dictionary_get(enviaron_catch, id_correlativo);
//
//		switch (mensaje_caught->resultado){
//		case 0:
//			pasar_a_unblocked(entrenador);
//			objetivo_global;
//			entrenador->pokemon_a_capturar = NULL;
//			break;
//		case 1:
//			break;
//		}
//		// si se confirma => asignar pokemon => pasar a cola correspondiente
//		// si no se asigna => pasar a cola correspondiente
//	}
//	else{
//		log_info(logger, "[MSG_RECIBIDO] CAUGHT_POKEMON: id_correlativo:%d ignorado", mensaje_caught->id_correlativo);
//	}
	// TODO: Fijarse si se tiene que loggear cualquier caught, sea de id correlativo correcto o no
	free(id_correlativo);

}

void procesar_mensaje_recibido(t_paquete_socket* paquete) {
	t_mensaje_appeared* mensaje_appeared;


	switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:
			procesar_mensaje_appeared(mensaje_appeared, paquete);
			break;

		case CONFIRMACION:
			log_info(logger,"Confirmacion %d",paquete->id_mensaje);
			break;

		case CAUGHT_POKEMON:
			procesar_mensaje_caught(paquete);
			break;

		default:
			pthread_exit(NULL);
			break;
	}
}

void agregar_pokemon_a_mapa_by_mensaje_appeared(t_mensaje_appeared* mensaje) {
	t_list* lista_posiciones;

	if (existe_pokemon_en_mapa(mensaje->pokemon))
		lista_posiciones = obtener_lista_posiciones_by_pokemon_requerido(
				mensaje->pokemon);
	else
		lista_posiciones = list_create();

	t_posicion* posicion = (t_posicion*) malloc(sizeof(t_posicion));

	posicion->x = mensaje->posx;
	posicion->y = mensaje->posy;

	list_add(lista_posiciones, posicion);

	agregar_pokemon_a_mapa(mensaje->pokemon, lista_posiciones);
	imprimir_pokemon_agregado(mensaje);
}

void pasar_entrenador_a_ready_segun_cercania(t_mensaje_appeared* mensaje){
	int distancia_cercana = 0;
	t_tcb_entrenador* entrenador_cercano = NULL;

	t_posicion* posicion_pokemon = (t_posicion*) malloc(sizeof(t_posicion));
	posicion_pokemon->x = mensaje->posx;
	posicion_pokemon->y = mensaje->posy;

	void elegir_entrenador_cercano(t_tcb_entrenador* entrenador){
		int nueva_distancia = 0;

		nueva_distancia = distancia_entre(entrenador->posicion, posicion_pokemon);

		if (entrenador_cercano == NULL){
			distancia_cercana = nueva_distancia;
			entrenador_cercano = entrenador;
		}
		else{
			if (distancia_cercana > nueva_distancia){
				distancia_cercana = nueva_distancia;
				entrenador_cercano = entrenador;
			}
		}
	}

	list_iterate(new, elegir_entrenador_cercano);

	t_pokemon* pokemon = malloc(sizeof(t_pokemon));
	pokemon->pokemon = mensaje->pokemon;
	pokemon->posicion = posicion_pokemon;

	cargar_tcb_captura(entrenador_cercano, pokemon);

	pasar_a_ready(entrenador_cercano, "CAPTURA");
	list_remove_element(new, entrenador_cercano);
}
