#include "team_suscripcion.h"

t_suscripcion* crear_t_suscripcion(int id_proceso, int cola) {
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
	suscripcion->cod_operacion = SUSCRIPCION;
	suscripcion->cola_a_suscribir = cola;
	suscripcion->id_proceso = id_proceso;
	return suscripcion;
}

void recibir_mensajes_team(int conexion) {
	while (1) {
		t_paquete_socket* paquete =  recibir_mensajes(conexion);
		procesar_mensaje_recibido_broker(paquete);
	}
}

void iniciar_suscripcion(int id_proceso, int conexion, int cola) {
	id_proceso = id_proceso * 10 + cola;
	if (conexion == -1) {
		printf("ERROR: Conexion %s con [Broker] no establecida\n", op_code_to_string(cola));
	}
	else {
		pthread_t hilo_suscripcion;
		pthread_create(&hilo_suscripcion, NULL, (void*) recibir_mensajes_team, conexion);
		pthread_detach(hilo_suscripcion);

		t_suscripcion* suscripcion = crear_t_suscripcion(id_proceso, cola);
		void* a_enviar = empaquetar_suscripcion(suscripcion);
		log_info(logger, "Conexion %s establecida con [Broker]", op_code_to_string(cola));
		enviar_mensaje(conexion, a_enviar, sizeof(int) * 3);
	}
}

