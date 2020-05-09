/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#include "colas.h"


void encolar_proceso(int socket_cliente,int numero_de_cola){

	switch(numero_de_cola){

	//GAMECARD
	case NEW_POKEMON:
		encolar_proceso_new_pokemon(socket_cliente);
		break;

	case GET_POKEMON:
		encolar_proceso_get_pokemon(socket_cliente);
		break;

	case CATCH_POKEMON:
		encolar_proceso_catch_pokemon(socket_cliente);
		break;

	//TEAM
	case APPEARED_POKEMON:
		//TODO
		break;

	case LOCALIZED_POKEMON:
		//TODO
		break;

	case CAUGHT_POKEMON:
		//TODO
		break;

	}

}
