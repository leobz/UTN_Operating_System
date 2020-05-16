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
void insertar_mensaje_sc(t_mensaje_sc* nuevo_mensaje, int cola_mensaje);
t_mensaje_sc* extraer_mensaje_sc(int cola_mensaje);
void insertar_mensaje_cc(t_mensaje_cc* nuevo_mensaje, int cola_mensaje);
t_mensaje_cc* extraer_mensaje_cc(int cola_mensaje);

#endif /* COLAS_H_ */
