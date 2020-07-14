#ifndef DIRECTORIOS_H_
#define DIRECTORIOS_H_
#include <sys/stat.h>
#include <dirent.h>
#include "gamecard.h"
#include "commons/string.h"
#include "commons/collections/list.h"
//Estructuras
typedef struct{
	int block_size;
	t_list* blocks;
	char* magic_number;
}t_file_metadata;

typedef struct{
	int block_size;
	int blocks;
	char* magic_number;
}t_metadata;

//Variables
char* path_directorio_metadata;
char* path_directorio_files;
char* path_directorio_blocks;

//Funciones
void inicializar_directorios();
int size_metadata_blocks(t_list* lista);
void procesar_new_pokemon(t_paquete_socket* paquete_socket);

#endif  //DIRECTORIOS_H_
