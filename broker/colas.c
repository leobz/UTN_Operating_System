/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#include "colas.h"


t_cola_proceso *cola_procesos[6][2] = { NULL };
t_mensaje_sc *mensajes_sc[3][2] = { NULL };
t_mensaje_cc *mensajes_cc[3][2] = { NULL };
int reset_indice = 3; // Esta variable es para empezar de cero nuevamente en la matriz




void encolar_proceso(int socket, int cola){

	// Creo una cola temporal
	t_cola_proceso *proceso = malloc(sizeof(t_cola_proceso));
	if (proceso == NULL) {
		printf("No se pudo asignar memoria");
		return;
	}

	proceso->socket_cliente = socket;
	proceso->mensaje_recibido = malloc(sizeof(t_cola_mensaje_recibido));
	proceso->mensaje_confirmado = malloc(sizeof(t_cola_mensaje_confirmado));

	// Si es el primero, ambos apuntan al mismo espacio de memoria
	if (cola_procesos[cola][FRENTE] == NULL) {
		cola_procesos[cola][FRENTE] = proceso;
	} else {
		cola_procesos[cola][FIN]->siguiente = proceso;
	}
	cola_procesos[cola][FIN] = proceso;
}

void insertar_mensaje_sc(t_mensaje_sc* nuevo_mensaje, int cola_mensaje){
	if(mensajes_sc[cola_mensaje][FRENTE] == NULL){
		mensajes_sc[cola_mensaje][FRENTE] = nuevo_mensaje;
	} else{
		mensajes_sc[cola_mensaje][FIN]->siguiente = nuevo_mensaje;
	}
	mensajes_sc[cola_mensaje][FIN] = nuevo_mensaje;
}

t_mensaje_sc* extraer_mensaje_sc(int cola_mensaje){
	t_mensaje_sc *actual = NULL;
	actual = mensajes_sc[cola_mensaje][FRENTE];
	mensajes_sc[cola_mensaje][FRENTE] = mensajes_sc[cola_mensaje][FRENTE]->siguiente;
	if (mensajes_sc[cola_mensaje][FRENTE] == NULL){
		mensajes_sc[cola_mensaje][FIN] = NULL;
	}
	return actual;
	// TODO
	free(actual);

}

void encolar_mensaje_cc(t_mensaje_cc* nuevo_mensaje, int cola_mensaje){
	if(mensajes_cc[cola_mensaje-reset_indice][FRENTE] == NULL){
		mensajes_cc[cola_mensaje-reset_indice][FRENTE] = nuevo_mensaje;
	} else{
		mensajes_cc[cola_mensaje-reset_indice][FIN]->siguiente = nuevo_mensaje;
	}
	mensajes_cc[cola_mensaje-reset_indice][FIN] = nuevo_mensaje;
}

t_mensaje_cc* extraer_mensaje_cc(int cola_mensaje){
	t_mensaje_cc *actual = NULL;
	actual = mensajes_cc[cola_mensaje-reset_indice][FRENTE];
	mensajes_cc[cola_mensaje-reset_indice][FRENTE] = mensajes_cc[cola_mensaje][FRENTE]->siguiente;
	if (mensajes_cc[cola_mensaje-reset_indice][FRENTE] == NULL){
		mensajes_cc[cola_mensaje-reset_indice][FIN] = NULL;
	}
	return actual;
	// TODO
	free(actual);

}


