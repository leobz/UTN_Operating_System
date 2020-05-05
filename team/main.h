#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <pthread.h>


// ESTRUCTURAS
typedef enum {
	NEW = 1,
	READY = 2,
	BLOCKED = 3,
	EXEC = 4,
	EXIT = 5,
	// TODO: Definir estados restantes
} tcb_estado;

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
#endif /* MAIN_H_ */
