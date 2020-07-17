#ifndef DIRECTORIOS_H_
#define DIRECTORIOS_H_
#include <sys/stat.h>
#include <dirent.h>
#include "gamecard.h"
#include "commons/string.h"
#include "commons/collections/list.h"
#define BIT_SIZE(x,y) ((x - 1) / y + 1)

//Estructuras

pthread_mutex_t mutex_abiertos[3];
pthread_mutex_t mutex_setear[3];
t_dictionary* archivos_existentes;
t_dictionary* archivos_abiertos;
//t_bitarray* bitmap;

//Funciones
void inicializar_directorios();
void inicializar_diccionarios();
void crear_archivo_pokemon(t_mensaje_new* mensaje_new);
int size_metadata_blocks(t_list* lista);
void procesar_new_pokemon(t_paquete_socket* paquete_socket);
void procesar_catch_pokemon(t_paquete_socket* paquete_socket);
void procesar_get_pokemon(t_paquete_socket* paquete_socket);
bool archivo_esta_abierto(char* pokemonn);
char* setear_archivo_abierto(char*pokemonn);
void cerrar_archivo(char* pokemonn);
char*formar_archivo_pokemon(char*pokemonn,bool creacion);
t_bitarray * crear_bitmap(int cant_bloques);
void actualizar_archivo_bitmap(t_bitarray * bitmap);
void setear_bloque_ocupado(int numero_bloque);
char*ruta_bitmap();
t_bitarray *leer_bitmap();
void modificar_bit(int bit,bool valor,t_bitarray*bitmap);
void crear_archivo_metadata(char *path_pokemonn,int contador_bloques);



#endif  //DIRECTORIOS_H_
