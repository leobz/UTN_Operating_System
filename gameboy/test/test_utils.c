/*
 * test_utils.c
 *
 *  Created on: 4 jun. 2020
 *      Author: utnso
 */

#include "test_utils.h"

t_paquete* crear_paquete_desde_mensaje(void* mensaje){
	t_paquete* paquete = (t_paquete*) malloc(sizeof(t_paquete));
	paquete->buffer = (t_buffer*) malloc(sizeof(t_buffer));

	int size = 0;
	int offset = 0;


	memcpy(&(paquete->codigo_operacion), mensaje + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(paquete->id_mensaje), mensaje + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(paquete->id_correlativo), mensaje + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(size), mensaje + offset, sizeof(int));
	offset += sizeof(int);

	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(size);

	memcpy(paquete->buffer->stream, mensaje + offset, paquete->buffer->size);

	return paquete;
}
