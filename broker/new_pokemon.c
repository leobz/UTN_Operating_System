/*
 * new_pokemon.c
 *
 *  Created on: May 3, 2020
 *      Author: diego
 */

#include "colas.h"

t_mensaje_sc *fin=NULL;
t_mensaje_sc *frente=NULL;

t_cola_proceso *cola_new_pokemon_first=NULL;
t_cola_proceso *cola_new_pokemon_last=NULL;

void insertar_new_pokemon(t_mensaje_sc* nuevo_mensaje){

		if(frente == NULL){
			frente = nuevo_mensaje;
		}

		else{
			fin->siguiente = nuevo_mensaje;
		}
	fin=nuevo_mensaje;
}


t_mensaje_sc* extraer_new_pokemon(){

	t_mensaje_sc *actual=NULL;

	actual = frente;
	frente = frente->siguiente;

	if(frente==NULL)
		fin=NULL;

	return actual;

	// TODO: suscript
	//if(suscriptor confirma)
	free(actual);
}

void encolar_proceso_new_pokemon(int socket_cliente){

	// Creo una cola temporal
	t_cola_proceso *c = malloc(sizeof(t_cola_proceso));
	c->socket_cliente = socket_cliente;

	// Si es el primero, ambos apuntan al mismo espacio de memoria
	if(cola_new_pokemon_first == NULL){
		cola_new_pokemon_first = c;
	}
	else{
		cola_new_pokemon_last->siguiente = c;
	}
	cola_new_pokemon_last = c;
}



