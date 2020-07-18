#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include "team.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include "../utils/listas.h"
#include "../utils/diccionarios.h"

pthread_t planificador;
pthread_t reintentador_de_conexion;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_tcb_exec;
pthread_mutex_t mutex_manejar_deadlock;

typedef struct {
	t_tcb_entrenador* tcb_1;
	t_tcb_entrenador* tcb_2;
} t_deadlock;

// ESTRUCTURAS

typedef enum {
	FIFO = 1,
	RR = 2,
	SJF_SD = 3,
	SJF_CD = 4,
} tipo_de_planificacion;

t_list* deadlock_actual;


// Inicializacion
void inicializar_listas();
void inicializar_diccionarios();
void iniciar_planificador();
void inicializar_metricas();
void planificar();

// Carga de TCB
void pasar_a_ready(t_tcb_entrenador* tcb, char* motivo);
void cargar_instruccion(t_tcb_entrenador* tcb, int instruccion);

void cargar_rafaga_intercambio(t_tcb_entrenador* tcb);
void cargar_rafaga_captura(t_tcb_entrenador*, t_posicion*);
void cargar_rafaga_movimiento(t_tcb_entrenador*, t_posicion*);
void cargar_rafaga_movimiento_a_entrenador(t_tcb_entrenador*, t_posicion*);

int distancia_entre(t_posicion* inicio, t_posicion* destino);
void actualizar_posicion(t_tcb_entrenador*);
void actualizar_posicion_entrenador_intercambio(t_tcb_entrenador*);


// Ejecucion de TCB
t_tcb_entrenador* siguiente_tcb_a_ejecutar();
t_tcb_entrenador* obtener_tcb_menor_proxima_estimacion();
bool tiene_menor_proxima_estimacion(t_tcb_entrenador*, t_tcb_entrenador*);
double obtener_proxima_estimacion(t_tcb_entrenador*);
void ejecutar_tcb(t_tcb_entrenador*);
void ejecutar_instruccion(int, t_tcb_entrenador*);
void ejecutar_intercambio(t_tcb_entrenador*);
void ejecutar_acciones_post_intercambio(t_tcb_entrenador*, bool);

void pasar_a_exit(t_tcb_entrenador*);
bool todos_los_entrenadores_exit();

// Mensajes
void ejecutar_catch(t_tcb_entrenador*);
char* recibir_id_correlativo(int);
void agregar_a_enviaron_catch(char*, t_tcb_entrenador*);

// Conexiones
void reintentar_conexion(int conexion);

// Acciones
void asignar_pokemon(t_tcb_entrenador*);

// Utilidades
int string_to_algoritmo_de_planificacion(char*);
char* cola_planificacion_a_string(int);
char* string_maximo_permitido(t_tcb_entrenador* tcb);
char* string_motivo_intercambio();

//Deadlock
t_list* pokemones_necesitados(t_tcb_entrenador* tcb);
t_list* pokemones_no_necesitados(t_tcb_entrenador* tcb);
t_deadlock* crear_deadlock(t_list*);
void ejecutar_manejador_de_deadlocks(t_tcb_entrenador*);
void loggear_deteccion_de_deadlock(t_list*);
void loggear_tcbs_para_intercambiar_deadlock(t_deadlock*);

void hay_espera_circular(t_tcb_entrenador* tcb_iterado);
t_list* detectar_deadlock_recursivo(t_tcb_entrenador*);
void continuar_o_manejar_deadlock();


void destruir_metricas();

#endif /* PLANIFICACION_H_ */
