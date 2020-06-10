#include "cache.h"


void inicializar_memoria_cache() {
	memoria_cache = malloc(broker_config->tamanio_memoria);

	inicializar_mutex_cache();
	inicializar_lista_particiones();
}

void inicializar_mutex_cache() {
	for (int i = 0; i < 6; i++) {
		if (pthread_mutex_init(&m_cache[i], NULL) != 0)
		{
			printf("Error en inicialización de mutex: %s (%d)\n", op_code_to_string(i), i);
			break;
		}
	}
}

void inicializar_lista_particiones() {
	if (es_buddy_system()) {
		// inicializacion bs
	}
	else if (es_particion_dinamica()) {
		particiones_dinamicas = list_create();
	}
}

int es_buddy_system() {
	return strcmp(broker_config->algoritmo_memoria, "BS") == 0;
}

int es_particion_dinamica() {
	return strcmp(broker_config->algoritmo_memoria, "PARTICIONES") == 0;
}

void agregar_mensaje_memoria_cache(t_mensaje* mensaje) {
	if (es_buddy_system()) {
		// logica de buddy system
	}
	else if (es_particion_dinamica()) {
		t_particion_dinamica* particion_libre = buscar_particion_dinamica_libre();
	}
}

t_particion_dinamica* buscar_particion_dinamica_libre(){
	if (list_is_empty(particiones_dinamicas)){
		t_particion_dinamica* particion = crear_particion_dinamica(broker_config->tamanio_memoria);
	}
	else{
		switch (ordenamiento){
		case PRIMER_AJUSTE:

			break;
		case MEJOR_AJUSTE:
			//
			break;
		}
	}

}

t_particion_dinamica* crear_particion_dinamica(int tamanio){

}

void finalizar_mutex_cache() {
	for (int i = 0; i < 6; i++) {
		pthread_mutex_destroy(&m_cache[i]);
	}
}
void finalizar_memoria_cache() {
	finalizar_mutex_cache();

	free(memoria_cache);
}
