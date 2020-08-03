/*
 * servidor_gameboy.c
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */

#include "servidor_gameboy.h"


void servidor_gameboy(int conexion) {

	while (1) {
		t_paquete_socket* paquete =  recibir_mensajes(conexion);
		procesar_mensaje_recibido(paquete);
	}
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket) {

		switch (paquete_socket->codigo_operacion) {

		case NEW_POKEMON:{
			t_mensaje_new* mensaje_new;

			mensaje_new = deserializar_mensaje_new_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: NEW_POKEMON %s %d %d %d",mensaje_new->pokemon, mensaje_new->posx,mensaje_new->posy,mensaje_new->cantidad);

				preparar_confirmacion(paquete_socket->id_mensaje);

				free(mensaje_new->pokemon);
				free(mensaje_new);

			break;}

		case GET_POKEMON:{

			t_mensaje_get* mensaje_get;

			mensaje_get= deserializar_mensaje_get_pokemon(paquete_socket->buffer);

			log_info(logger,"Mensaje recibido de [Broker]: GET_POKEMON %s",mensaje_get->pokemon);

			preparar_confirmacion(paquete_socket->id_mensaje);

			free(mensaje_get->pokemon);
			free(mensaje_get);

			break;}


		case CATCH_POKEMON:{

			t_mensaje_catch* mensaje_catch;

			mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);
				log_info(logger,"%ld Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d", (long)getpid(), mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);

				preparar_confirmacion(paquete_socket->id_mensaje);

				free(mensaje_catch->pokemon);
				free(mensaje_catch);


			break;
		}

		case APPEARED_POKEMON:{

			t_mensaje_appeared *mensaje_appeared;
			mensaje_appeared= deserializar_mensaje_appeared_pokemon(paquete_socket->buffer);

				log_info(logger,"Mensaje recibido de [Broker]: APPEARED_POKEMON %s %d %d",mensaje_appeared->pokemon, mensaje_appeared->posx,mensaje_appeared->posy);

				preparar_confirmacion(paquete_socket->id_mensaje);

				free(mensaje_appeared->pokemon);
				free(mensaje_appeared);

			break;}

		case LOCALIZED_POKEMON:{

			t_mensaje_localized* mensaje_localized;
			mensaje_localized= deserializar_mensaje_localized_pokemon(paquete_socket->buffer);
				log_info(logger,"Mensaje recibido de [Broker]: LOCALIZED_POKEMON %s %d",mensaje_localized->pokemon, mensaje_localized->cantidad_posiciones);

				preparar_confirmacion(paquete_socket->id_mensaje);

				free(mensaje_localized->pokemon);
				free(mensaje_localized);


			break;}

		case CAUGHT_POKEMON:{

			t_mensaje_caught* mensaje_caught;

			mensaje_caught= deserializar_mensaje_caught_pokemon(paquete_socket->buffer);

			log_info(logger,"Mensaje recibido de [Broker]: CAUGHT_POKEMON %s con ID_CORRELATIVO: %d",value_to_state(mensaje_caught->resultado),paquete_socket->id_correlativo);

			preparar_confirmacion(paquete_socket->id_mensaje);

			free(mensaje_caught);

			break;}

		default:
			break;
		}


		liberar_paquete_socket(paquete_socket);
}


void recibir_id_correlativo(int socket_cliente) {
	t_paquete_socket* paquete =  recibir_mensajes(socket_cliente);

	//log_info(logger, "[MSG_RECIBIDO] ID_CORRELATIVO: %d\n", paquete->id_mensaje);
	printf("Recibida Confirmacion: %d\n",paquete->id_mensaje);
	free(paquete);
}

void preparar_confirmacion(int id_men){

	int conexion_corfirmacion = crear_conexion(gameboy_config->ip_broker,gameboy_config->puerto_broker);
	enviar_confirmacion(id_men,CONFIRMACION,conexion_corfirmacion);
	liberar_conexion(conexion_corfirmacion);
}


void enviar_confirmacion(int id, int confirmacion, int socket){
	int offset=0;

	void*enviar = malloc(sizeof(int) * 3);
	memcpy(enviar, &confirmacion,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id_proceso_gameboy,sizeof(int)); //valor nulo pq no es un id_proceso

	enviar_mensaje(socket,enviar,sizeof(int)*3);
	//le devuelve al proceso emisor el id del mensaje
}
