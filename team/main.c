#include "main.h"

t_list* ready;
t_list* new;
t_list* blocked;

int main(){
	inicializar_listas();


    return 0;
}

void inicializar_listas(){
	ready = list_create();
	blocked = list_create();
	new = list_create();
}
