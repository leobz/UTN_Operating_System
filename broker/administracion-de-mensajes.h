/*
 * administracion-de-mensajes.h
 *
 *  Created on: 15 jun. 2020
 *      Author: leo
 */

#ifndef ADMINISTRACION_DE_MENSAJES_H_
#define ADMINISTRACION_DE_MENSAJES_H_

#include "broker-commons.h"
#include "cache.h"
#include "colas.h"

t_adm_mensaje* administrador_confirmado;

t_adm_mensaje*iniciar_administracion(t_mensaje*mensaje);
void *generar_mensaje(t_adm_mensaje* actual_administrator, int*bytes);
void agregar_mensaje_memoria_cache(t_adm_mensaje*, t_mensaje*);
void eliminar_adm_mensaje_para_particion(t_adm_mensaje* adm_mensaje);
void eliminar_adm_mensaje_particion_by_id(t_adm_mensaje* adm_mensaje);
void eliminar_adm_mensaje_particion_by_cod(t_adm_mensaje* adm_mensaje);


#endif /* ADMINISTRACION_DE_MENSAJES_H_ */
