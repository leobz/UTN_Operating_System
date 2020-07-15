/*
 * broker.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */
#include "broker-commons.h"

void* empaquetar_mensaje_broker(t_mensaje* mensaje,int *bytes){


	*bytes=mensaje->payload_size + sizeof(int)*4;
	void* paquete_broker=malloc(*bytes);
	int offset=0;

	memcpy(paquete_broker + offset, &mensaje->codigo_operacion, sizeof(int));
		offset += sizeof(int);

	memcpy(paquete_broker + offset, &mensaje->id_mensaje, sizeof(int));
		offset += sizeof(int);

	memcpy(paquete_broker + offset, &mensaje->id_correlativo, sizeof(int));
		offset += sizeof(int);

	memcpy(paquete_broker + offset, &mensaje->payload_size, sizeof(int));
		offset += sizeof(int);

	memcpy(paquete_broker + offset, mensaje->payload, mensaje->payload_size);
		offset += mensaje->payload_size;


	return paquete_broker;
}


int id_necesario(int id_mensaje,int id_correlativo,op_code codigo_operacion){
	if((codigo_operacion>=0)&&(codigo_operacion<=2))
		return id_mensaje;
	else
		return id_correlativo;
}

void parsear_broker_config(t_broker_config *broker_config, t_config *config) {
	broker_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));

	broker_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));


	broker_config->tamanio_memoria = config_get_int_value(config, "TAMANO_MEMORIA");
	broker_config->tamanio_minimo_particion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
	broker_config->algoritmo_memoria = strdup(config_get_string_value(config, "ALGORITMO_MEMORIA"));
	broker_config->algoritmo_reemplazo = strdup(config_get_string_value(config, "ALGORITMO_REEMPLAZO"));
	broker_config->algoritmo_particion_libre = strdup(config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE"));
	broker_config->frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");

}


void destruir_broker_config(t_broker_config *broker_config) {
	free(broker_config->ip_broker);
	free(broker_config->puerto_broker);
	free(broker_config->algoritmo_memoria);
	free(broker_config->algoritmo_reemplazo);
	free(broker_config->algoritmo_particion_libre);

	free(broker_config);
}


t_broker_config *cargar_broker_config(char *path_archivo) {
	t_config *config;
	t_broker_config *broker_config;

	config = leer_config(path_archivo);
	broker_config = malloc(sizeof(t_broker_config));

	parsear_broker_config(broker_config, config);
	destruir_config(config);
	return broker_config;
}

void inicializar_broker() {
	broker_config = cargar_broker_config("broker.config");
	logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);
}

void finalizar_broker() {
	destruir_broker_config(broker_config);
	destruir_logger(logger);
}




