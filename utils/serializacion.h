#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <commons/string.h>
#include "commons/collections/list.h"
#include <inttypes.h>

// ESTRUCTURAS

typedef enum {
	OP_ERROR = -1,
	NEW_POKEMON = 0,
	GET_POKEMON = 1,
	CATCH_POKEMON = 2,
	APPEARED_POKEMON = 3,
	LOCALIZED_POKEMON = 4,
	CAUGHT_POKEMON = 5,
	SUSCRIPCION =6,
	CONFIRMACION = 7
// TODO: Definir codigos de operacion restantes
} op_code;

typedef enum {
	FAIL = 0, OK = 1
} estado;

typedef enum {
	SIN = 0, CON = 1
} barra_cero;

typedef struct {
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
	uint32_t id_mensaje;
	uint32_t id_correlativo;
} t_paquete;

typedef struct{
	uint32_t posx;
	uint32_t posy;
}
t_posiciones;

typedef struct {
	uint32_t length_pokemon;
	char* pokemon;
	uint32_t posx;
	uint32_t posy;
	uint32_t cantidad;
} t_mensaje_new;

typedef struct {
	uint32_t length_pokemon;
	char* pokemon;
} t_mensaje_get;


typedef struct {
	uint32_t length_pokemon;
	char* pokemon;
	uint32_t posx;
	uint32_t posy;
} t_mensaje_catch;

typedef struct {
	uint32_t length_pokemon;
	char* pokemon;
	uint32_t posx;
	uint32_t posy;
} t_mensaje_appeared;

typedef struct {
	uint32_t length_pokemon;
	char* pokemon;
	uint32_t cantidad_posiciones;
	t_posiciones pos[];
} t_mensaje_localized;

typedef struct {
	uint32_t id_correlativo;
	estado resultado;
} t_mensaje_caught;

typedef struct {
	op_code codigo_operacion;
	uint32_t id_correlativo;
	uint32_t id_mensaje;
	uint32_t socket_cliente;
	uint32_t id_proceso;
	uint32_t cola; //Solo para el caso q sea una suscripcion
	t_buffer* buffer;
} t_paquete_socket;

typedef struct{
	op_code confirmacion;
	uint32_t id_mensaje;
}t_confirmacion;


typedef struct {
	uint32_t cod_operacion;
	uint32_t cola_a_suscribir;
	uint32_t id_proceso;
} t_suscripcion;

typedef struct {
	uint32_t id_proceso;
	uint32_t conexion;
	uint32_t cola;
} t_datos_suscripcion;


// PAQUETES GENERAL
t_paquete* crear_paquete(uint32_t codigo_operacion, t_buffer* buffer, uint32_t id_mensaje,
		uint32_t id_correlativo);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, uint32_t bytes);
void* serializar_segun_codigo_sin_barra(void*payload,op_code codigo,uint32_t*size);
void* serializar_segun_codigo_con_barra(void* payload,op_code codigo,uint32_t*size);
// BUFFERS GENERAL
void* deserializar_buffer_de_un_string(t_buffer* buffer);

// NEW_POKEMON
void* serializar_new_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,
		uint32_t pos_y, uint32_t cantidad,uint32_t id_mensaje, uint32_t id_correlativo);
t_buffer* buffer_new_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad);
t_mensaje_new* deserializar_mensaje_new_pokemon(t_buffer* buffer);
void eliminar_mensaje_new(t_mensaje_new* mensaje_new);
t_mensaje_new* deserializar_paquete_new_pokemon(void* package);
void* payload_new_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad);
void* payload_new_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad);
t_mensaje_new* deserializar_payload_new_pokemon(void*payload);
t_mensaje_new* deserializar_payload_new_pokemon_con_barra(void*payload);

