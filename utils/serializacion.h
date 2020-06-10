#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

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

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
	int id_correlativo;
} t_paquete;

typedef struct{
	int posx;
	int posy;
}
t_posiciones;

typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
	int cantidad;
} t_mensaje_new;

typedef struct {
	int length_pokemon;
	char* pokemon;
} t_mensaje_get;


typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
} t_mensaje_catch;

typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
} t_mensaje_appeared;

typedef struct {
	int length_pokemon;
	char* pokemon;
	int cantidad_posiciones;
	t_posiciones pos[];
} t_mensaje_localized;

typedef struct {
	estado resultado;
} t_mensaje_caught;

typedef struct {
	op_code codigo_operacion;
	int id_correlativo;
	int id_mensaje;
	int socket_cliente;
	int id_proceso;
	int cola; //Solo para el caso q sea una suscripcion
	t_buffer* buffer;
} t_paquete_socket;

typedef struct{
	op_code confirmacion;
	int id_mensaje;
}t_confirmacion;


typedef struct {
	int cod_operacion;
	int cola_a_suscribir;
	int id_proceso;
} t_suscripcion;


// PAQUETES GENERAL
t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer,
		int id_correlativo);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);

// BUFFERS GENERAL
void* deserializar_buffer_de_un_string(t_buffer* buffer);

// NEW_POKEMON
void* serializar_new_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int cantidad, int id_correlativo);
t_buffer* buffer_new_pokemon(char* nombre_pokemon, int pos_x, int pos_y,
		int cantidad);
t_mensaje_new* deserializar_mensaje_new_pokemon(t_buffer* buffer);

// GET_POKEMON
void* serializar_get_pokemon(int* bytes, char* nombre_pokemon,
		int id_correlativo);
t_buffer* buffer_get_pokemon(char* nombre_pokemon);
t_mensaje_get* deserializar_mensaje_get_pokemon(t_buffer* buffer);

//CATCH_POKEMON
void* serializar_catch_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int id_correlativo);
t_buffer* buffer_catch_pokemon(char* nombre_pokemon, int pos_x, int pos_y);
t_mensaje_catch* deserializar_mensaje_catch_pokemon(t_buffer* buffer);
t_mensaje_catch* deserializar_paquete_catch_pokemon(void* package);

// APPEARED_POKEMON
void* serializar_appeared_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int id_correlativo);
t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, int pos_x, int pos_y);
t_mensaje_appeared* deserializar_mensaje_appeared_pokemon(t_buffer* buffer);
void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared);

//CAUGHT_POKEMON
void* serializar_caught_pokemon(int* bytes, int estado, int id_correlativo);
t_buffer* buffer_caught_pokemon(int estado);
t_mensaje_caught* deserializar_mensaje_caught_pokemon(t_buffer* buffer);

//SUSCRIPCION
void* empaquetar_suscripcion(t_suscripcion* suscripcion);

//CONFIRMACION
t_confirmacion* deserializar_confirmacion(void*mensaje);

// UTILIDADES
char* op_code_to_string(int enum_value);
int string_to_op_code(char* enum_cola);
char* value_to_state(int value);
void liberar_paquete_socket(t_paquete_socket* paquete);
#endif /* SERIALIZACION_H_ */
