#ifndef TEST_COMMON_TESTING_H_
#define TEST_COMMON_TESTING_H_

#include "../planificacion.h"

t_tcb_entrenador *tcb_generico();
t_tcb_entrenador* create_tcb_entrenador(int entrenador_pos_x,
		int entrenador_pos_y, int pokemon_pos_x, int pokemon_pos_y);
void destroy_tcb_entrenador(t_tcb_entrenador* tcb);
t_posicion* posicion_generica();
t_posicion* posicion(int x, int y);
t_mensaje_appeared* create_mensaje_appeared(char* pokemon, int posx, int posy);

#endif /* TEST_COMMON_TESTING_H_ */
