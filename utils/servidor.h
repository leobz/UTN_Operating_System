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


int iniciar_servidor(char* ip, char* puerto);
void esperar_cliente(int socket_cliente, void(*procesar_mensaje_recibido)(t_paquete_socket*));
t_paquete_socket* recibir_mensaje_servidor(int socket_cliente);

#endif
