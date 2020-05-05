#ifndef TEAM_H_
#define TEAM_H_

#include "../utils/servidor.h"
#include<commons/collections/list.h>
#include <commons/collections/dictionary.h>

typedef t_dictionary t_objetivo_global;
typedef t_dictionary t_pokemon_requeridos;

t_objetivo_global* objetivo_global;
t_pokemon_requeridos* pokemon_requeridos;

typedef struct {
	int x;
	int y;
} t_posicion;

// INICIALIZACIONES TEAM

// TODO: este método debería recibir una lista de entrenadores
void inicializar_objetivo_global();
void agregar_pokemon_a_objetivo_global(char* pokemon, int cantidad);
bool existe_pokemon_en_objetivo_global(char* pokemon);
void destruir_objetivo_global();

void crear_pokemon_requeridos();
void agregar_pokemon_a_pokemon_requeridos(char* pokemon, t_list* lista_posiciones);
bool existe_pokemon_en_pokemon_requeridos(char* pokemon);
t_list* obtener_lista_posiciones_by_pokemon_requerido(char *pokemon);
void destruir_pokemon_requeridos();
void destruir_lista_posiciones(t_list* posiciones);
void destruir_posicion(t_posicion* posicion);

// PROCESO DE MENSAJES
void procesar_mensaje_recibido(t_paquete* paquete);
void agregar_pokemon_requerido_by_mensaje_appeared(t_mensaje_appeared* mensaje);

#endif
