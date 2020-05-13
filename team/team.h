#ifndef TEAM_H_
#define TEAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "../utils/config.h"
#include "../utils/servidor.h"


//ESTRUCTURAS
typedef t_dictionary t_objetivo_global;
typedef t_dictionary t_pokemon_requeridos;
typedef t_list t_entrenadores;

t_objetivo_global* objetivo_global;
t_pokemon_requeridos* pokemon_requeridos;
t_entrenadores* entrenadores;

typedef enum {
	REALIZAR_CATCH = 1,
} type_instruccion;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef struct{
	char **posiciones_entrenadores;
	char **pokemon_entrenadores;
	char **objetivos_entrenadores;
	int tiempoDeReconexion;
	int retardo_ciclo_cpu;
	char *algoritmo_de_planificacion;
	int quantum;
	int estimacion_inicial;
	char *ip_broker;
	char *puerto_broker;
	char *log_file;
}t_team_config;

typedef struct{
	char* pokemon;
	t_posicion* posicion;
}t_pokemon;

typedef struct{
	pthread_t* entrenador;
	int TID;
	t_posicion* posicion;
	t_dictionary* objetivos;
	type_instruccion instruccion;
	t_list* rafaga;
	t_pokemon* pokemon_a_capturar;
	struct t_tcb_entrenador* entrenador_a_intercambiar;
	int pokemones_max;
	t_dictionary* pokemones_capturados;
}t_tcb_entrenador;


// INICIALIZACIONES TEAM
void cargar_objetivo_global (void);
void crear_pokemon_requeridos();

int obtener_pokemon_a_objetivo_global(char* pokemon);
void agregar_pokemon_a_objetivo_global(char* pokemon, int cantidad);
bool existe_pokemon_en_objetivo_global(char* pokemon);
void destruir_objetivo_global();

void agregar_pokemon_a_pokemon_requeridos(char* pokemon, t_list* lista_posiciones);
bool existe_pokemon_en_pokemon_requeridos(char* pokemon);
t_list* obtener_lista_posiciones_by_pokemon_requerido(char *pokemon);
void destruir_pokemon_requeridos();
void destruir_lista_posiciones(t_list* posiciones);
void destruir_posicion(t_posicion* posicion);

// PROCESO DE MENSAJES
void procesar_mensaje_recibido(t_paquete* paquete);
void agregar_pokemon_requerido_by_mensaje_appeared(t_mensaje_appeared* mensaje);

t_team_config *cargar_team_config(char *path_archivo);
void parsear_team_config(t_team_config *team_config, t_config *config);
void destruir_team_config(t_team_config *team_config);
//void crear_tcb_entrenadores(char ** entrenadores);

#endif

