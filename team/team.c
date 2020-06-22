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

void cargar_objetivo_global(t_team_config* team_config) {
	objetivo_global = sum_dictionaries_values(
			team_config->objetivos_entrenadores);
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

void crear_pokemon_requeridos() {
	pokemon_requeridos = dictionary_create();
}

void agregar_pokemon_a_pokemon_requeridos(char* pokemon,
		t_list* lista_posiciones) {
	dictionary_put(pokemon_requeridos, pokemon, lista_posiciones);
}

bool existe_pokemon_en_pokemon_requeridos(char* pokemon) {
	return dictionary_has_key(pokemon_requeridos, pokemon);
}

t_list* obtener_lista_posiciones_by_pokemon_requerido(char* pokemon) {
	return dictionary_get(pokemon_requeridos, pokemon);
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
	t_list* de_la_especie_en_mapa = dictionary_get(pokemon_requeridos,
			mensaje->pokemon);

	int cantidad = list_size(de_la_especie_en_mapa);
	t_posicion* posicion = list_get(de_la_especie_en_mapa, cantidad - 1);

	printf("[AGREGADO]: %s %d %d [TOTAL]: %d\n",
			mensaje->pokemon,
			posicion->x,
			posicion->y,
			cantidad);
}

void procesar_mensaje_recibido(t_paquete_socket* paquete) {
	t_mensaje_appeared* mensaje_appeared;


	switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:
			mensaje_appeared = deserializar_mensaje_appeared_pokemon(paquete->buffer);
			loggear_appeared_recibido(mensaje_appeared);

			if (existe_pokemon_en_objetivo_global(mensaje_appeared->pokemon)){
				agregar_pokemon_requerido_by_mensaje_appeared(mensaje_appeared);
				pasar_entrenador_a_ready_segun_cercania(mensaje_appeared);
			}

			break;

		case CONFIRMACION:
				log_info(logger,"Confirmacion %d",paquete->id_mensaje);
				break;

		default:
			pthread_exit(NULL);
			break;
	}
}

void agregar_pokemon_requerido_by_mensaje_appeared(t_mensaje_appeared* mensaje) {
	t_list* lista_posiciones;

	if (existe_pokemon_en_pokemon_requeridos(mensaje->pokemon))
		lista_posiciones = obtener_lista_posiciones_by_pokemon_requerido(
				mensaje->pokemon);
	else
		lista_posiciones = list_create();

	t_posicion* posicion = (t_posicion*) malloc(sizeof(t_posicion));

	posicion->x = mensaje->posx;
	posicion->y = mensaje->posy;

	list_add(lista_posiciones, posicion);

	agregar_pokemon_a_pokemon_requeridos(mensaje->pokemon, lista_posiciones);
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

	pasar_a_ready(entrenador_cercano);
	list_remove_element(new, entrenador_cercano);

	log_info(
			logger, "[CAMBIO ENTRENADOR] (NEW -> READY) MOTIVO:CAPTURA ID_ENTRENADOR:%d POSICION:(%d,%d)",
			entrenador_cercano->tid,
			entrenador_cercano->posicion->x,
			entrenador_cercano->posicion->y);
}
