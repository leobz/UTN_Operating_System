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

typedef struct{
  t_list* blocks;
  int size;
  char* directory;
  bool open;
}t_archivo;

pthread_mutex_t mutex_abiertos[3];
t_metadata* metadata;
t_dictionary* archivos_existentes;
t_dictionary* archivos_abiertos;

//Funciones
void inicializar_directorios();
void inicializar_diccionarios();
void crear_archivo_pokemon(t_mensaje_new* mensaje_new);
int size_metadata_blocks(t_list* lista);
void procesar_new_pokemon(t_paquete_socket* paquete_socket);
void procesar_catch_pokemon(t_paquete_socket* paquete_socket);
void procesar_get_pokemon(t_paquete_socket* paquete_socket);
char* crear_ruta(char* ruta);

#endif  //DIRECTORIOS_H_
