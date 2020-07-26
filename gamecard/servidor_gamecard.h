/*
 * servidor_gamecard.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef SERVIDOR_GAMECARD_H_
#define SERVIDOR_GAMECARD_H_

#include "directorios.h"

int id_proceso_gamecard;

// Funciones
void procesar_mensaje_recibido(t_paquete_socket* paquete_socket);
void servidor_gamecard(int conexion);
void enviar_confirmacion(int id, int confirmacion, int socket);
void preparar_confirmacion(int id_men);

#endif /* SERVIDOR_GAMECARD_H_ */
