/*
 * servidor_gameboy.h
 *
 *  Created on: 30 may. 2020
 *      Author: utnso
 */

#ifndef SERVIDOR_GAMEBOY_H_
#define SERVIDOR_GAMEBOY_H_

#include "../utils/config.h"
#include "../utils/log.h"

#include "../utils/servidor.h"
#include "../utils/cliente.h"

typedef struct {
	char *ip_broker;
	char *ip_team;
	char *ip_gamecard;
	char *ip_gameboy;
	char *puerto_broker;
	char *puerto_team;
	char *puerto_gamecard;
	char *puerto_gameboy;
} t_gameboy_config;

t_gameboy_config *gameboy_config;
t_log *logger;

void* servidor_gameboy();
void desuscribir_gameboy(t_suscripcion* suscripcion, int conexion);
void procesar_mensaje_recibido(t_paquete_socket* paquete_socket);

#endif /* SERVIDOR_GAMEBOY_H_ */
