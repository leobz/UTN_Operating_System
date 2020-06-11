/*
 * gamecard.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef GAMECARD_H_
#define GAMECARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/config.h"
#include "../utils/log.h"
#include "../utils/servidor.h"

// Estructuras
typedef struct {
	int tiempo_reintento_conexion;
	int tiempo_reintento_operacion;
	int tiempo_retardo_operacion;
	char* punto_montaje_tallgrass;
	char* ip_broker;
	char* puerto_broker;
} t_gamecard_config;

// Variables
t_log* logger;
t_gamecard_config* gamecard_config;

// Funciones
t_gamecard_config* cargar_gamecard_congif(char* path_archivo);
void destruir_gamecard_config(t_gamecard_config* gamecard_config);
void inicializar_gamecard();
void finalizar_gamecard();

#endif /* GAMECARD_H_ */
