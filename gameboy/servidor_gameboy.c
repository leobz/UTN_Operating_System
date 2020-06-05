/*
 * servidor_gameboy.c
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */

#include "servidor_gameboy.h"

void servidor_gameboy(int *conexion) {


	while (1) {
		t_paquete_socket* paquete =  recibir_mensaje_servidor(*conexion);
		procesar_mensaje_recibido(paquete);
	}
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket) {
	// enviar el mensaje al brocker de desuscripcion

	// swich case de deserializacion para cada mensaje

	if ((paquete_socket->codigo_operacion >= 0)&& (paquete_socket->codigo_operacion <= 5)) {

		//log_info(logger,"Recibiendo del broker");


		switch (paquete_socket->codigo_operacion) {

		case NEW_POKEMON:
/*
			mensaje_new = deserializar_mensaje_new_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: NEW_POKEMON %s %d %d %d",mensaje_new->pokemon, mensaje_new->pos_x,mensaje_new->pos_y,mensaje_new->cantidad);

				free(mensaje_new->pokemon);
				free(mensaje_new);
				liberar_paquete(paquete_socket);*/
			break;

		case GET_POKEMON:
			/*mensaje_get= deserializar_mensaje_get_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: GET_POKEMON %s %d %d",mensaje_get->pokemon, mensaje_get->pos_x,mensaje_get->pos_y);

				free(mensaje_get->pokemon);
				free(mensaje_get);
				liberar_paquete(paquete_socket);*/

			break;

		case CATCH_POKEMON:

			mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->pos_x,mensaje_catch->pos_y);

				free(mensaje_catch->pokemon);
				free(mensaje_catch);
				liberar_paquete(paquete_socket);
			break;

		case APPEARED_POKEMON:

			/*mensaje_appeared= deserializar_mensaje_appeared_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: APPEARED_POKEMON %s %d %d",mensaje_appeared->pokemon, mensaje_appeared->pos_x,mensaje_appeared->pos_y);

				free(mensaje_appeared->pokemon);
				free(mensaje_appeared);
				liberar_paquete(paquete_socket);*/

			break;

		case LOCALIZED_POKEMON:

			/*mensaje_localized= deserializar_mensaje_localized_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: LOCALIZED_POKEMON %s %d",mensaje_localized->pokemon, mensaje_localized->cantidad_posiciones);

				free(mensaje_localized->pokemon);
				free(mensaje_localized);
				liberar_paquete(paquete_socket);*/


			break;

		case CAUGHT_POKEMON:
			/*mensaje_caught= deserializar_mensaje_caught_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: CAUGHT_POKEMON %d",mensaje_caught>resultado);

				free(mensaje_caught);
				liberar_paquete(paquete_socket);*/
			break;

		default:
			break;
		}

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
