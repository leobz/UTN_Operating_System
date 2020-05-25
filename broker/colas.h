/*
 * colas.h
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#ifndef COLAS_H_
#define COLAS_H_

#include<stdio.h>
#include<stdlib.h>
#include "server.h"

typedef enum{FRENTE,FIN}punteros;

void encolar_proceso(int socket, int cola);
void insertar_mensaje(t_mensaje* nuevo_mensaje, int cola_mensaje);
t_mensaje* extraer_mensaje(int cola_mensaje);

#endif /* COLAS_H_ */
