#ifndef CACHE_H_
#define CACHE_H_

#include "broker.h"

pthread_mutex_t m_cache[6];

void* memoria_cache;

typedef struct {
	int tamanio_particion;
	bool esta_ocupado;
	int offset;
	int size_mensaje;
	struct t_particion_bs* primer_hijo;
	struct t_particion_bs* segundo_hijo;
} t_particion_bs;


// Inicialización cache
void inicializar_memoria_cache();
void inicializar_mutex_cache();

// Funciones cache
void agregar_mensaje_memoria_cache(t_mensaje* mensaje);

// Finalización cache
void finalizar_mutex_cache();
void finalizar_memoria_cache();


#endif /* CACHE_H_ */
