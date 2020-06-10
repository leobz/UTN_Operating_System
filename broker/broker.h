/*
 * broker.h
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#ifndef BROKER_H_
#define BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/config.h"
#include "../utils/log.h"
#include "../utils/servidor.h"

typedef struct {
	char *ip_broker;
	char *puerto_broker;
	int tamanio_memoria;
	int tamanio_minimo_particion;
	char* algoritmo_memoria;
	char* algoritmo_reemplazo;
	char* algoritmo_particion_libre;
	int frecuencia_compactacion;
} t_broker_config;

typedef struct mensaje{

	op_code codigo_operacion;
	int id_mensaje;
	int id_correlativo;
	int payload_size;
	void* payload;
	struct mensaje* siguiente;

}t_mensaje;

typedef struct cola_mensaje_recibido{
	int id_mensaje;
	int confirmacion; //boolean para saber si el receptor confirmo el mensaje
	struct cola_mensaje_recibido* siguiente;
}t_cola_mensaje_recibido;

typedef struct cola_proceso{
	int socket_cliente;
	struct cola_proceso* siguiente;
}t_cola_proceso;

t_log* logger;
t_broker_config* broker_config;

void *empaquetar_mensaje_broker(t_mensaje *mensaje,int* bytes);
void parsear_broker_config(t_broker_config *broker_config, t_config *config);
void destruir_broker_config(t_broker_config *broker_config);
void inicializar_broker();
void finalizar_broker();
t_broker_config *cargar_broker_config(char *path_archivo);

#endif /* BROKER_H_ */
