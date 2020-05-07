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

#include"colas.h"
#include "../utils/serializacion.h"
#include "../utils/config.h"
#include "../utils/log.h"

#define IP "127.0.0.1"
#define PUERTO "5009"


typedef struct mensaje_sc{

	op_code codigo_operacion;
	int id_mensaje;
	void* payload;
	struct mensaje_sc* siguiente;

}t_mensaje_sc;

typedef struct mensaje_cc{

	op_code codigo_operacion;
	int id_mensaje;
	int id_correlativo;
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

void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void devolver_mensaje(void* payload, int size, int socket_cliente);

#endif /* SERVER_H_ */
