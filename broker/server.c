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
		mensaje_a_encolar = preparar_mensaje(paquete);

		//log_info(logger, "id:%d",id_mensaje);

		pthread_mutex_lock(&mutex[paquete->codigo_operacion]);
			insertar_mensaje(mensaje_a_encolar, paquete->codigo_operacion);
		pthread_mutex_unlock(&mutex[paquete->codigo_operacion]);


		sem_post(&cola_vacia[paquete->codigo_operacion]);
		liberar_paquete_socket(paquete);
	}

	else {

		switch (paquete->codigo_operacion) {
		case SUSCRIPCION:

			log_info(logger, "[SUSCRIPCION] Cola:%s", op_code_to_string(paquete->cola));
			list_add(suscriptores[paquete->cola], paquete->socket_cliente);

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

	int identificador=id_mensaje++;
	enviar_confirmacion(identificador,CONFIRMACION,paquete->socket_cliente);
	mensaje_a_preparar->id_mensaje = identificador;

	pthread_mutex_unlock(&global);

	mensaje_a_preparar->codigo_operacion = paquete->codigo_operacion;
	mensaje_a_preparar->id_correlativo = paquete->id_correlativo;
	mensaje_a_preparar->payload_size = paquete->buffer->size;
	mensaje_a_preparar->payload = malloc(paquete->buffer->size);
	memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
	mensaje_a_preparar->siguiente = NULL;

	return mensaje_a_preparar;
}

void enviar_confirmacion(int id,op_code confirmacion,int socket){

		//log_info(logger, "socket %d",socket);
		//log_info(logger, "id %d",id);

		int offset=0;

		void*enviar=malloc(sizeof(int)*2);
		memcpy(enviar,&confirmacion,sizeof(int));
		offset+=sizeof(int);
		memcpy(enviar,&id,sizeof(int));

		enviar_mensaje(socket,enviar,sizeof(int)*2); //le devuelve al proceso emisor el id del mensaje

}

