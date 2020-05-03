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


void insertar_new_pokemon(t_mensaje_sc* mensaje);
t_mensaje_sc* extraer_new_pokemon();

#endif /* COLAS_H_ */
