/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#include "colas.h"

void inicializar_listas(int cola){
	suscriptores[cola]=list_create();
	administradores[cola]=list_create();
	meter_en_diccionario(administracion_por_cod,cola,administradores[cola]);
}

void insertar_mensaje(t_mensaje* nuevo_mensaje, int cola_mensaje){
	printf("Insertando mensaje %s\n",op_code_to_string(cola_mensaje));
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

