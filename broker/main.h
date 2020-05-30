/*
 * main.h
 *
 *  Created on: May 28, 2020
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "colas.h"

t_mensaje* mensaje[6];
t_broker_config* config_broker;
t_log* log_broker;

pthread_t sem_mensajes[6];
pthread_mutex_t mutex[6];

void extraer_new_pokemon();
#endif /* MAIN_H_ */
