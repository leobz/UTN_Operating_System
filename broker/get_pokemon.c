/*
 * get_pokemon.c
 *
 *  Created on: 9 may. 2020
 *      Author: utnso
 */

#include "colas.h"

t_cola_proceso *cola_get_pokemon_first=NULL;
t_cola_proceso *cola_get_pokemon_last=NULL;

void encolar_proceso_get_pokemon(int socket_cliente){

	// Creo una cola temporal
	t_cola_proceso *c = malloc(sizeof(t_cola_proceso));
	c->socket_cliente = socket_cliente;

	// Si es el primero, ambos apuntan al mismo espacio de memoria
	if(cola_get_pokemon_first == NULL){
		cola_get_pokemon_first = c;
	}
	else{
		cola_get_pokemon_last->siguiente = c;
	}
	cola_get_pokemon_last = c;
}
