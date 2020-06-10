#ifndef CACHE_H_
#define CACHE_H_

#include "broker.h"
#include "../utils/listas.h"

pthread_mutex_t m_cache[6];

void* memoria_cache;
t_list* particiones_dinamicas;
int ordenamiento;

typedef struct {
	int tamanio_particion;
	bool esta_ocupado;
	int offset;
	int size_mensaje;
	struct t_particion_bs* primer_hijo;
	struct t_particion_bs* segundo_hijo;
} t_particion_bs;

typedef struct {
	int offset;
	int tamanio_particion;
	bool esta_ocupado;
} t_particion_dinamica;

typedef enum {
	PRIMER_AJUSTE = 0, MEJOR_AJUSTE = 1
} t_ordenamiento;

// Inicialización cache
void inicializar_memoria_cache();
void inicializar_mutex_cache();

// Funciones cache
void agregar_mensaje_memoria_cache(t_mensaje* mensaje);

// Funciones particion dinamica
t_particion_dinamica* buscar_particion_dinamica_libre();
t_particion_dinamica* crear_particion_dinamica(int);

// Finalización cache
void finalizar_mutex_cache();
void finalizar_memoria_cache();


#endif /* CACHE_H_ */
