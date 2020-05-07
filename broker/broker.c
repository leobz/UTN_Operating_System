/*
 * broker.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */
#include "broker.h"

void inicializar_broker(t_socket_config **broker_config, t_log **logger) {
	*broker_config = cargar_broker_config("broker.config");
	*logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);
}


void finalizar_broker(t_socket_config* broker_config, t_log* logger) {
	destruir_socket_config(broker_config);
	destruir_logger(logger);
}
