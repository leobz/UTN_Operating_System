#include "team_suscripcion.h"

t_suscripcion* crear_t_suscripcion(int id_proceso, int cola) {
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
	suscripcion->cod_operacion = SUSCRIPCION;
	suscripcion->cola_a_suscribir = cola;
	suscripcion->id_proceso = id_proceso;
	return suscripcion;
}

void iniciar_suscripcion_appeared(int id_proceso, char* ip, char* puerto) {
	int conexion = crear_conexion(ip, puerto);
	if (conexion == -1) {
		printf("ERROR: Conexion APPEARED con [Broker] no establecida");
	}
	else {
		pthread_t hilo_appeared;
		pthread_create(&hilo_appeared, NULL, (void*) recibir_mensaje_appeared, conexion);
		pthread_detach(hilo_appeared);

		t_suscripcion* suscripcion_get = crear_t_suscripcion(id_proceso, APPEARED_POKEMON);
		void* a_enviar = empaquetar_suscripcion(suscripcion_get);
		log_info(logger, "%ld Conexion APPEARED establecida con [Broker]",
				(long) getpid());
		enviar_mensaje(conexion, a_enviar, sizeof(int) * 3);
	}
}

void recibir_mensaje_appeared(int conexion) {
	while (1) {
		t_paquete_socket* paquete =  recibir_mensajes(conexion);
		t_mensaje_appeared* mensaje_appeared;


		switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:
			procesar_mensaje_appeared(mensaje_appeared, paquete);
			break;

		case CONFIRMACION:
			log_info(logger,"Confirmacion %d",paquete->id_mensaje);
			break;

		default:
			pthread_exit(NULL);
			break;
		}
	}
}
