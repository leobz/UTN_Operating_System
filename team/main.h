#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include "../utils/listas.h"


// ESTRUCTURAS
typedef enum {
	NEW = 1,
	READY = 2,
	BLOCKED = 3,
	EXEC = 4,
	EXIT = 5,
} tcb_estado;

typedef enum {
	FIFO = 1,
	RR = 2,
	SJF = 3,
} tipo_de_planificacion;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef struct {
	pthread_t *entrenador;
	int tid;
	tcb_estado estado;
	t_posicion posicion;
	// TODO: Agregar pokemons objetivo
} tcb_entrenador;

void inicializar_listas();
void pasar_a_ready(tcb_entrenador* tcb);
tcb_entrenador* siguiente_tcb_a_ejecutar();
#endif /* MAIN_H_ */
