/*
 * server.c
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#include"colas.h"



void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {

	log_info(logger, "[CONEXION] COD_OP:%s ID:%d",
			op_code_to_string(paquete->codigo_operacion),
			paquete->id_correlativo);
}



void procesar_mensaje_recibido(t_paquete_socket* paquete) {




	if ((paquete->codigo_operacion >= 0) && (paquete->codigo_operacion <= 5)) {
		loggear_nueva_conexion(logger, paquete);

		t_mensaje* mensaje_a_encolar;

		switch (paquete->codigo_operacion) {

		case NEW_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[NEW_POKEMON]);
			insertar_mensaje(mensaje_a_encolar, NEW_POKEMON);
			pthread_mutex_unlock(&mutex[NEW_POKEMON]);

			break;

		case GET_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[GET_POKEMON]);
			insertar_mensaje(mensaje_a_encolar, GET_POKEMON);
			pthread_mutex_unlock(&mutex[GET_POKEMON]);

			break;

		case CATCH_POKEMON:

			mensaje_a_encolar= preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[CATCH_POKEMON]);
			insertar_mensaje(mensaje_a_encolar,CATCH_POKEMON);
			pthread_mutex_unlock(&mutex[CATCH_POKEMON]);

			break;

		case APPEARED_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[APPEARED_POKEMON]);
			insertar_mensaje(mensaje_a_encolar, APPEARED_POKEMON);
			pthread_mutex_unlock(&mutex[APPEARED_POKEMON]);

			break;

		case LOCALIZED_POKEMON:

			mensaje_a_encolar= preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[LOCALIZED_POKEMON]);
			insertar_mensaje(mensaje_a_encolar,LOCALIZED_POKEMON);
			pthread_mutex_unlock(&mutex[LOCALIZED_POKEMON]);

			break;

		case CAUGHT_POKEMON:

			mensaje_a_encolar= preparar_mensaje(paquete);

			pthread_mutex_lock(&mutex[CAUGHT_POKEMON]);
			insertar_mensaje(mensaje_a_encolar,CAUGHT_POKEMON);
			pthread_mutex_unlock(&mutex[CAUGHT_POKEMON]);

			break;

		default:
			pthread_exit(NULL);

			break;
		}


		sem_post(&cola_vacia[paquete->codigo_operacion]);
		liberar_paquete(paquete);
	}

	else {

		switch (paquete->codigo_operacion) {
		case SUSCRIPCION:

			log_info(logger, "[SUSCRIPCION] Cola:%s", op_code_to_string(paquete->cola));
			encolar_proceso(paquete->socket_cliente,paquete->cola);
			log_info(logger, "Cola:%d",paquete->cola);
			sem_post(&sem_proceso[paquete->cola]);
			break;

		case OP_ERROR:

			printf("Error al recibir mensaje del socket: %d\n",
					paquete->socket_cliente);
			pthread_exit(NULL);
			break;

		default:
			pthread_exit(NULL);
			break;
		}

		free(paquete);
	}
}

t_mensaje* preparar_mensaje(t_paquete_socket* paquete) {

	t_mensaje* mensaje_a_preparar = malloc(sizeof(t_mensaje));

	pthread_mutex_lock(&global); //id_mensaje es una variable compartida

		id_mensaje++;
		enviar_mensaje_nofree(paquete->socket_cliente,&id_mensaje,sizeof(int)); //le devuelve al proceso emisor el id del mensaje
		mensaje_a_preparar->id_mensaje = id_mensaje;

	pthread_mutex_unlock(&global);

	mensaje_a_preparar->codigo_operacion = paquete->codigo_operacion;
	mensaje_a_preparar->id_correlativo = paquete->id_correlativo;
	mensaje_a_preparar->payload_size = paquete->buffer->size;
	mensaje_a_preparar->payload = malloc(paquete->buffer->size);
	memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
	mensaje_a_preparar->siguiente = NULL;

	return mensaje_a_preparar;
}

