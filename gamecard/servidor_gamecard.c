/*
 * servidor_gamecard.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "servidor_gamecard.h"

void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {
	log_info(logger, "[CONEXION] COD_OP:%s ID:%d",
			op_code_to_string(paquete->codigo_operacion),
			paquete->id_correlativo);
}

void confirmar_recepcion(t_paquete_socket* paquete_socket){
	int offset = 0;
	void* a_enviar = malloc(sizeof(int)*2);
	int* confirmacion = CONFIRMACION;

	memcpy(a_enviar, &confirmacion, sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar, &paquete_socket->id_correlativo, sizeof(int));

	enviar_mensaje(paquete_socket->socket_cliente, a_enviar, sizeof(int)*2);
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket){

	if ((paquete_socket->codigo_operacion >= 0) && (paquete_socket->codigo_operacion <=3)){
		printf("Recibiendo mensaje de la cola %d... \n", paquete_socket->codigo_operacion);
		loggear_nueva_conexion(logger, paquete_socket);

		confirmar_recepcion(paquete_socket);

		switch (paquete_socket->codigo_operacion){
		case NEW_POKEMON:
			//TODO
			break;

		case CATCH_POKEMON:
			//TODO
			break;

		case GET_POKEMON:
			//TODO
			break;
		}
		free(paquete_socket);
	}
}
