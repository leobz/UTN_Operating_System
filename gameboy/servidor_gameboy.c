/*
 * servidor_gameboy.c
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */

#include "servidor_gameboy.h"

void servidor_gameboy(int conexion) {


	while (1) {
		t_paquete_socket* paquete =  recibir_mensaje_servidor(conexion);
		procesar_mensaje_recibido(paquete);
	}
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket) {

	if ((paquete_socket->codigo_operacion >= 0)&& (paquete_socket->codigo_operacion <= 5)) {

		switch (paquete_socket->codigo_operacion) {

		case NEW_POKEMON:
			mensaje_new = deserializar_mensaje_new_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: NEW_POKEMON %s %d %d %d",mensaje_new->pokemon, mensaje_new->posx,mensaje_new->posy,mensaje_new->cantidad);

				free(mensaje_new->pokemon);
				free(mensaje_new);

			break;

		case GET_POKEMON:
			mensaje_get= deserializar_mensaje_get_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: GET_POKEMON %s",mensaje_get->pokemon);

				free(mensaje_get->pokemon);
				free(mensaje_get);


			break;

		case CATCH_POKEMON:

			mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);
				log_info(logger,"Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);


				free(mensaje_catch->pokemon);
				free(mensaje_catch);

			break;

		case APPEARED_POKEMON:

			mensaje_appeared= deserializar_mensaje_appeared_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: APPEARED_POKEMON %s %d %d",mensaje_appeared->pokemon, mensaje_appeared->posx,mensaje_appeared->posy);

				free(mensaje_appeared->pokemon);
				free(mensaje_appeared);


			break;

		case LOCALIZED_POKEMON:

			/*mensaje_localized= deserializar_mensaje_localized_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: LOCALIZED_POKEMON %s %d",mensaje_localized->pokemon, mensaje_localized->cantidad_posiciones);

				free(mensaje_localized->pokemon);
				free(mensaje_localized);
				*/


			break;

		case CAUGHT_POKEMON:
			mensaje_caught= deserializar_mensaje_caught_pokemon(paquete_socket->buffer);

			log_info(logger,"Mensaje recibido de [Broker]: CAUGHT_POKEMON %s",value_to_state(mensaje_caught->resultado));

				free(mensaje_caught);

			break;

		default:
			break;
		}

		liberar_paquete_socket(paquete_socket);

	}
	else{
		switch (paquete_socket->codigo_operacion) {
			case SUSCRIPCION:
				break;

			case CONFIRMACION:
				log_info(logger,"Confirmacion %d",paquete_socket->id_mensaje);

				break;

			case OP_ERROR:
				break;

			default:
				break;
			}
	free(paquete_socket);
	}
}