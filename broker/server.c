/*
 * server.c
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#include"colas.h"

t_log* logger;
t_broker_config* broker_config;

void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {

	log_info(logger, "[CONEXION] COD_OP:%s ID:%d",
			op_code_to_string(paquete->codigo_operacion),
			paquete->id_correlativo);
}

void procesar_mensaje_recibido(t_paquete_socket* paquete) {

	inicializar_broker(&broker_config, &logger);

	loggear_nueva_conexion(logger, paquete);

	if ((paquete->codigo_operacion >= 0) && (paquete->codigo_operacion <= 5)) {

		t_mensaje* mensaje_a_encolar;
		t_mensaje* mensaje_a_enviar;



		switch (paquete->codigo_operacion) {

		case NEW_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar, NEW_POKEMON);
			mensaje_a_enviar = extraer_mensaje(NEW_POKEMON);

			break;

		case GET_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar, GET_POKEMON);
			mensaje_a_enviar = extraer_mensaje(GET_POKEMON);


			break;

		case CATCH_POKEMON:

			mensaje_a_encolar= preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar,CATCH_POKEMON);
			mensaje_a_enviar= extraer_mensaje(CATCH_POKEMON);


			break;

		case APPEARED_POKEMON:

			mensaje_a_encolar = preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar, APPEARED_POKEMON);
			mensaje_a_enviar = extraer_mensaje(APPEARED_POKEMON);


			break;

		case LOCALIZED_POKEMON:

			mensaje_a_encolar= preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar,LOCALIZED_POKEMON);
			mensaje_a_enviar= extraer_mensaje(LOCALIZED_POKEMON);


			break;

		case CAUGHT_POKEMON:
			mensaje_a_encolar= preparar_mensaje(paquete);
			insertar_mensaje(mensaje_a_encolar,CATCH_POKEMON);
			mensaje_a_enviar= extraer_mensaje(CATCH_POKEMON);


			break;

		default:
			pthread_exit(NULL);

			break;
		}

		free(mensaje_a_encolar->payload); //Hacer solo despues de guardar el mensaje en la cache y en la cola auxiliar
		free(mensaje_a_encolar);
		liberar_paquete(paquete);
	}

	else {

		switch (paquete->codigo_operacion) {
		case SUSCRIPCION:

			log_info(logger, "[SUSCRIPCION] Cola:%s", op_code_to_string(paquete->cola));

			//encolar_proceso(paquete->socket_cliente,paquete->cola);

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
	finalizar_broker(broker_config, logger);
}

t_mensaje* preparar_mensaje(t_paquete_socket* paquete) {

	t_mensaje* mensaje_a_preparar = malloc(sizeof(t_mensaje));

	id_cola[paquete->codigo_operacion]++;

	mensaje_a_preparar->codigo_operacion = paquete->codigo_operacion;
	mensaje_a_preparar->id_mensaje = id_cola[paquete->codigo_operacion];
	mensaje_a_preparar->id_correlativo = paquete->id_correlativo;
	mensaje_a_preparar->payload_size = paquete->buffer->size;
	mensaje_a_preparar->payload = malloc(paquete->buffer->size);
	memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
	mensaje_a_preparar->siguiente = NULL;

	return mensaje_a_preparar;
}

void liberar_paquete(t_paquete_socket* paquete) {

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
