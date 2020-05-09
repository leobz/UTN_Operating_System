/*
 * broker.h
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#ifndef BROKER_H_
#define BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/config.h"
#include "../utils/log.h"


#define IP "127.0.0.1"
#define PUERTO "4444"

typedef struct {
	char *ip_broker;
	char *puerto_broker;
} t_broker_config;


void parsear_broker_config(t_broker_config *broker_config, t_config *config);
void destruir_broker_config(t_broker_config *broker_config);
void inicializar_broker(t_broker_config **broker_config, t_log **logger);
void finalizar_broker(t_broker_config* broker_config, t_log* logger);
t_broker_config *cargar_broker_config(char *path_archivo);

#endif /* BROKER_H_ */