// GET_POKEMON
void* serializar_get_pokemon(uint32_t* bytes, char* nombre_pokemon,uint32_t id_mensaje,uint32_t id_correlativo);
t_buffer* buffer_get_pokemon(char* nombre_pokemon);
t_mensaje_get* deserializar_mensaje_get_pokemon(t_buffer* buffer);
void eliminar_mensaje_get(t_mensaje_get* mensaje_get);
t_mensaje_get* deserializar_paquete_get_pokemon(void* package);
t_buffer* buffer_get_sin_barra(char* nombre_pokemon);
void* payload_get_con_barra(char* nombre_pokemon);
void* payload_get_sin_barra(char* nombre_pokemon);
t_buffer* buffer_get_(char* pokemon);
t_mensaje_get* deserializar_payload_get_pokemon(void *payload);
t_mensaje_get* deserializar_payload_get_pokemon_con_barra(void *payload);
//CATCH_POKEMON
void* serializar_catch_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,
uint32_t pos_y, uint32_t id_mensaje,uint32_t id_correlativo);
t_buffer* buffer_catch_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
t_mensaje_catch* deserializar_mensaje_catch_pokemon(t_buffer* buffer);
t_mensaje_catch* deserializar_paquete_catch_pokemon(void* package);
void eliminar_mensaje_catch(t_mensaje_catch* mensaje_catch);
void* payload_catch_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
void* payload_catch_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
t_mensaje_catch* deserializar_payload_catch_pokemon(void*payload);
t_mensaje_catch* deserializar_payload_catch_pokemon_con_barra(void*payload);
// APPEARED_POKEMON
void* serializar_appeared_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,
		uint32_t pos_y,uint32_t id_mensaje, uint32_t id_correlativo);
t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
t_mensaje_appeared* deserializar_mensaje_appeared_pokemon(t_buffer* buffer);
void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared);
t_mensaje_appeared* deserializar_paquete_appeared_pokemon(void* package);
void* payload_appeared_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
void* payload_appeared_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);
t_mensaje_appeared* deserializar_payload_appeared_pokemon(void*payload);
t_mensaje_appeared* deserializar_payload_appeared_pokemon_con_barra(void*payload);

//LOCALIZED_POKEMON

t_buffer* buffer_localized_pokemon(char* nombre_pokemon, t_list* posiciones);
void* serializar_localized_pokemon(uint32_t* bytes, char* nombre_pokemon, t_list* posiciones,uint32_t id_mensaje, uint32_t id_correlativo);
t_mensaje_localized* deserializar_mensaje_localized_pokemon(t_buffer* buffer);
t_mensaje_localized* deserializar_paquete_localized_pokemon(void* package);
void* payload_localized_sin_barra(char* nombre_pokemon, uint32_t cantidad_posiciones,t_posiciones pos[]);
t_mensaje_localized* deserializar_payload_localized_pokemon(void* package);
void* payload_localized_con_barra(char* nombre_pokemon, uint32_t cantidad_posiciones,t_posiciones pos[]);
t_mensaje_localized* deserializar_payload_localized_pokemon_con_barra(void* package);
void eliminar_mensaje_localized(t_mensaje_localized* mensaje_localized);

//CAUGHT_POKEMON
void* serializar_caught_pokemon(uint32_t* bytes, uint32_t estado,uint32_t id_mensaje, uint32_t id_correlativo);
t_buffer* buffer_caught_pokemon(uint32_t estado);
t_mensaje_caught* deserializar_mensaje_caught_pokemon(t_buffer* buffer);
t_mensaje_caught* deserializar_paquete_caught_pokemon(void* package);
t_buffer* buffer_caught_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y);

//SUSCRIPCION
void* empaquetar_suscripcion(t_suscripcion* suscripcion);

//CONFIRMACION
t_confirmacion* deserializar_confirmacion(void*mensaje);

// UTILIDADES
char* op_code_to_string(uint32_t enum_value);
uint32_t string_to_op_code(char* enum_cola);
char* value_to_state(uint32_t value);
char* pasar_a_char(uint32_t numero);
void liberar_paquete_socket(t_paquete_socket* paquete);
t_suscripcion* crear_t_suscripcion(uint32_t id_proceso, uint32_t cola);
#endif /* SERIALIZACION_H_ */
