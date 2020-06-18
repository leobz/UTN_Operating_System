/*
 * main.h
 *
 *  Created on: May 28, 2020
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "colas.h"
#include "administracion-de-mensajes.h"

t_mensaje* mensaje[6];

t_mensaje_new *mensaje_new_recibido;
t_mensaje_get *mensaje_get_recibido;
t_mensaje_catch *mensaje_catch_recibido;
t_mensaje_appeared *mensaje_appeared_recibido;
t_mensaje_caught *mensaje_caught_recibido;
//t_broker_config* config_broker;
//t_log* log_broker;

pthread_t sem_mensajes[6];
pthread_mutex_t mutex[6];


void enviar_mensajes_en_cola(int);
#endif /* MAIN_H_ */
