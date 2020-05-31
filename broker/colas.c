/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#include "colas.h"


void encolar_proceso(int socket, int cola){

	// Creo una cola temporal
	t_cola_proceso *proceso = malloc(sizeof(t_cola_proceso));
	if (proceso == NULL) {
		printf("No se pudo asignar memoria");
		return;
	}

	proceso->socket_cliente = socket;
	//proceso->mensaje_recibido = malloc(sizeof(t_cola_mensaje_recibido));


	// Si es el primero, ambos apuntan al mismo espacio de memoria
	if (cola_procesos[cola][FRENTE] == NULL) {
		cola_procesos[cola][FRENTE] = proceso;
		cola_procesos[cola][ACTUAL] = proceso;
		proceso_vacio[cola]=false;
	} else {
		cola_procesos[cola][FIN]->siguiente = proceso;
	}
	cola_procesos[cola][FIN] = proceso;
}

t_cola_proceso* desencolar_proceso(int cola){

	return cola_procesos[cola][ACTUAL];

	proceso_vacio[cola]=false;

	if(cola_procesos[cola][ACTUAL]->siguiente == NULL){
		proceso_vacio[cola]=true; //si recorre toda la cola lo vuelvo a apuntar al inicio
		cola_procesos[cola][ACTUAL]= cola_procesos[cola][FRENTE];
	}

	if (proceso_vacio[cola]==false)
		cola_procesos[cola][ACTUAL]=cola_procesos[cola][ACTUAL]->siguiente;
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
	mensajes[cola_mensaje][ACTUAL] = mensajes[cola_mensaje][FRENTE];
	mensajes[cola_mensaje][FRENTE] = mensajes[cola_mensaje][FRENTE]->siguiente;
	if (mensajes[cola_mensaje][FRENTE] == NULL){
		mensajes[cola_mensaje][FIN] = NULL;
	}
	return mensajes[cola_mensaje][ACTUAL];
}

