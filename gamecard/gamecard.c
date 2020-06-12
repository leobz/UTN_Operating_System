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
	gamecard_config->ip_gamecard = strdup((config_get_string_value(config, "IP_GAMECARD")));
	gamecard_config->puerto_gamecard = strdup(config_get_string_value(config, "PUERTO_GAMECARD"));
}


void destruir_gamecard_config(t_gamecard_config* gamecard_config) {
	free(gamecard_config->punto_montaje_tallgrass);
	free(gamecard_config->ip_broker);
	free(gamecard_config->puerto_broker);
	free(gamecard_config->ip_gamecard);
	free(gamecard_config->puerto_broker);
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

void finalizar_gamecard() {
	destruir_gamecard_config(gamecard_config);
	destruir_logger(logger);
}

void suscribirme_al_broker(t_gamecard_config* gamecard_config){
	printf("Suscribiendome al broker...\n");


	int colas_a_suscribir[] = {NEW_POKEMON, GET_POKEMON, CATCH_POKEMON};
	int i;
	for (i=0; i<(&colas_a_suscribir)[1]-colas_a_suscribir; i++){
		int conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
		if (conexion == -1){
			printf("ERROR: Conexion con [Broker] no establecida\n");
			exit(-1);
		}

		t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
		suscripcion->cod_operacion = SUSCRIPCION;
		suscripcion->cola_a_suscribir = colas_a_suscribir[i];

		void* a_enviar = suscripcion;
		printf("Enviando mensaje de cola %d...\n", suscripcion->cola_a_suscribir);
		enviar_mensaje(conexion, a_enviar, sizeof(int) * 2);

		liberar_conexion(conexion);
	}
}
