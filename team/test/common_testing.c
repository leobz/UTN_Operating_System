#include "common_testing.h"

t_tcb_entrenador* tcb_generico(int tid) {
	t_tcb_entrenador* tcb = malloc(sizeof(t_tcb_entrenador));
	tcb->estado_tcb = NEW;
	tcb->tid = tid;
	tcb->rafaga = queue_create();

	return tcb;
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
