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
#include <stdbool.h>



t_cola_proceso *cola_procesos[6][4];
t_mensaje*mensajes[6][3];
int proceso_vacio[6][2];
int contador_procesos[6];

typedef enum{VACIA,ULTIMO}process;

typedef enum{FRENTE,FIN,ACTUAL,AUXILIAR}punteros;

void encolar_proceso(int socket, int cola);
t_cola_proceso* desencolar_proceso(int cola);
void insertar_mensaje(t_mensaje* nuevo_mensaje, int cola_mensaje);
t_mensaje* extraer_mensaje(int cola_mensaje);

#endif /* COLAS_H_ */
