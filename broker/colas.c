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

	// Si es el primero, ambos apuntan al mismo espacio de memoria
	if (cola_procesos[cola][FRENTE] == NULL) {
		cola_procesos[cola][FRENTE] = proceso;
		cola_procesos[cola][ACTUAL] = proceso;
		cola_procesos[cola][AUXILIAR] = proceso;
	} else
		cola_procesos[cola][FIN]->siguiente = proceso;

	cola_procesos[cola][FIN] = proceso;

	contador_procesos[cola]++; //contador de procesos
}

t_cola_proceso* desencolar_proceso(int cola){

	cola_procesos[cola][ACTUAL]=cola_procesos[cola][AUXILIAR];

	if(cola_procesos[cola][ACTUAL]->siguiente == NULL){
			proceso_vacio[cola][ULTIMO]=true;
			proceso_vacio[cola][VACIA]=true;}

	if(proceso_vacio[cola][ULTIMO]){
		cola_procesos[cola][AUXILIAR]=cola_procesos[cola][FRENTE];
		return cola_procesos[cola][FIN];}


	cola_procesos[cola][AUXILIAR]=cola_procesos[cola][AUXILIAR]->siguiente;


	return cola_procesos[cola][ACTUAL];
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

