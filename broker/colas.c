/*
 * colas.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */



void encolar_proceso(int socket_cliente,int numero_de_cola){

	switch(numero_de_cola){

	case NEW_POKEMON:
		encolar_proceso_new_pokemon(socket_cliente);
	}
}
