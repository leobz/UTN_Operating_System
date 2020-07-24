/*
 * gamecard.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include "gamecard.h"

void parsear_gamecard_config(t_gamecard_config* gamecard_config, t_config *config) {
	gamecard_config->tiempo_reintento_conexion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	gamecard_config->tiempo_reintento_operacion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	gamecard_config->tiempo_retardo_operacion = config_get_int_value(config, "TIEMPO_RETARDO_OPERACION");
	gamecard_config->punto_montaje_tallgrass = strdup(config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS"));
	gamecard_config->ip_broker = strdup(config_get_string_value(config, "IP_BROKER"));
	gamecard_config->puerto_broker = strdup(config_get_string_value(config, "PUERTO_BROKER"));
	gamecard_config->ip_gamecard = strdup(config_get_string_value(config, "IP_GAMECARD"));
	gamecard_config->puerto_gamecard = strdup(config_get_string_value(config, "PUERTO_GAMECARD"));
	gamecard_config->id_proceso = config_get_int_value(config, "ID_PROCESO");
}


void destruir_gamecard_config(t_gamecard_config* gamecard_config) {
	free(gamecard_config->punto_montaje_tallgrass);
	free(gamecard_config->ip_broker);
	free(gamecard_config->puerto_broker);
	free(gamecard_config->ip_gamecard);
	free(gamecard_config->puerto_gamecard);
	free(gamecard_config);
}

t_gamecard_config* cargar_gamecard_config(char* path_archivo){
	t_config* config;
	t_gamecard_config* gamecard_config;

	config = leer_config(path_archivo);
	gamecard_config = malloc(sizeof(t_gamecard_config));

	parsear_gamecard_config(gamecard_config, config);
	destruir_config(config);

	return gamecard_config;
}

void inicializar_gamecard() {
	gamecard_config = cargar_gamecard_config("gamecard.config");
	logger = iniciar_logger("gamecard.log", "gamecard", LOG_LEVEL_INFO);
}
void inicializar_diccionarios(){
archivos_existentes = dictionary_create();
cantidad_posiciones_pokemon=dictionary_create();
pokemon_semaphores=dictionary_create();
}


void liberar_paths(){
	free(path_directorio_metadata);
	free(path_directorio_files);
	free(path_directorio_blocks);
}

void finalizar_gamecard() {
	liberar_paths();
	destruir_gamecard_config(gamecard_config);
	destruir_logger(logger);
}



/////////////////////////BLOQUES/////////////////////////////////////////////////////


