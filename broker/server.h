/*
 * server.h
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "../utils/cliente.h"

#include "administracion-de-mensajes.h"




int id_mensaje; //este es vector de contadores para cada cola cuando les llega un nuevo mensaje

int cola_paquete;

pthread_mutex_t mutex[6];
pthread_mutex_t global;
sem_t cola_vacia[6];
sem_t sem_proceso[6];
sem_t sem;


void procesar_mensaje_recibido(t_paquete_socket* paquete);
t_mensaje* preparar_mensaje(t_paquete_socket* paquete);
void enviar_confirmacion(int id,op_code confirmacion,int socket);

void verificar_cache(t_proceso* proceso);

#endif /* SERVER_H_ */
