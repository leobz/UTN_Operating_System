#include "cache.h"

void inicializar_memoria_cache() {
	memoria_cache = malloc(broker_config->tamanio_memoria);

	inicializar_mutex_cache();
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

void agregar_mensaje_memoria_cache(t_mensaje* mensaje) {
	if (strcmp(broker_config->algoritmo_memoria, "BS") == 0) {
		// logica de buddy system
	}
	else if (strcmp(broker_config->algoritmo_memoria, "PARTICIONES") == 0) {
			// logica de particionamiento dinamico
	}
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
