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
#include "serializacion.h"

#define IP "127.0.0.1"
#define PUERTO "4444"


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
