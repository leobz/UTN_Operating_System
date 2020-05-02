#ifndef SERVIDOR_H_
#define SERVIDOR_H_

//#include<stdio.h>
//#include<stdlib.h>
//#include<sys/socket.h>
//#include<unistd.h>
//#include<netdb.h>
//#include<commons/log.h>
//#include<commons/collections/list.h>
//#include<string.h>
//#include<pthread.h>

#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<string.h>
#include<sys/socket.h>
#include<pthread.h>

//typedef struct {
//	int size;
//	void* stream;
//} t_buffer;
//
//typedef struct {
//	op_code codigo_operacion;
//	t_buffer* buffer;
//} t_paquete;
//
//typedef struct {
//	int length_pokemon;
//	char* pokemon;
//	int posx;
//	int posy;
//} t_mensaje_appeared;


pthread_t thread;

void* recibir_buffer(int*, int);

void iniciar_servidor(char* ip, char* puerto);
void esperar_cliente(int);
void procesar_mensaje_recibido(int *socket);


//void* recibir_mensaje(int socket_cliente, int* size);
//void* serializar_paquete(t_paquete* paquete, int bytes);
//void devolver_mensaje(void* payload, int size, int socket_cliente);


#endif
