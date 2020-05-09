#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include "../utils/listas.h"


// ESTRUCTURAS
typedef enum {
	NEW = 1,
	READY = 2,
	BLOCKED = 3,
	EXEC = 4,
	EXIT = 5,
	//ESTADOS INTERMEDIOS
	READY_TO_EXCHANGE = 6,
} estado;

typedef enum {
	FIFO = 1,
	RR = 2,
	SJF = 3,
} tipo_de_planificacion;

typedef enum {
	MOVERSE = 1,
	CATCH = 2,
	INTERCAMBIAR = 3,
} instruccion;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef struct {
	t_posicion posicion;
	char* pokemon;
} t_pokemon;


typedef struct tcb_entrenador{
	pthread_t *entrenador;
	int tid;
	estado estado;
	t_posicion posicion;
	t_list* objetivos;
	instruccion instruccion;
	t_queue* rafaga;
	t_pokemon* pokemon_a_capturar;
	struct tcb_entrenador* entrenador_a_intercambiar;
} tcb_entrenador;

// Inicializacion
void inicializar_listas();

// Carga de TCB
void pasar_a_ready(tcb_entrenador* tcb);
void cargar_instruccion(tcb_entrenador* tcb, int instruccion);

void cargar_rafaga_intercambio(tcb_entrenador* tcb);
void cargar_rafaga_captura(tcb_entrenador*, t_posicion);
void cargar_rafaga_movimiento(tcb_entrenador*, t_posicion);

int distancia_entre(t_posicion inicio, t_posicion destino);


// Ejecucion de TCB
tcb_entrenador* siguiente_tcb_a_ejecutar();

#endif /* MAIN_H_ */
