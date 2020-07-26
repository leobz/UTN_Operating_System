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

// Variables
t_dictionary* archivos_existentes;
t_dictionary* cantidad_posiciones_pokemon;
t_dictionary* pokemon_semaphores;
pthread_t suscripcion_broker;


char* path_directorio_metadata;
char* path_directorio_files;
char* path_directorio_blocks;
int id_proceso_gamecard;



// Funciones
t_gamecard_config* cargar_gamecard_config(char* path_archivo);
void suscribirme_al_broker();
void destruir_gamecard_config(t_gamecard_config* gamecard_config);
void inicializar_gamecard();
void inicializar_diccionarios();
void finalizar_gamecard();

#endif /* GAMECARD_H_ */
