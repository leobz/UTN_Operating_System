#include "team.h"

void inicializar_objetivo_global() {
	objetivo_global =  dictionary_create();

	// TODO: cargar el objetivo global una vez este integrado la estructura de entrenadores
}

void agregar_pokemon_a_objetivo_global(char* pokemon, int cantidad) {
	dictionary_put(objetivo_global, pokemon, cantidad);
}

bool existe_pokemon_en_objetivo_global(char* pokemon) {
	return dictionary_has_key(objetivo_global, pokemon);
}

void destruir_objetivo_global() {
	dictionary_destroy(objetivo_global);
}


void crear_pokemon_requeridos() {
	pokemon_requeridos =  dictionary_create();
}

void agregar_pokemon_a_pokemon_requeridos(char* pokemon, t_list* lista_posiciones) {
	dictionary_put(pokemon_requeridos, pokemon, lista_posiciones);
}

bool existe_pokemon_en_pokemon_requeridos(char* pokemon) {
	return dictionary_has_key(pokemon_requeridos, pokemon);
}

t_list* obtener_lista_posiciones_by_pokemon_requerido(char* pokemon) {
	return dictionary_get(pokemon_requeridos, pokemon);
}


void destruir_posicion(t_posicion* posicion) {
	free(posicion);
}

void destruir_lista_posiciones(t_list* posiciones) {
	list_destroy_and_destroy_elements(posiciones, destruir_posicion);
}

void destruir_pokemon_requeridos() {
	dictionary_destroy_and_destroy_elements(pokemon_requeridos, destruir_lista_posiciones);
}


void procesar_mensaje_recibido(t_paquete_socket* paquete) {
	t_mensaje_appeared* mensaje_appeared;

	switch(paquete->codigo_operacion) {
		case APPEARED_POKEMON:
			mensaje_appeared = get_mensaje_appeared_by_buffer(paquete->buffer);
			agregar_pokemon_requerido_by_mensaje_appeared(mensaje_appeared);
			break;

		default:
			pthread_exit(NULL);
			break;
	}
}

void agregar_pokemon_requerido_by_mensaje_appeared(t_mensaje_appeared* mensaje) {
	if (existe_pokemon_en_objetivo_global(mensaje->pokemon)) {
		t_list* lista_posiciones;

		if(existe_pokemon_en_pokemon_requeridos(mensaje->pokemon))
			lista_posiciones = obtener_lista_posiciones_by_pokemon_requerido(mensaje->pokemon);
		else
			lista_posiciones = list_create();

		t_posicion* posicion = (t_posicion*)malloc(sizeof(t_posicion));

		posicion->x = mensaje->posx;
		posicion->y = mensaje->posy;

		list_add(lista_posiciones, posicion);

		agregar_pokemon_a_pokemon_requeridos(mensaje->pokemon, lista_posiciones);
	}
}
