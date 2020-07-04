/*
 * administracion-de-mensajes.h
 *
 *  Created on: 15 jun. 2020
 *      Author: leo
 */

#ifndef ADMINISTRACION_DE_MENSAJES_H_
#define ADMINISTRACION_DE_MENSAJES_H_

#include "cache.h"


t_adm_mensaje* administrador_confirmado;


t_list* suscriptores[6];
t_list* administradores[6];

t_adm_mensaje*iniciar_administracion(t_mensaje*mensaje);
void *generar_mensaje(t_adm_mensaje* actual_administrator, int*bytes);
void agregar_mensaje_memoria_cache(t_adm_mensaje*, t_mensaje*);
void eliminar_adm_mensaje_particion_en_diccionarios(t_adm_mensaje* adm_mensaje);


#endif /* ADMINISTRACION_DE_MENSAJES_H_ */
