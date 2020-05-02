#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ESTRUCTURAS
typedef enum {
	APPEARED_POKEMON = 1,
	// TODO: Definir codigos de operacion restantes
} op_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
} t_mensaje_appeared;

// PAQUETES GENERAL
t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);

// APPEARED_POKEMON
void* serializar_appeared_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y);
t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, int pos_x, int pos_y);

t_mensaje_appeared* get_mensaje_appeared_by_buffer(t_buffer* buffer);
void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared);

#endif /* SERIALIZACION_H_ */
