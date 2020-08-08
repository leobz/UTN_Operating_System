#ifndef CLIENTE_H_
#define CLIENTE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include "serializacion.h"



int crear_conexion(char* ip, char* puerto);

void enviar_mensaje(int socket_cliente, void* a_enviar, int bytes);
void enviar_mensaje_nofree(int socket_cliente, void* a_enviar, int bytes);
int enviar_mensaje_con_retorno_eliminando(int socket_cliente, void* a_enviar, int bytes);
int enviar_mensaje_con_retorno(int socket_cliente, void* a_enviar, int bytes);

int recibir_mensaje(int socket_cliente, t_buffer *buffer_recibido);


void liberar_conexion(int socket_cliente);

#endif
