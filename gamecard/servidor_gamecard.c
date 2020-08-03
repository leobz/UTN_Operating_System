/*
 * servidor_gamecard.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "servidor_gamecard.h"

int socket_broker;

void solicitar_suscripcion(int id_proceso, int cola, int conexion) {
	t_suscripcion* suscripcion = crear_t_suscripcion(id_proceso, cola);
	void* a_enviar = empaquetar_suscripcion(suscripcion);
	log_info(logger, "Conexion %s establecida con [Broker]", op_code_to_string(cola));
	enviar_mensaje(conexion, a_enviar, sizeof(int) * 3);
}

void iniciar_hilo_suscripcion(t_datos_suscripcion* datos_suscripcion) {
	pthread_t hilo_suscripcion;
	pthread_create(&hilo_suscripcion, NULL, (void*) recibir_mensajes_gamecard, datos_suscripcion);
	pthread_detach(hilo_suscripcion);
}

void iniciar_suscripcion(t_datos_suscripcion* datos_suscripcion) {
	while (datos_suscripcion->conexion == -1) {
		printf("ERROR: Conexion %s con [Broker] no establecida\n", op_code_to_string(datos_suscripcion->cola));
		sleep(gamecard_config->tiempo_reintento_conexion);
		datos_suscripcion->conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
	}

	solicitar_suscripcion(datos_suscripcion->id_proceso, datos_suscripcion->cola, datos_suscripcion->conexion);
}

void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {
	log_info(logger, "[CONEXION] COD_OP:%s ID:%d",
			op_code_to_string(paquete->codigo_operacion),
			paquete->id_correlativo);
}

void recibir_mensajes_gamecard(t_datos_suscripcion* datos_suscripcion){
	iniciar_suscripcion(datos_suscripcion);

	while (true){
		t_paquete_socket* paquete =  recibir_mensajes(datos_suscripcion->conexion);

		if (paquete->codigo_operacion != OP_ERROR) {
			procesar_mensaje_recibido_broker(paquete);
		}
		else {
			close(datos_suscripcion->conexion);
			datos_suscripcion->conexion = -1;
			iniciar_suscripcion(datos_suscripcion);
		}
	}
}



void procesar_mensaje_recibido_broker(t_paquete_socket* paquete_socket){

	if ((paquete_socket->codigo_operacion >= 0) && (paquete_socket->codigo_operacion <=3)){
		printf("Recibiendo mensaje de la cola %d... \n", paquete_socket->codigo_operacion);
		loggear_nueva_conexion(logger, paquete_socket);

		preparar_confirmacion(paquete_socket->id_mensaje);

		switch (paquete_socket->codigo_operacion){
		case NEW_POKEMON:
			procesar_new_pokemon(paquete_socket);
			break;

		case CATCH_POKEMON:
			procesar_catch_pokemon(paquete_socket);
			break;

		case GET_POKEMON:
			procesar_get_pokemon(paquete_socket);
			break;

		default:
			break;
		}
		liberar_paquete_socket(paquete_socket);
	}
}

void procesar_mensaje_recibido_cliente(t_paquete_socket* paquete_socket){

	if ((paquete_socket->codigo_operacion >= 0) && (paquete_socket->codigo_operacion <=3)){
		printf("Recibiendo mensaje de la cola %d... \n", paquete_socket->codigo_operacion);
		loggear_nueva_conexion(logger, paquete_socket);

		switch (paquete_socket->codigo_operacion){
		case NEW_POKEMON:
			procesar_new_pokemon(paquete_socket);
			liberar_paquete_socket(paquete_socket);
			break;

		case CATCH_POKEMON:
			procesar_catch_pokemon(paquete_socket);
			liberar_paquete_socket(paquete_socket);
			break;

		case GET_POKEMON:
			procesar_get_pokemon(paquete_socket);
			liberar_paquete_socket(paquete_socket);
			break;
		}
	}
}

void preparar_confirmacion(int id_men){

	int conexion_corfirmacion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	enviar_confirmacion(id_men,CONFIRMACION,conexion_corfirmacion);
	liberar_conexion(conexion_corfirmacion);
}

void enviar_confirmacion(int id, int confirmacion, int socket){
	int offset=0;
	printf("Enviando confirmacion al socket %d... \n", socket);
	void*enviar = malloc(sizeof(int) * 3);
	memcpy(enviar, &confirmacion,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&gamecard_config->id_proceso,sizeof(int));

	enviar_mensaje(socket,enviar,sizeof(int)*3);
}

