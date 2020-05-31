/*
 * servidor_gameboy.c
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */
/*
#include "servidor_gameboy.h"

void* servidor_gameboy() {
	char*ip = gameboy_config->ip_gameboy;
	char*puerto = gameboy_config->puerto_gameboy;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1) {
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket) {
	// enviar el mensaje al brocker de desuscripcion

	// swich case de deserializacion para cada mensaje

	if ((paquete_socket->codigo_operacion >= 0)
			&& (paquete_socket->codigo_operacion <= 5)) {

		t_mensaje_catch* mensaje_catch;

		switch (paquete_socket->codigo_operacion) {

		case NEW_POKEMON:
			// TODO
			break;

		case GET_POKEMON:
			// TODO
			break;

		case CATCH_POKEMON:

			mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);

			log_info(logger,
					"Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d",
					mensaje_catch->pokemon, mensaje_catch->pos_x,
					mensaje_catch->pos_y);

			break;

		case APPEARED_POKEMON:
			// TODO
			break;

		case LOCALIZED_POKEMON:
			// TODO
			break;

		case CAUGHT_POKEMON:
			// TODO
			break;

		default:
			break;
		}
		liberar_paquete(paquete_socket);
		free(mensaje_catch->pokemon);
		free(mensaje_catch);
	}
	else{
		switch (paquete_socket->codigo_operacion) {
			case SUSCRIPCION:
				break;

			case DESUSCRIPCION:
				break;

			case OP_ERROR:
				break;

			default:
				break;
			}
	free(paquete_socket);
	}
}

void desuscribir_gameboy(t_suscripcion* suscripcion, int conexion){
	// Me desuscribo de la cola - structura nueva

	suscripcion->cod_operacion = DESUSCRIPCION;

	void *a_enviar = suscripcion;

	log_info(logger, "Desuscribiendome del [Broker]");
	enviar_mensaje(conexion, a_enviar, sizeof(int) * 2);
}

*/
