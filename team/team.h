#ifndef TEAM_H_
#define TEAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include "../utils/listas.h"
#include "../utils/diccionarios.h"
#include "../utils/config.h"
#include "../utils/servidor.h"
#include "../utils/log.h"
#include <commons/collections/queue.h>


//ESTRUCTURAS
typedef t_dictionary t_objetivo_global;
typedef t_dictionary t_pokemon_requeridos;

t_objetivo_global* objetivo_global;
t_pokemon_requeridos* pokemon_requeridos;

t_list* ready;
t_list* new;
t_list* blocked;
t_list* unblocked;

t_log* logger;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef enum {
	NEW = 1,
	READY = 2,
	BLOCKED = 3,
	EXEC = 4,
	EXIT = 5,
	//estado_tcbS INTERMEDIOS
	READY_TO_EXCHANGE = 6,
} estado_tcb;


typedef enum {
	MOVERSE = 1,
	CATCH = 2,
	INTERCAMBIAR = 3,
} instruccion;


typedef struct{
	t_list* posiciones_entrenadores;
	t_list* pokemon_entrenadores;
	t_list* objetivos_entrenadores;
	int cantidad_entrenadores;
	int tiempoDeReconexion;
	int retardo_ciclo_cpu;
	char *algoritmo_de_planificacion;
	int quantum;
	int estimacion_inicial;
	char *ip_broker;
	char *puerto_broker;
	char *log_file;
}t_team_config;

t_team_config* team_config;

typedef struct{
	char* pokemon;
	t_posicion* posicion;
}t_pokemon;


typedef struct{
	pthread_t* entrenador;
	int tid;
	t_posicion* posicion;
	t_dictionary* objetivos;
	t_queue* rafaga;
	struct t_tcb_entrenador* entrenador_a_intercambiar;
	int pokemones_max;
	t_dictionary* pokemones_capturados;
	estado_tcb estado_tcb;
	t_pokemon* pokemon_a_capturar;
}t_tcb_entrenador;


// INICIALIZACIONES TEAM
void cargar_objetivo_global(t_team_config*);
void crear_pokemon_requeridos();
void agregar_pokemones_de_entrenador_a_objetivo_global(
		char** objetivos_entrenadores);
void agregar_pokemon_a_objetivo_global(char *pokemon);


int obtener_cantidad_global_por_pokemon(char* pokemon);

bool existe_pokemon_en_objetivo_global(char* pokemon);
void destruir_objetivo_global();

void agregar_pokemon_a_pokemon_requeridos(char* pokemon, t_list* lista_posiciones);
bool existe_pokemon_en_pokemon_requeridos(char* pokemon);
t_list* obtener_lista_posiciones_by_pokemon_requerido(char *pokemon);
void destruir_pokemon_requeridos();
void destruir_lista_posiciones(t_list* posiciones);
void destruir_posicion(t_posicion* posicion);

// PROCESO DE MENSAJES
void procesar_mensaje_recibido(t_paquete_socket* paquete);
void agregar_pokemon_requerido_by_mensaje_appeared(t_mensaje_appeared* mensaje);
void pasar_entrenador_a_ready_segun_cercania(t_mensaje_appeared* mensaje);


// CONFIG
t_team_config *cargar_team_config(char *path_archivo);
void parsear_team_config(t_team_config *team_config, t_config *config);
void destruir_team_config(t_team_config *team_config);
t_list* obtener_lista_de_posiciones(t_config *config, char* clave);
t_list* obtener_lista_de_pokemones(t_config *config, char* clave);

//void crear_tcb_entrenadores(char ** entrenadores);

void a_la_lista(char *pokemon_requerido);
void crear_tcb_entrenadores();

void loggear_appeared_recibido(t_mensaje_appeared* mensaje_appeared);
void imprimir_pokemon_agregado(t_mensaje_appeared* mensaje);

#endif

