#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/config.h>
#include "../utils/config.h"

//ESTRUCTURAS

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
//CONFIGURACION
void cargar_objetivo_global (void);
int cargar_configuracion(void);
t_team_config *cargar_team_config(char *path_archivo);
void parsear_team_config(t_team_config *team_config, t_config *config);
void destruir_team_config(t_team_config *team_config);
//void crear_tcb_entrenadores(char ** entrenadores);
