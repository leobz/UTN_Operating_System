/*
 * broker.h
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#ifndef BROKER_H_
#define BROKER_H_


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<pthread.h>
#include<semaphore.h>
#include <string.h>
#include "../utils/config.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include "../utils/diccionarios.h"




typedef struct {
	char *ip_broker;
	char *puerto_broker;
	char *algoritmo_memoria;
} t_broker_config;

t_log* logger;
t_broker_config* broker_config;


typedef struct mensaje{

	op_code codigo_operacion;
	int id_mensaje;
	int id_correlativo;
	int payload_size;
	void* payload;
	struct mensaje* siguiente;

}t_mensaje;

typedef struct{
	op_code codigo_operacion;
	int id_necesario;
	int payload_size;
	void* payload;
}t_mensaje_global;


typedef struct{

	int id_proceso;
	int socket;

}t_proceso;



typedef struct particion_bs {
	int tamanio_particion;
	bool esta_libre;
	int offset;
	int size_mensaje;
	struct particion_bs* primer_hijo;
	struct particion_bs* segundo_hijo;
} t_particion_bs;

typedef struct {
	int offset;
	int tamanio_particion;
	bool esta_ocupado;
} t_particion_dinamica;

typedef struct{
	int id_mensaje;
	int id_correlativo;
	op_code codigo_operacion;
	t_particion_bs* particion_bs;
	t_particion_dinamica *particion_dinamica;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
}
t_adm_mensaje;



int socket_confirmado;


t_dictionary* administracion_por_id;
t_dictionary* administracion_por_cod;
t_dictionary* dic_suscriptores[6];
t_dictionary* subscribers;

void *empaquetar_mensaje_broker(t_mensaje *mensaje,int* bytes);
void parsear_broker_config(t_broker_config *broker_config, t_config *config);
void destruir_broker_config(t_broker_config *broker_config);
void inicializar_broker(t_broker_config **broker_config, t_log **logger);
void finalizar_broker(t_broker_config* broker_config, t_log* logger);
t_broker_config *cargar_broker_config(char *path_archivo);
int id_necesario(int id_mensaje,int id_correlativo,op_code codigo_operacion);

#endif /* BROKER_H_ */
