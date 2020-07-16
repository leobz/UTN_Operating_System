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
#include "gamecard.h"

typedef t_config t_bloque;

typedef struct{
	t_list* blocks;
	int size;
	char* directory;
	bool open;
}t_archivo;


/**
* @NAME: crear_bloque(char* ruta_del_archivo)
* @DESC: Abre un archivo de texto de tipo bloque, lee todas sus keys
* y las parsea a un formato t_bloque(t_config). Si no existe, lo crea
*/
t_bloque* crear_bloque(char* ruta_archivo);
char* crear_ruta(char* ruta);


t_config *cargar_bloque_desde_buffer(char* un_buffer);

char* archivo_a_string(char* ruta);
char* buffer_archivo(t_archivo* archivo);
char* ruta_blocks(char* numero_de_bloque);
t_archivo* leer_archivo(char* ruta);

#endif /* BLOQUES_H_ */
