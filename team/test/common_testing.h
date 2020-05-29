#ifndef TEST_COMMON_TESTING_H_
#define TEST_COMMON_TESTING_H_

#include "../planificacion.h"

t_tcb_entrenador *tcb_generico();
t_posicion* posicion_generica();
t_posicion* posicion(int x, int y);
t_mensaje_appeared* create_mensaje_appeared(char* pokemon, int posx, int posy);

#endif /* TEST_COMMON_TESTING_H_ */
