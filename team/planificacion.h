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

// ESTRUCTURAS

typedef enum {
	FIFO = 1,
	RR = 2,
	SJF = 3,
} tipo_de_planificacion;

// Inicializacion
void inicializar_listas();

// Carga de TCB
void pasar_a_ready(t_tcb_entrenador* tcb);
void cargar_instruccion(t_tcb_entrenador* tcb, int instruccion);

void cargar_rafaga_intercambio(t_tcb_entrenador* tcb);
void cargar_rafaga_captura(t_tcb_entrenador*, t_posicion*);
void cargar_rafaga_movimiento(t_tcb_entrenador*, t_posicion*);

int distancia_entre(t_posicion* inicio, t_posicion* destino);


// Ejecucion de TCB
t_tcb_entrenador* siguiente_tcb_a_ejecutar();

#endif /* PLANIFICACION_H_ */
