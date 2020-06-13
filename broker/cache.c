#include "cache.h"


void inicializar_memoria_cache() {
	memoria_cache = malloc(broker_config->tamanio_memoria);

	inicializar_mutex_cache();
	inicializar_lista_particiones();
}

void inicializar_mutex_cache() {
	if (pthread_mutex_init(&m_cache, NULL) != 0)
	{
		printf("Error en inicialización de mutex de caché\n");
	}

}

void inicializar_lista_particiones() {
	if (es_buddy_system()) {
		inicializar_particion_bs();
	}
	else if (es_particion_dinamica()) {
		inicializar_particiones_dinamicas();
	}
}

void inicializar_particion_bs() {
	particion_bs = malloc(sizeof(t_particion_bs));

	particion_bs->offset = 0;
	particion_bs->tamanio_particion = broker_config->tamanio_memoria;
	particion_bs->esta_libre = true;
	particion_bs->size_mensaje = 0;
	particion_bs->primer_hijo = NULL;
	particion_bs->segundo_hijo = NULL;
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
		list_sort(hojas_libres, (void*)es_menor_offset);
	}
	else if (strcmp(broker_config->algoritmo_particion_libre, "BF") == 0) {
		list_sort(hojas_libres, (void*)es_menor_tamanio);
	}
}


// PARTICIONES DINAMICAS

void* leer_particion_dinamica(t_particion_dinamica* particion){
	void* payload = malloc(particion->tamanio_particion);

	memcpy(payload, memoria_cache + particion->offset, particion->tamanio_particion);

	return payload;
}

t_particion_dinamica* guardar_payload_en_particion_dinamica(void *payload, int tamanio){
	t_particion_dinamica* particion_destino = buscar_particion_dinamica_libre(tamanio);

	particion_destino->esta_libre = false;
	particion_destino->tamanio_particion = tamanio;

	crear_particion_intermedia(particion_destino);

	guardar_en_cache(payload, particion_destino->offset, particion_destino->tamanio_particion);

	return particion_destino;
}

void crear_particion_intermedia(t_particion_dinamica* particion_ocupada){
	t_particion_dinamica* particion_intermedia;
	t_particion_dinamica* siguiente_particion;
	int tamanio_intermedio;
	int offset_intermedio;

	siguiente_particion = particion_ocupada->siguiente_particion;

	if (siguiente_particion != NULL) {
		tamanio_intermedio = calcular_tamanio_particion_intermedia(particion_ocupada, siguiente_particion);
	}

	else{
		tamanio_intermedio = broker_config->tamanio_memoria - particion_ocupada->tamanio_particion;
	}

	offset_intermedio = particion_ocupada->offset + particion_ocupada->tamanio_particion;

	particion_intermedia = crear_particion_dinamica_libre(offset_intermedio, tamanio_intermedio);
	list_add(particiones_dinamicas, particion_intermedia);
}

int calcular_tamanio_particion_intermedia(t_particion_dinamica* part_ocupada, t_particion_dinamica* sig_particion) {
	return sig_particion->offset - (part_ocupada->offset + part_ocupada->tamanio_particion + 1);
}


t_particion_dinamica* buscar_particion_dinamica_libre(int tamanio){
	t_list* particiones_posibles = obtener_particiones_posibles(tamanio);
	ordenar_segun_algoritmo_de_particiones_libres(particiones_posibles);

	return list_first(particiones_posibles);
}

t_list* obtener_particiones_posibles(int tamanio) {
	int particiones_eliminadas = 0;
	t_list* particiones_posibles = filtar_particiones_libres_y_suficientes(tamanio);

	while (list_is_empty(particiones_posibles)) {
		if (supero_limite_de_eliminaciones(particiones_eliminadas)) {
			compactar_particiones_dinamicas();
			particiones_eliminadas = 0;
		}
		t_list* particiones_posibles = filtar_particiones_libres_y_suficientes(tamanio);

		if (list_is_empty(particiones_posibles)) {
			eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima();
			particiones_eliminadas++;
		}
	}
	return particiones_posibles;
}

t_list* filtar_particiones_libres_y_suficientes(int tamanio) {
	t_list* particiones_libres = obtener_particiones_dinamicas_libres();
	t_list* particiones_posibles = filtrar_particiones_por_tamanio(particiones_libres, tamanio);

	free(particiones_libres);
	return particiones_posibles;
}

int supero_limite_de_eliminaciones(int particiones_eliminadas) {
	return particiones_eliminadas >= broker_config->frecuencia_compactacion;
}

void eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima(){
	// TODO
}

void compactar_particiones_dinamicas() {
	// TODO
}

t_list* filtrar_particiones_por_tamanio(t_list* particiones, int tamanio_payload) {

	int particion_es_mayor_a_tamanio_de_payload(t_particion_dinamica* particion){
		return particion->tamanio_particion >= tamanio_payload;
	}

	return list_filter(particiones, (void*) particion_es_mayor_a_tamanio_de_payload);
}

t_list* obtener_particiones_dinamicas_libres() {

	int particion_esta_libre(t_particion_dinamica* particion){
		return particion->esta_libre;
	}

	return list_filter(particiones_dinamicas, (void*) particion_esta_libre);
}

void ordenar_segun_algoritmo_de_particiones_libres(t_list* particiones){
	if (strcmp(broker_config->algoritmo_particion_libre, "FF") == 0) {
		list_sort(particiones, (void*)pd_es_menor_offset);
	}
	else if (strcmp(broker_config->algoritmo_particion_libre, "BF") == 0) {
		list_sort(particiones, (void*)pd_es_menor_tamanio);
	}
}

bool pd_es_menor_offset(t_particion_dinamica* particion, t_particion_dinamica* siguiente_particion) {
	return particion->offset < siguiente_particion->offset;
}

bool pd_es_menor_tamanio(t_particion_dinamica* particion, t_particion_dinamica* siguiente_particion) {
	return particion->tamanio_particion < siguiente_particion->tamanio_particion;
}

t_particion_dinamica* crear_particion_dinamica(int offset, int tamanio){
	t_particion_dinamica* particion = malloc(sizeof(t_particion_dinamica));
	particion->offset = offset;
	particion->tamanio_particion = tamanio;
	particion->esta_libre = false;
	particion->siguiente_particion = NULL;

	return particion;
}

t_particion_dinamica* crear_particion_dinamica_libre(int offset, int tamanio){
	t_particion_dinamica* particion = crear_particion_dinamica(offset, tamanio);
	particion->esta_libre = true;

	return particion;
}

void guardar_en_cache(void* payload, int offset, int size){
	memcpy(memoria_cache + offset, payload, size);
}

void finalizar_mutex_cache() {
	pthread_mutex_destroy(&m_cache);
}

void finalizar_lista_particiones() {
	if (es_buddy_system()) {
		finalizar_particion_bs();
	}
	else if (es_particion_dinamica()) {
		finalizar_particiones_dinamicas();
	}
}

void finalizar_particion_bs() {
	// TODO: implementar logica eliminacion particion bs
}

static void particion_dinamica_destroy(t_particion_dinamica* particion) {
    free(particion);
}

void finalizar_particiones_dinamicas() {
	list_destroy_and_destroy_elements(particiones_dinamicas, particion_dinamica_destroy);
}

void finalizar_memoria_cache() {
	finalizar_mutex_cache();
	finalizar_lista_particiones();

	free(memoria_cache);
}
