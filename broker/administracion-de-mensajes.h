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

typedef struct{
	int id_mensaje;
	int id_correlativo;
	op_code codigo_operacion;
	t_particion_bs* particion_bs;
	t_particion_dinamica *particion_dinamica;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
}
t_adm_mensaje;

t_adm_mensaje* administrador_confirmado;

t_adm_mensaje*iniciar_administracion(t_mensaje*mensaje);
void *generar_mensaje(t_adm_mensaje* actual_administrator, int*bytes);
void agregar_mensaje_memoria_cache(t_adm_mensaje*, t_mensaje*);



#endif /* ADMINISTRACION_DE_MENSAJES_H_ */
