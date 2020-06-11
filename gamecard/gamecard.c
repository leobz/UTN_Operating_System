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
	gamecard_config->punto_montaje_tallgrass = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	gamecard_config->ip_broker = strdup((config_get_string_value(config, "IP_BROKER")));
	gamecard_config->puerto_broker = strdup(config_get_string_value(config, "PUERTO_BROKER"));
}


void destruir_gamecard_config(t_gamecard_config* gamecard_config) {
	free(gamecard_config->punto_montaje_tallgrass);
	free(gamecard_config->ip_broker);
	free(gamecard_config->puerto_broker);
	free(gamecard_config);
}

t_gamecard_config* cargar_gamecard_congif(char* path_archivo){
	t_config* config;
	t_gamecard_config* gamecard_config;

	config = leer_config(path_archivo);
	gamecard_config = malloc(sizeof(t_gamecard_config));

	parsear_gamecard_config(gamecard_config, config);
	destruir_config(config);

	return gamecard_config;
}

void inicializar_gamecard() {
	gamecard_config = cargar_gamecard_congif("gamecard.config");
	logger = iniciar_logger("gamecard.log", "gamecard", LOG_LEVEL_INFO);
}

void finalizar_gamecard() {
	destruir_gamecard_config(gamecard_config);
	destruir_logger(logger);
}
