#ifndef CACHE_H_
#define CACHE_H_

#include "broker.h"
#include "../utils/listas.h"

typedef struct particion_bs {
	int tamanio_particion;
	bool esta_libre;
	int offset;
	int size_mensaje;
	struct particion_bs* primer_hijo;
	struct particion_bs* segundo_hijo;
} t_particion_bs;

typedef struct particion_dinamica {
	int offset;
	int tamanio_particion;
	bool esta_libre;
	struct particion_dinamica* siguiente_particion;
} t_particion_dinamica;

typedef enum {
	PRIMER_AJUSTE = 0, MEJOR_AJUSTE = 1
} t_ordenamiento;

pthread_mutex_t m_cache[6];

t_particion_bs* particion_bs;
void* memoria_cache;
t_list* particiones_dinamicas;
int ordenamiento;

// Inicialización cache
void inicializar_memoria_cache();
void inicializar_mutex_cache();
void inicializar_lista_particiones();
void inicializar_particion_bs();
void inicializar_particiones_dinamicas();

// Funciones cache
void agregar_mensaje_memoria_cache(t_mensaje* mensaje);
int es_particion_dinamica();
int es_buddy_system();
void guardar_en_cache(void* payload, int offset, int size);

// Funciones buddy system
void obtener_hojas_libres_bs(t_list* hojas_libres, t_particion_bs* particion);
bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres);

// Funciones particion dinamica
t_particion_dinamica* buscar_particion_dinamica_libre(int);
t_list* obtener_particiones_dinamicas_libres();
t_particion_dinamica* crear_particion_dinamica(int, int);
t_particion_dinamica* crear_particion_dinamica_libre(int , int);
void ordenar_segun_algoritmo_de_particiones_libres(t_list*);
bool pd_es_menor_offset(t_particion_dinamica*, t_particion_dinamica*);
bool pd_es_menor_tamanio(t_particion_dinamica*, t_particion_dinamica*);
t_list* filtrar_particiones_por_tamanio(t_list*, int);
void eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima();
void compactar_particiones_dinamicas();
t_list* filtar_particiones_libres_y_suficientes(int);
t_list* obtener_particiones_posibles(int);


// Finalización cache
void finalizar_mutex_cache();
void finalizar_lista_particiones();
void finalizar_particion_bs();
void finalizar_particiones_dinamicas();
void finalizar_memoria_cache();

#endif /* CACHE_H_ */
