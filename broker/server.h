/*
 * server.h
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#ifndef SERVER_H_
#define SERVER_H_


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>


#include "../utils/serializacion.h"
#include "../utils/servidor.h"
#include "../utils/config.h"
#include "../utils/log.h"
#include "broker.h"



typedef struct mensaje_sc{

	op_code codigo_operacion;
	int id_mensaje;
	int payload_size;
	void* payload;
	struct mensaje_sc* siguiente;

}t_mensaje_sc;

typedef struct mensaje_cc{

	op_code codigo_operacion;
	int id_mensaje;
	int id_correlativo;
	int payload_size;
	void* payload;
	struct mensaje_cc* siguiente;

}t_mensaje_cc;

typedef struct cola_mensaje_recibido{
	int id_mensaje;
	struct cola_mensaje_recibido* siguiente;
}t_cola_mensaje_recibido;

typedef struct cola_mensaje_confirmado{
	int id_mensaje;
	struct cola_mensaje_confirmado* siguiente;
}t_cola_mensaje_confirmado;

typedef struct cola_proceso{
	int socket_cliente;
	t_cola_mensaje_recibido* mensaje_recibido;
	t_cola_mensaje_confirmado* mensaje_confirmado;
	struct cola_proceso* siguiente;
}t_cola_proceso;

int id_cola[8]; //este es vector de contadores para cada cola cuando les llega un nuevo mensaje

pthread_t thread;

void procesar_mensaje_recibido(t_paquete_socket* paquete);
t_mensaje_sc* preparar_mensaje_sc(t_paquete_socket* paquete, t_mensaje_sc* mensaje_a_preparar)
t_mensaje_cc* preparar_mensaje_cc(t_paquete_socket* paquete, t_mensaje_cc* mensaje_a_preparar);
void liberar_paquete(t_paquete_socket* paquete);


#endif /* SERVER_H_ */
