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
t_dictionary* cantidad_posiciones_pokemon;
t_dictionary* pokemon_semaphores;
//t_bitarray* bitmap;

//Funciones
void inicializar_directorios();
void crear_archivo_pokemon(t_mensaje_new* mensaje_new);
int size_metadata_blocks(t_list* lista);
void procesar_new_pokemon(t_paquete_socket* paquete_socket);
void procesar_catch_pokemon(t_paquete_socket* paquete_socket);
void procesar_get_pokemon(t_paquete_socket* paquete_socket);
bool archivo_esta_abierto(char* pokemonn);
char* setear_archivo_abierto(char*pokemonn);
void cerrar_archivo(char* pokemonn);
t_bitarray * crear_bitmap(int cant_bloques);
void actualizar_archivo_bitmap(t_bitarray * bitmap);
void crear_archivo_metadata(char *path_pokemonn,int contador_bloques);
void agregar_posicion(t_mensaje_new*);
t_mensaje_appeared*obtener_mensaje_appeared(t_mensaje_new*);
void enviar_mensaje_appeared(t_mensaje_appeared*appeared);
t_posiciones*obtener_posiciones_pokemon(char*);
void checkear_archivo_abierto(char*pokemonn,op_code cola);
char* crear_pokemon_metadata(char*pokemonn);
void enviar_mensaje_localized(t_posiciones**,int cantidad_de_posiciones,int id_correlativo);
void crear_metadata_para_directorios(char*ruta_directorio);
void crear_diccionario_semaforo(char*pokemonn);


#endif  //DIRECTORIOS_H_
