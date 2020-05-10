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

t_objetivo_global* objetivo_global;
t_pokemon_requeridos* pokemon_requeridos;

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
/*typedef struct{
	pthread entrenador;
	int estado;
	int tid;
	int[2] posicion;
	char **pokemons_a_capturar;
	char **pokemons_que_tiene;
}t_tcb_entrenador;
*/

// INICIALIZACIONES TEAM
void cargar_objetivo_global (void);
int cargar_configuracion(void);
void crear_pokemon_requeridos();

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

