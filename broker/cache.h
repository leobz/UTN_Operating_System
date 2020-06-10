#ifndef CACHE_H_
#define CACHE_H_

#include "broker.h"

pthread_mutex_t m_cache[6];

t_particion_bs* particion_bs;
void* memoria_cache;

typedef struct particion_bs {
	int tamanio_particion;
	bool esta_libre;
	int offset;
	int size_mensaje;
	struct particion_bs* primer_hijo;
	struct particion_bs* segundo_hijo;
} t_particion_bs;


// Inicialización cache
void inicializar_memoria_cache();
void inicializar_mutex_cache();

// Funciones cache
void agregar_mensaje_memoria_cache(t_mensaje* mensaje);
void obtener_hojas_libre_bs(t_list* hojas_libres, t_particion_bs* particion);
bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres);

// Finalización cache
void finalizar_mutex_cache();
void finalizar_memoria_cache();

#endif /* CACHE_H_ */
