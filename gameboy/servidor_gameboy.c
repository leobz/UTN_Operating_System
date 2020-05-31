/*
 * servidor_gameboy.c
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */

#include "servidor_gameboy.h"

void* servidor_gameboy() {
	char*ip = gameboy_config->ip_broker;
	char*puerto = gameboy_config->puerto_broker;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1) {
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket) {
	// deserializar paquete
	// loggear en el archivo
	// enviar el mensaje al brocker de desuscripcion

	// swich case de deserializacion para cada mensaje

	if ((paquete_socket->codigo_operacion >= 0)
			&& (paquete_socket->codigo_operacion <= 5)) {

		switch (paquete_socket->codigo_operacion) {

		case NEW_POKEMON:
			// TODO
			break;

		case GET_POKEMON:
			// TODO
			break;

		case CATCH_POKEMON:
			t_buffer* buffer = paquete_socket->buffer;
			char* pokemon;
			int pokemon_length, pos_x, pos_y;

			int offset = 0;
			memcpy(pokemon_length, buffer->stream + offset, sizeof(int));

			offset += sizeof(int);
			memcpy(pokemon, buffer->stream + offset, pokemon_length - 1);
			// TODO: a pokemon le tengo que sacar el ultimo caracter... esta bien hecho?
			offset += pokemon_length;
			memcpy(pos_x, buffer->stream + offset, sizeof(int));

			offset += sizeof(int);
			memcpy(pos_y, buffer->stream + offset, sizeof(int));

			log_info(logger,
					"Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d",
					pokemon, pos_x, pos_y);

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
		}
	}

}
