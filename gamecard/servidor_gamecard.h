/*
 * servidor_gamecard.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef SERVIDOR_GAMECARD_H_
#define SERVIDOR_GAMECARD_H_

#include "directorios.h"
#include "../utils/serializacion.h"

int id_proceso_gamecard;
int finalizo_suscripcion;


// Funciones
t_datos_suscripcion* crear_datos_suscripcion(int id_proceso, int cola);
void solicitar_suscripcion(int id_proceso, int cola, int conexion);
void iniciar_hilo_suscripcion(t_datos_suscripcion* datos_suscripcion);
void iniciar_suscripcion(t_datos_suscripcion* datos_suscripcion);
void procesar_mensaje_recibido_broker(t_paquete_socket* paquete_socket);
void procesar_mensaje_recibido_cliente(t_paquete_socket* paquete_socket);
void recibir_mensajes_gamecard(t_datos_suscripcion* datos_suscripcion);
void enviar_confirmacion(int id, int confirmacion, int socket);
void preparar_confirmacion(int id_men);
void preparar_confirmacion(int id_men);
void enviar_confirmacion(int id, int confirmacion, int socket);

#endif /* SERVIDOR_GAMECARD_H_ */
