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
void enviar_mensaje(char* mensaje, int socket_cliente);
char* recibir_mensaje(int socket_cliente);
void liberar_conexion(int socket_cliente);

#endif
