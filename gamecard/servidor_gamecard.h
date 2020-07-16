/*
 * servidor_gamecard.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef SERVIDOR_GAMECARD_H_
#define SERVIDOR_GAMECARD_H_

#include "directorios.h"

// Funciones
void procesar_mensaje_recibido(t_paquete_socket* paquete_socket);
void servidor_gamecard(int conexion);

#endif /* SERVIDOR_GAMECARD_H_ */
