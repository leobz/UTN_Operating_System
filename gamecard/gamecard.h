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
#include <pthread.h>
#include "bloques.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include <sys/stat.h>
#include <dirent.h>

// Estructuras
typedef struct {
	int tiempo_reintento_conexion;
	int tiempo_reintento_operacion;
	int tiempo_retardo_operacion;
	char* punto_montaje_tallgrass;
	char* ip_broker;
	char* puerto_broker;
	char* ip_gamecard;
	char* puerto_gamecard;
	int id_proceso;
} t_gamecard_config;

// Variables
t_log* logger;
t_gamecard_config* gamecard_config;
pthread_t suscripcion_broker;


char* path_directorio_metadata;
char* path_directorio_files;
char* path_directorio_blocks;


// Funciones
t_gamecard_config* cargar_gamecard_config(char* path_archivo);
void suscribirme_al_broker();
void destruir_gamecard_config(t_gamecard_config* gamecard_config);
void inicializar_gamecard();
void finalizar_gamecard();

#endif /* GAMECARD_H_ */
