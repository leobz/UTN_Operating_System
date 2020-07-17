#ifndef BLOQUES_H_
#define BLOQUES_H_

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include "commons/string.h"
#include "../utils/config.h"
#include "../utils/listas.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include "../utils/diccionarios.h"
//#include "gamecard.h"


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


typedef t_config t_bloque;

typedef struct{
	t_list* blocks;
	int size;
	char* directory;
	bool open;
}t_archivo;

t_gamecard_config* gamecard_config;

/**
* @NAME: crear_ruta(char* ruta)
* @DESC: Convierte una ruta relativa a absoluta
* Ejemplo: crear_ruta("unDirectorio/unArchivo.bin") => "home/../punto-de-montaje/unDirectorio/unArchivo.bin"
*
*/
char* crear_ruta(char* ruta);


/**
* @NAME: ruta_blocks(char* numero_de_bloque)
* @DESC: Retorna relativa a absoluta de un numero de block
* Ejemplo: ruta_blocks("1") => "home/../punto-de-montaje/Blocks/1.bin"
*/
char* ruta_blocks(char* numero_de_bloque);


/**
* @NAME: crear_bloque(char* ruta_del_archivo)
* @DESC: Abre un archivo de texto de tipo bloque, lee todas sus keys
* y las parsea a un formato t_bloque(t_config). Si no existe, lo crea
*/
t_bloque* crear_bloque(char* ruta_archivo);


/**
* @NAME: buffer_del_archivo_completo(t_archivo* archivo)
* @DESC: Dado un t_archivo, busca el contenido de cada bloque y los une en un solo char*
* Ejemplo: {1.bin="hola", 2.bin=" chau"} ==> "hola chau"
*/
char* buffer_del_archivo_completo(t_archivo* archivo);


/**
* @NAME: leer_archivo(char* ruta)
* @DESC: Crea un struct t_archivo desde un Metadata.bin de un POKEMON (No sirve para el del FS)
* Ejemplo: leer_archivo_de_datos("Files/Pikachu/Metadata.bin") => t_archivo.size = 12, t_archivo.blocks =....
*/
t_archivo* leer_archivo_de_datos(char* ruta_al_metadata_bin_del_archivo);


/**
* @NAME: archivo_a_string(char* ruta)
* @DESC: Convierte a string un archivo
*/
char* archivo_a_string(char* ruta);

/**
* @NAME: cargar_bloque_desde_buffer(char* un_buffer);
* @DESC: Crea un t_config/t_bloque nuevo desde un char* en vez de desde un archivo
*/
t_config *cargar_bloque_desde_buffer(char* un_buffer);



#endif /* BLOQUES_H_ */
