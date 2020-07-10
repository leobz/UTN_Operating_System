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


pthread_t planificador;
pthread_t reintentador_de_conexion;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_tcb_exec;
pthread_mutex_t mutex_manejar_deadlock;

typedef struct {
	t_tcb_entrenador* tcb1;
	t_tcb_entrenador* tcb2;
} t_deadlock;

// ESTRUCTURAS

typedef enum {
	FIFO = 1,
	RR = 2,
	SJF_SD = 3,
	SJF_CD = 4,
} tipo_de_planificacion;


// Inicializacion
void inicializar_listas();
void inicializar_diccionarios();
void iniciar_planificador();
void planificar();

// Carga de TCB
void pasar_a_ready(t_tcb_entrenador* tcb, char* motivo);
void cargar_instruccion(t_tcb_entrenador* tcb, int instruccion);

void cargar_rafaga_intercambio(t_tcb_entrenador* tcb);
void cargar_rafaga_captura(t_tcb_entrenador*, t_posicion*);
void cargar_rafaga_movimiento(t_tcb_entrenador*, t_posicion*);

int distancia_entre(t_posicion* inicio, t_posicion* destino);


// Ejecucion de TCB
t_tcb_entrenador* siguiente_tcb_a_ejecutar();
void ejecutar_tcb(t_tcb_entrenador*);
void ejecutar_instruccion(int, t_tcb_entrenador*);

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
#endif /* PLANIFICACION_H_ */
