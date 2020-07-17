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
#include <math.h>
#include "commons/string.h"
#include "../utils/config.h"
#include "../utils/listas.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include "gamecard.h"
#include "directorios.h"


typedef t_config t_bloque;
int numero_de_bloque_disponible; // Esto se usa para un mock, borrar cuando no se use mas

typedef struct{
	t_list* blocks;
	int size;
	char* directory;
	bool open;
}t_archivo;


/**
* @NAME: crear_ruta(char* ruta)
* @DESC: Convierte una ruta relativa a absoluta
* Ejemplo: crear_ruta("unDirectorio/unArchivo.bin") => "home/../punto-de-montaje/unDirectorio/unArchivo.bin"
*
*/
char* crear_ruta(char* ruta);


/**
* @NAME: ruta_blocks(char* numero_de_bloque)
* @DESC: Retorna la ruta absoluta de un numero de block
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
* @NAME: buffer_
* del_archivo_completo(t_archivo* archivo)
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

char* config_save_to_buffer(t_config *self) ;
int cantidad_de_bloques_necesarios(int size_buffer_de_guardado) ;
int escribir_buffer_en_bloque(char* buffer, char*  numero_de_bloque);
char* obtener_numero_de_bloque_disponible();
int escribir_archivo(t_archivo* archivo, char* buffer_de_guardado);

#endif /* BLOQUES_H_ */
