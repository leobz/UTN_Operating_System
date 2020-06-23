#ifndef CACHE_H_
#define CACHE_H_

#include "broker-commons.h"
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

pthread_mutex_t m_cache;

t_particion_bs* particion_bs;
void* memoria_cache;
t_list* particiones_dinamicas;
int ordenamiento;


// ********************************** INICIALIZACION MEMORIA CACHE ********************************** //
void inicializar_memoria_cache();
void inicializar_mutex_cache();
void inicializar_lista_particiones();
void inicializar_particion_bs();
void inicializar_particiones_dinamicas();
int obtener_tamanio_memoria_cache_bs();


// ********************************** FUNCIONES MEMORIA CACHE ********************************** //
int es_particion_dinamica();
int es_buddy_system();
void guardar_en_cache(void* payload, int offset, int size);


// ********************************** FUNCIONES BUDDY SYSTEM *********************************** //
t_particion_bs* agregar_mensaje_memoria_cache_bs(t_mensaje* mensaje);
int obtener_tamanio_particion_necesaria (int tamanio_mensaje);
void obtener_hojas_libres_con_espacio_suficiente(t_list* hojas_libres, t_particion_bs* particion, int tamanio_particion_necesaria);
bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres);
t_particion_bs* dividir_particion_elegida (t_particion_bs* hoja_libre, int tamanio_particion_necesaria);
void cargar_particion_elegida (t_particion_bs* particion_elegida, t_mensaje* mensaje);
void* leer_particion_bs(t_particion_bs* particion);


// ********************************** FUNCIONES PARTICIONES DINAMICAS ********************************** //
t_particion_dinamica* agregar_mensaje_memoria_cache_particion_dinamica(t_mensaje* mensaje);
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
t_particion_dinamica* guardar_payload_en_particion_dinamica(void*, int);
void* leer_particion_dinamica(t_particion_dinamica*);
void reubicar_particion(t_particion_dinamica* particion_din,int hueco_particiones);


// ********************************** FINALIZACION MEMORIA CACHE ********************************** //
void finalizar_mutex_cache();
void finalizar_lista_particiones();
void elimimar_particiones_bs(t_particion_bs* particion);
void finalizar_particiones_dinamicas();
void finalizar_memoria_cache();


#endif /* CACHE_H_ */
