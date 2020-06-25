#include "common_testing.h"

t_tcb_entrenador* tcb_generico(int tid) {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	tcb->estado_tcb = NEW;
	tcb->tid = tid;
	tcb->rafaga = queue_create();
	tcb->posicion = malloc(sizeof(t_posicion));
	tcb->posicion->x = 0;
	tcb->posicion->y = 0;


	return tcb;
}

t_tcb_entrenador* create_tcb_entrenador(int entrenador_pos_x,
		int entrenador_pos_y, int pokemon_pos_x, int pokemon_pos_y) {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	tcb->posicion = malloc(sizeof(t_posicion));
	tcb->pokemon_a_capturar = malloc(sizeof(t_pokemon));
	tcb->pokemon_a_capturar->posicion = malloc(sizeof(t_posicion));

	tcb->posicion->x = entrenador_pos_x;
	tcb->posicion->y = entrenador_pos_y;

	tcb->pokemon_a_capturar->posicion->x = pokemon_pos_x;
	tcb->pokemon_a_capturar->posicion->y = pokemon_pos_y;

	return tcb;
}

void destroy_tcb_entrenador(t_tcb_entrenador* tcb) {
	free(tcb->pokemon_a_capturar->posicion);
	free(tcb->pokemon_a_capturar);
	free(tcb->posicion);
	free(tcb);
}

t_posicion* posicion_generica() {
	t_posicion* posicion;
	posicion->x = 1;
	posicion->y = 1;
	return posicion;
}

t_posicion* posicion(int x, int y) {
	t_posicion* posicion;
	posicion->x = x;
	posicion->y = y;
	return posicion;
}

t_mensaje_appeared* create_mensaje_appeared(char* pokemon, int posx, int posy) {
	t_mensaje_appeared* mensaje_appeared = malloc(sizeof(t_mensaje_appeared));

	mensaje_appeared->length_pokemon = strlen(pokemon);
	mensaje_appeared->pokemon = pokemon;
	mensaje_appeared->posx = posx;
	mensaje_appeared->posy = posy;

	return mensaje_appeared;
}
