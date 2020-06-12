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
} t_broker_config;


typedef struct mensaje{

	op_code codigo_operacion;
	int id_mensaje;
	int id_correlativo;
	int payload_size;
	void* payload;
	struct mensaje* siguiente;

}t_mensaje;

typedef struct{

	int id_proceso;
	int socket;

}t_proceso;

typedef struct{
	int id_mensaje;
	int id_correlativo;
	op_code tipo_mensaje;
	//t_particion* particion;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
}
t_adm_mensaje;

t_adm_mensaje* administrador_confirmado;
int socket_confirmado;
t_proceso* proceso_confirmado;

t_dictionary* administracion_por_id;
t_dictionary* administracion_por_cod;
t_dictionary* dic_suscriptores;

void *empaquetar_mensaje_broker(t_mensaje *mensaje,int* bytes);
void parsear_broker_config(t_broker_config *broker_config, t_config *config);
void destruir_broker_config(t_broker_config *broker_config);
void inicializar_broker(t_broker_config **broker_config, t_log **logger);
void finalizar_broker(t_broker_config* broker_config, t_log* logger);
t_broker_config *cargar_broker_config(char *path_archivo);
int id_necesario(int id_mensaje,int id_correlativo,op_code codigo_operacion);

#endif /* BROKER_H_ */
