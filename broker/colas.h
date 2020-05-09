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
#include "../broker/server.h"






void insertar_new_pokemon(t_mensaje_sc *mensaje);
t_mensaje_sc *extraer_new_pokemon();
void encolar_proceso(int socket_cliente,int numero_de_cola);
void encolar_proceso_new_pokemon(int socket_cliente);
void encolar_proceso_get_pokemon(int socket_cliente);

#endif /* COLAS_H_ */
