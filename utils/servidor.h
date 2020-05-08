#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<netdb.h>
#include<string.h>
#include<sys/socket.h>
#include<pthread.h>
#include "serializacion.h"

pthread_t thread;

void* recibir_buffer(int*, int);

int iniciar_servidor(char* ip, char* puerto);
void esperar_cliente(int socket_cliente, void(*procesar_mensaje_recibido)(t_paquete*));


t_paquete* recibir_mensaje_servidor(int socket_cliente);

#endif
