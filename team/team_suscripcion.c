#include "team_suscripcion.h"

t_suscripcion* crear_t_suscripcion(int id_proceso, int cola) {
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
	suscripcion->cod_operacion = SUSCRIPCION;
	suscripcion->cola_a_suscribir = cola;
	suscripcion->id_proceso = id_proceso;
	return suscripcion;
}

void iniciar_suscripcion_get(int id_proceso, char* ip, char* puerto) {
	int conexion_get = crear_conexion(ip, puerto);
	if (conexion_get == -1) {
		printf("ERROR: Conexion GET con [Broker] no establecida");
	}
	else {
		pthread_t hilo_get;
		pthread_create(&hilo_get, NULL, (void*) recibir_mensaje_get, conexion_get);
		pthread_detach(hilo_get);

		t_suscripcion* suscripcion_get = crear_t_suscripcion(id_proceso, GET_POKEMON);
		void* a_enviar = empaquetar_suscripcion(suscripcion_get);
		log_info(logger, "%ld Conexion GET establecida con [Broker]",
				(long) getpid());
		enviar_mensaje(conexion_get, a_enviar, sizeof(int) * 3);
	}
}

void recibir_mensaje_get(int conexion) {
	while (1) {
		t_paquete_socket* paquete =  recibir_mensajes(conexion);
		procesar_mensaje_get(paquete);
	}
}

void procesar_mensaje_get(t_paquete_socket* paquete_socket){
	if ( paquete_socket->codigo_operacion == GET_POKEMON){
		mensaje_get= deserializar_mensaje_get_pokemon(paquete_socket->buffer);

		log_info(logger,"Mensaje recibido de [Broker]: GET_POKEMON %s",mensaje_get->pokemon);

		free(mensaje_get->pokemon);
		free(mensaje_get);
	}
	else if ( paquete_socket->codigo_operacion == CONFIRMACION) {
		log_info(logger,"Confirmacion %d",paquete_socket->id_mensaje);
	}
	else {
		printf("\nERROR, Servidor GET recibio codigo de operacion : %s \n", paquete_socket->codigo_operacion);
	}
	liberar_paquete_socket(paquete_socket);
}
