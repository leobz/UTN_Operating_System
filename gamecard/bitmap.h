/*
 * bitmap.h
 *
 *  Created on: Jul 22, 2020
 *      Author: diego
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include "commons/string.h"
#include "commons/bitarray.h"
#include "../utils/config.h"
#include "../utils/listas.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include "../utils/diccionarios.h"
#define BIT_SIZE(x,y) ((x - 1) / y + 1);

typedef struct{
	int block_size;
	int blocks;
	char* magic_number;
}t_metadata;

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

t_gamecard_config* gamecard_config;

t_metadata* metadata;


/**
* @NAME: crear_ruta(char* ruta)
* @DESC: Convierte una ruta relativa a absoluta
* Ejemplo: crear_ruta("unDirectorio/unArchivo.bin") => "home/../punto-de-montaje/unDirectorio/unArchivo.bin"
*
*/
char* crear_ruta(char* ruta);

char*ruta_bitmap();
void modificar_bit(t_bitarray*bitmap,bool valor,int bit);
void actualizar_archivo_bitmap(t_bitarray*bitmap);
t_bitarray *crear_bitmap(int cant_bloques);
t_bitarray *leer_bitmap();
void setear_bloque_ocupado(int numero_bloque);
void setear_bloque_libre(int numero_bloque);
int bloque_disponible_en_bitmap();

#endif /* BITMAP_H_ */
