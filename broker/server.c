/*
 * server.c
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */


#include"colas.h"


void procesar_mensaje_recibido(t_paquete_socket* paquete) {
	//t_mensaje_new* mensaje_new;

	int size;
	int num_cola;
	t_log* logger;
	t_broker_config* broker_config;

	t_mensaje_sc* mensaje_a_encolar= malloc(sizeof(t_mensaje_sc));
	t_mensaje_sc* mensaje_a_enviar;

	inicializar_broker(&broker_config,&logger);

	switch(paquete->codigo_operacion) {
		case NEW_POKEMON:

			id_cola[NEW_POKEMON]++;

			mensaje_a_encolar->codigo_operacion= NEW_POKEMON;
			mensaje_a_encolar->id_mensaje=id_cola[NEW_POKEMON];
			mensaje_a_encolar->payload_size= paquete->buffer->size;
			mensaje_a_encolar->payload = malloc(paquete->buffer->size);
			memcpy(mensaje_a_encolar->payload, paquete->buffer->stream, size);
			mensaje_a_encolar->siguiente=NULL;

			insertar_new_pokemon(mensaje_a_encolar);
			mensaje_a_enviar= extraer_new_pokemon();

			log_info(logger,"Mensaje recibido de [GAMEBOY]:  %d",mensaje_a_enviar->codigo_operacion);

			free(mensaje_a_encolar->payload);
			free(mensaje_a_encolar);
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);

			break;

		case SUSCRIPCION:


			recv(paquete->socket_cliente,&num_cola,sizeof(int),MSG_WAITALL);
			encolar_proceso(paquete->socket_cliente,num_cola);

			break;

		case OP_ERROR:

			printf("Error al recibir mensaje del socket: %d\n",paquete->socket_cliente);
			pthread_exit(NULL);
			break;
		default:
			pthread_exit(NULL);
			break;
	}
}




