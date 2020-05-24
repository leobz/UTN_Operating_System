/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#include "colas.h"


t_cola_proceso *cola_procesos[6][2] = { NULL };
t_mensaje*mensajes[6][2] = { NULL };




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

void insertar_mensaje(t_mensaje* nuevo_mensaje, int cola_mensaje){
	if(mensajes[cola_mensaje][FRENTE] == NULL){
		mensajes[cola_mensaje][FRENTE] = nuevo_mensaje;
	} else{
		mensajes[cola_mensaje][FIN]->siguiente = nuevo_mensaje;
	}
	mensajes[cola_mensaje][FIN] = nuevo_mensaje;
}

t_mensaje* extraer_mensaje(int cola_mensaje){
	t_mensaje *actual = NULL;
	actual = mensajes[cola_mensaje][FRENTE];
	mensajes[cola_mensaje][FRENTE] = mensajes[cola_mensaje][FRENTE]->siguiente;
	if (mensajes[cola_mensaje][FRENTE] == NULL){
		mensajes[cola_mensaje][FIN] = NULL;
	}
	return actual;
	// TODO
	free(actual);

}

