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
	int id_proceso_gameboy;
} t_gameboy_config;

t_gameboy_config *gameboy_config;
t_log *logger;
t_log *logger_debug;
int id_proceso_gameboy;
int conexion_gameboy;


void servidor_gameboy(int conexion);
void procesar_mensaje_recibido(t_paquete_socket* paquete_socket);
void recibir_id_correlativo(int socket_cliente);
void preparar_confirmacion(int id_men);
void enviar_confirmacion(int id,op_code confirmacion,int socket);

#endif /* SERVIDOR_GAMEBOY_H_ */
