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

typedef struct {
	int offset;
	int tamanio_particion;
	bool esta_ocupado;
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
int es_particion_dinamica();
int es_buddy_system();

// Funciones buddy system
t_particion_bs* agregar_mensaje_memoria_cache_bs(t_mensaje* mensaje);
int obtener_tamanio_particion_necesaria (int tamanio_mensaje);
void obtener_hojas_libres_con_espacio_suficiente(t_list* hojas_libres, t_particion_bs* particion, int tamanio_particion_necesaria);
bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres);
t_particion_bs* dividir_particion_elegida (t_particion_bs* hoja_libre, int tamanio_particion_necesaria);
void cargar_particion_elegida (t_particion_bs* particion_elegida, t_mensaje mensaje);

// Funciones particion dinamica
void agregar_mensaje_memoria_cache_particion_dinamica(t_mensaje* mensaje);
t_particion_dinamica* buscar_particion_dinamica_libre(int);
t_particion_dinamica* crear_particion_dinamica(int, int);
t_particion_dinamica* crear_particion_dinamica_libre(int , int);

// Finalización cache
void finalizar_mutex_cache();
void finalizar_lista_particiones();
void finalizar_particion_bs();
void finalizar_particiones_dinamicas();
void finalizar_memoria_cache();

#endif /* CACHE_H_ */
