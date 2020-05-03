/*
 * new_pokemon.c
 *
 *  Created on: May 3, 2020
 *      Author: diego
 */

#include "colas.h"

t_mensaje_sc *fin=NULL;
t_mensaje_sc *frente=NULL;

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
	//free(actual);
}

