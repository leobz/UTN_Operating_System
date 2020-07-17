/*
 * servidor_gamecard.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "servidor_gamecard.h"

int socket_broker;

void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {
	log_info(logger, "[CONEXION] COD_OP:%s ID:%d",
			op_code_to_string(paquete->codigo_operacion),
			paquete->id_correlativo);
}

void servidor_gamecard(int conexion){
	while (1){
		t_paquete_socket* paquete =  recibir_mensajes(conexion);
		procesar_mensaje_recibido(paquete);
	}
}

void suscribirme_al_broker(){
	printf("Suscribiendome al broker...\n");

	int colas_a_suscribir[] = {NEW_POKEMON, GET_POKEMON, CATCH_POKEMON};
	int id_proceso = 0;
	int i;
	for (i=0; i<3; i++){
		int conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
		socket_broker = conexion;
		while (conexion == -1) {
			printf("ERROR: Conexion con [Broker] no establecida\n");
			sleep(gamecard_config->tiempo_reintento_conexion);
			conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
		}

		pthread_t hilo_gamecard_servidor;
		pthread_create(&hilo_gamecard_servidor, NULL, (void*)servidor_gamecard, conexion);
		pthread_detach(hilo_gamecard_servidor);

		t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
		suscripcion->cod_operacion = SUSCRIPCION;
		suscripcion->cola_a_suscribir = colas_a_suscribir[i];
		suscripcion->id_proceso = id_proceso;

		void *a_enviar = empaquetar_suscripcion(suscripcion);
		printf("Enviando mensaje de cola %d...\n", suscripcion->cola_a_suscribir);
		enviar_mensaje(socket_broker, a_enviar, sizeof(int) * 3);

		//TODO liberar_conexion(conexion);
	}
}

void confirmar_recepcion(t_paquete_socket* paquete_socket){
	printf("Enviando mensaje al socket %d... \n", socket_broker);
	int offset = 0;
	int* confirmacion = CONFIRMACION;
	void* a_enviar = malloc(sizeof(int)*3);

	memcpy(a_enviar, &confirmacion, sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar, &paquete_socket->id_mensaje, sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar, &gamecard_config->id_proceso, sizeof(int));

	enviar_mensaje(paquete_socket->socket_cliente, a_enviar, sizeof(int)*2);
}

void procesar_mensaje_recibido(t_paquete_socket* paquete_socket){

	if ((paquete_socket->codigo_operacion >= 0) && (paquete_socket->codigo_operacion <=3)){
		printf("Recibiendo mensaje de la cola %d... \n", paquete_socket->codigo_operacion);
		loggear_nueva_conexion(logger, paquete_socket);

		confirmar_recepcion(paquete_socket);

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
		}
		free(paquete_socket);
	}
}
