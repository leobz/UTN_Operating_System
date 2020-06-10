#include "cache.h"


void inicializar_memoria_cache() {
	memoria_cache = malloc(broker_config->tamanio_memoria);
	particion_bs = malloc(sizeof(t_particion_bs));

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
		inicializar_particiones_dinamicas();
	}
}

void inicializar_particiones_dinamicas(){
	particiones_dinamicas = list_create();
	t_particion_dinamica* particion_inicial = crear_particion_dinamica_libre(0, broker_config->tamanio_memoria);
	list_add(particiones_dinamicas, particion_inicial);
}

int es_buddy_system() {
	return strcmp(broker_config->algoritmo_memoria, "BS") == 0;
}

int es_particion_dinamica() {
	return strcmp(broker_config->algoritmo_memoria, "PARTICIONES") == 0;
}

void agregar_mensaje_memoria_cache(t_mensaje* mensaje) {
	//ESTA FUNCION DEBERIA RETORNAR UNA PARTICION O UN OFFSET A LA ADMINISTRACION DEL MENSAJE
	void* payload = mensaje->payload;
	int size = mensaje->payload_size;

	if (es_buddy_system()) {
		t_list* hojas_libres = list_create();

		obtener_hojas_libres_bs(hojas_libres, particion_bs);
		ordenar_hojas_libres_segun_algoritmo_particion_libre(hojas_libres);
	}
	else if (es_particion_dinamica()) {
		t_particion_dinamica* particion_libre = buscar_particion_dinamica_libre(size);
		guardar_en_cache(payload, particion_libre->offset, particion_libre->tamanio_particion);
	}
}

void obtener_hojas_libres_bs(t_list* hojas_libres, t_particion_bs* particion) {

	if ((particion->primer_hijo != NULL || particion->segundo_hijo != NULL) && particion->esta_libre) {
		if (particion->primer_hijo != NULL)
			obtener_hojas_libres_bs(hojas_libres, particion->primer_hijo);

		if (particion->segundo_hijo != NULL)
			obtener_hojas_libres_bs(hojas_libres, particion->segundo_hijo);
	}
	else if (particion->esta_libre) {
		list_add(hojas_libres, particion);
	}
}

bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre) {
	return hoja_libre->offset < siguiente_hoja_libre->offset;
}

bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre) {
	return hoja_libre->tamanio_particion < siguiente_hoja_libre->tamanio_particion;
}

void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres) {

	if (strcmp(broker_config->algoritmo_particion_libre, "FF") == 0) {
		list_sort(hojas_libres, es_menor_offset);
	}
	else if (strcmp(broker_config->algoritmo_particion_libre, "BF") == 0) {
		list_sort(hojas_libres, es_menor_tamanio);
	}
}

t_particion_dinamica* buscar_particion_dinamica_libre(int tamanio){
	switch (ordenamiento){
	case PRIMER_AJUSTE:

		break;
	case MEJOR_AJUSTE:
		//
		break;
	}
}

t_particion_dinamica* crear_particion_dinamica(int offset, int tamanio){
	t_particion_dinamica* particion = malloc(sizeof(t_particion_dinamica));
	particion->offset = offset;
	particion->tamanio_particion = tamanio;
	particion->esta_ocupado = 1;

	return particion;
}


t_particion_dinamica* crear_particion_dinamica_libre(int offset, int tamanio){
	t_particion_dinamica* particion = crear_particion_dinamica(offset, tamanio);
	particion->esta_ocupado = 0;

	return particion;
}

void guardar_en_cache(void* payload, int offset, int size){
	memcpy(memoria_cache + offset, payload, size);
}

void finalizar_mutex_cache() {
	for (int i = 0; i < 6; i++) {
		pthread_mutex_destroy(&m_cache[i]);
	}
}
void finalizar_memoria_cache() {
	finalizar_mutex_cache();

	free(particion_bs);
	free(memoria_cache);
}
