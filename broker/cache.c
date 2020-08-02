#include "cache.h"

// ********************************** INICIALIZACION MEMORIA CACHE ********************************** //

void inicializar_memoria_cache() {
	memoria_cache = malloc(broker_config->tamanio_memoria);

	id_particion = 0;
	contador_uso = 0;
	orden_creacion = 0;

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

	id_particion++;

	particion_bs->id = id_particion;
	particion_bs->offset = 0;
	particion_bs->tamanio_particion = obtener_tamanio_memoria_cache_bs();
	particion_bs->esta_libre = true;
	particion_bs->size_mensaje = 0;
	particion_bs->contador_uso = 0;
	particion_bs->orden_creacion = 0;
	particion_bs->adm_mensaje = NULL;
	particion_bs->padre = NULL;
	particion_bs->primer_hijo = NULL;
	particion_bs->segundo_hijo = NULL;
}

int obtener_tamanio_memoria_cache_bs() {
	int tamanio_memoria_cache_config = broker_config->tamanio_memoria;
	int tamanio_memoria_cache_real = 2;
	bool memoria_insuficiente = true;

	while (memoria_insuficiente) {
		if ((tamanio_memoria_cache_real * 2) <= tamanio_memoria_cache_config)
			tamanio_memoria_cache_real *= 2;
		else
			memoria_insuficiente = false;
	}

	return tamanio_memoria_cache_real;
}

void inicializar_particiones_dinamicas(){
	particiones_dinamicas = list_create();
	t_particion_dinamica* particion_inicial = crear_particion_dinamica_libre(0, broker_config->tamanio_memoria);
	list_add(particiones_dinamicas, particion_inicial);
}

// ********************************** FUNCIONES MEMORIA CACHE ********************************** //

int es_buddy_system() {
	return strcmp(broker_config->algoritmo_memoria, "BS") == 0;
}

int es_particion_dinamica() {
	return strcmp(broker_config->algoritmo_memoria, "PARTICIONES") == 0;
}

int es_fifo(){
	return strcmp(broker_config->algoritmo_reemplazo, "FIFO") == 0;
}

int es_lru(){
	return strcmp(broker_config->algoritmo_reemplazo, "LRU") == 0;
}

int es_first_fit(){
	return strcmp(broker_config->algoritmo_particion_libre, "FF") == 0;
}

int es_best_fit(){
	return strcmp(broker_config->algoritmo_particion_libre, "BF") == 0;
}



void guardar_en_cache(void* payload, int offset, int size){
	memmove(memoria_cache + offset, payload, size);

}

void dump_cache(int senial){
	switch(senial){
		case SIGUSR1:
			if (es_buddy_system())
				generar_archivo_dump_bs();
			else if (es_particion_dinamica())
				generar_archivo_dump_particion_dinamica();
			break;
		default:
			break;
	}
}

char* obtener_fecha_actual(){
	time_t t;
	struct tm* local_time;
	char* fecha = malloc(sizeof(20));

	t = time(NULL);
	local_time = localtime(&t);
	strftime(fecha, 20, "%d/%m/%Y", local_time);

	return fecha;
}

char* obtener_hora_actual(){
	time_t t;
	struct tm* local_time;
	char* hora = malloc(sizeof(20));

	t = time(NULL);
	local_time = localtime(&t);
	strftime(hora, 20, "%H:%M:%S", local_time);

	return hora;

}

// ********************************** FUNCIONES BUDDY SYSTEM *********************************** //

t_particion_bs* agregar_mensaje_memoria_cache_bs(t_mensaje* mensaje, t_adm_mensaje* adm_mensaje) {
	t_list* hojas_libres = list_create();
	t_particion_bs* particion_elegida = NULL;
	void* payload = mensaje->payload;

	int payload_size = mensaje->payload_size;
	int tamanio_particion_necesaria = obtener_tamanio_particion_necesaria(payload_size);
	bool sin_hojas_libres = true;

	while (sin_hojas_libres) {
		obtener_hojas_libres_con_espacio_suficiente(hojas_libres, particion_bs, tamanio_particion_necesaria);

		if (list_size(hojas_libres) == 0) {
			liberar_hoja_segun_algoritmo_reemplazo();
		}
		else{
			sin_hojas_libres = false;
		}
	}
	ordenar_hojas_libres_segun_algoritmo_particion_libre(hojas_libres);
	particion_elegida = dividir_particion_elegida (list_first(hojas_libres), tamanio_particion_necesaria);
	cargar_particion_elegida(particion_elegida, payload_size, adm_mensaje);

	guardar_en_cache(payload, particion_elegida->offset, particion_elegida->size_mensaje);

	list_clean(hojas_libres);
	free(hojas_libres);

	return particion_elegida;
}

t_particion_bs* agregar_mensaje_memoria_cache_bs_barra_cero(t_mensaje* mensaje, t_adm_mensaje* adm_mensaje) {
	t_list* hojas_libres = list_create();
	t_particion_bs* particion_elegida = NULL;
	int size=mensaje->payload_size;
	void* payload = serializar_segun_codigo_sin_barra(mensaje->payload,mensaje->codigo_operacion,&size);
	int payload_size = size;
	int tamanio_particion_necesaria = obtener_tamanio_particion_necesaria(payload_size);
	bool sin_hojas_libres = true;

	while (sin_hojas_libres) {
		obtener_hojas_libres_con_espacio_suficiente(hojas_libres, particion_bs, tamanio_particion_necesaria);

		if (list_size(hojas_libres) == 0) {
			liberar_hoja_segun_algoritmo_reemplazo();
		}
		else{
			sin_hojas_libres = false;
		}
	}
	ordenar_hojas_libres_segun_algoritmo_particion_libre(hojas_libres);
	particion_elegida = dividir_particion_elegida (list_first(hojas_libres), tamanio_particion_necesaria);
	cargar_particion_elegida(particion_elegida, payload_size, adm_mensaje);
	guardar_en_cache(payload, particion_elegida->offset, particion_elegida->size_mensaje);

	list_clean(hojas_libres);
	free(hojas_libres);

	return particion_elegida;
}


int obtener_tamanio_particion_necesaria (int tamanio_mensaje) {
	int tamanio_particion_necesaria = 2;

	while (tamanio_particion_necesaria < broker_config->tamanio_minimo_particion || tamanio_particion_necesaria < tamanio_mensaje) {
		tamanio_particion_necesaria *= 2;
	}

	return tamanio_particion_necesaria;
}

void obtener_hojas_libres_con_espacio_suficiente(t_list* hojas_libres, t_particion_bs* particion, int tamanio_particion_necesaria) {

	if (particion->primer_hijo != NULL || particion->segundo_hijo != NULL) {

		if (particion->primer_hijo != NULL)
			obtener_hojas_libres_con_espacio_suficiente(hojas_libres, particion->primer_hijo, tamanio_particion_necesaria);

		if (particion->segundo_hijo != NULL)
			obtener_hojas_libres_con_espacio_suficiente(hojas_libres, particion->segundo_hijo, tamanio_particion_necesaria);
	}
	else if (particion->esta_libre && particion->tamanio_particion >= tamanio_particion_necesaria) {
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

t_particion_bs* dividir_particion_elegida (t_particion_bs* hoja_libre, int tamanio_particion_necesaria) {

	if ((hoja_libre->tamanio_particion / 2) >= tamanio_particion_necesaria) {
		t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
		t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));


		id_particion++;

		primer_hijo->id = id_particion;
		primer_hijo->esta_libre = true;
		primer_hijo->offset = hoja_libre->offset;
		primer_hijo->tamanio_particion = hoja_libre->tamanio_particion / 2;
		primer_hijo->size_mensaje = 0;
		primer_hijo->contador_uso = 0;
		primer_hijo->orden_creacion = 0;
		primer_hijo->adm_mensaje = NULL;
		primer_hijo->padre = hoja_libre;
		primer_hijo->primer_hijo = NULL;
		primer_hijo->segundo_hijo = NULL;


		id_particion++;

		segundo_hijo->id = id_particion;
		segundo_hijo->esta_libre = true;
		segundo_hijo->offset = hoja_libre->offset + hoja_libre->tamanio_particion / 2;
		segundo_hijo->tamanio_particion = hoja_libre->tamanio_particion / 2;
		segundo_hijo->size_mensaje = 0;
		segundo_hijo->contador_uso = 0;
		segundo_hijo->orden_creacion = 0;
		segundo_hijo->adm_mensaje = NULL;
		segundo_hijo->padre = hoja_libre;
		segundo_hijo->primer_hijo = NULL;
		segundo_hijo->segundo_hijo = NULL;

		hoja_libre->primer_hijo = primer_hijo;
		hoja_libre->segundo_hijo = segundo_hijo;
		hoja_libre->esta_libre = false;

		return dividir_particion_elegida(hoja_libre->primer_hijo, tamanio_particion_necesaria);
	}

	return hoja_libre;
}

void cargar_particion_elegida (t_particion_bs* particion_elegida, int payload_size, t_adm_mensaje* adm_mensaje) {
	contador_uso++;
	orden_creacion++;

	particion_elegida->esta_libre = false;
	particion_elegida->size_mensaje = payload_size;
	particion_elegida->contador_uso = contador_uso;
	particion_elegida->orden_creacion = orden_creacion;
	particion_elegida->adm_mensaje = adm_mensaje;
}

void* leer_particion_bs(t_particion_bs* particion) {
	void* payload = malloc(particion->tamanio_particion);
	contador_uso++;

	particion->contador_uso = contador_uso;
	memcpy(payload, memoria_cache + particion->offset, particion->size_mensaje);

	return payload;
}

void liberar_hoja_segun_algoritmo_reemplazo(){
	t_list* hojas_posibles_victimas = list_create();
	t_particion_bs* particion_victima = NULL;

	obtener_hojas_posibles_victimas(hojas_posibles_victimas, particion_bs);
	ordenar_hojas_posibles_victimas_segun_algoritmo_reemplazo(hojas_posibles_victimas);
	particion_victima = list_first(hojas_posibles_victimas);
	liberar_particion_victima(particion_victima);
	consolidar_particiones_companieros(particion_victima);
}

void obtener_hojas_posibles_victimas(t_list* hojas_posibles_victimas, t_particion_bs* particion) {
	if (particion->primer_hijo != NULL || particion->segundo_hijo != NULL) {

		if (particion->primer_hijo != NULL)
			obtener_hojas_posibles_victimas(hojas_posibles_victimas, particion->primer_hijo);

		if (particion->segundo_hijo != NULL)
			obtener_hojas_posibles_victimas(hojas_posibles_victimas, particion->segundo_hijo);
	}
	else if (!particion->esta_libre) {
		list_add(hojas_posibles_victimas, particion);
	}
}

bool es_menor_orden_creacion(t_particion_bs* hoja_posible_victima, t_particion_bs* siguiente_hoja_posible_victima) {
	return hoja_posible_victima->orden_creacion < siguiente_hoja_posible_victima->orden_creacion;
}

bool es_menor_contador_uso(t_particion_bs* hoja_posible_victima, t_particion_bs* siguiente_hoja_posible_victima) {
	return hoja_posible_victima->contador_uso < siguiente_hoja_posible_victima->contador_uso;
}


void ordenar_hojas_posibles_victimas_segun_algoritmo_reemplazo(t_list* hojas_posibles_victimas){
	if (strcmp(broker_config->algoritmo_reemplazo, "FIFO") == 0) {
		list_sort(hojas_posibles_victimas, (void*)es_menor_orden_creacion);
	}
	else if (strcmp(broker_config->algoritmo_reemplazo, "LRU") == 0) {
		list_sort(hojas_posibles_victimas, (void*)es_menor_contador_uso);
	}
}

void liberar_particion_victima(t_particion_bs* particion_victima){
	t_adm_mensaje* adm_mensaje = particion_victima->adm_mensaje;

	eliminar_adm_mensaje_particion_en_diccionarios(adm_mensaje);

	orden_creacion++;

	particion_victima->orden_creacion = orden_creacion;
	particion_victima->esta_libre = true;
	particion_victima->size_mensaje = 0;
	particion_victima->adm_mensaje = NULL;

	log_info(logger,"[BUDDY SYSTEM] Eliminación de partición con posición de inicio: %d", particion_victima->offset);
}

void consolidar_particiones_companieros(t_particion_bs* particion_victima){
	t_particion_bs* particion_padre = particion_victima->padre;
	t_particion_bs* particion_companiera = NULL;

	if (particion_padre->primer_hijo != particion_victima){
		particion_companiera = particion_padre->primer_hijo;
	}
	else if (particion_padre->segundo_hijo != particion_victima){
		particion_companiera = particion_padre->segundo_hijo;
	}

	if (particion_companiera->esta_libre){
		particion_padre->esta_libre = true;
		particion_padre->primer_hijo = NULL;
		particion_padre->segundo_hijo = NULL;

		log_info(logger,"[BUDDY SYSTEM] Asociación de particiones con inicio en %d y %d",
					particion_victima->offset,
					particion_companiera->offset);

		free(particion_victima);
		elimimar_particiones_bs(particion_companiera);

		if (particion_padre->padre != NULL){
			consolidar_particiones_companieros(particion_padre);
		}
	}
}

void generar_archivo_dump_bs(){
	FILE* archivo_dump;
	char* fecha_actual = obtener_fecha_actual();
	char* hora_actual = obtener_hora_actual();

	log_info(logger,"[BUDDY SYSTEM] Solicitud de Dump de cache");

	archivo_dump = fopen("dump_cache.txt", "a+");

	if (archivo_dump != NULL){
		t_list* hojas_particion_bs = list_create();

		fputs("---------------------------------------------------------------------------------------------------------------------------------------\n", archivo_dump);
		fprintf(archivo_dump, "Dump:\t\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t\t%s\n", fecha_actual, hora_actual);

		obtener_hojas_bs(hojas_particion_bs, particion_bs);
		agregar_contenido_bs_archivo_dump(archivo_dump, hojas_particion_bs);

		fputs("---------------------------------------------------------------------------------------------------------------------------------------\n", archivo_dump);
	}

	free(hora_actual);
	free(fecha_actual);

	fclose(archivo_dump);
}

void obtener_hojas_bs(t_list* hojas_particion_bs, t_particion_bs* particion){
	if (particion->primer_hijo != NULL || particion->segundo_hijo != NULL) {

		if (particion->primer_hijo != NULL)
			obtener_hojas_bs(hojas_particion_bs, particion->primer_hijo);

		if (particion->segundo_hijo != NULL)
			obtener_hojas_bs(hojas_particion_bs, particion->segundo_hijo);
	}
	else {
		list_add(hojas_particion_bs, particion);
	}
}

void agregar_contenido_bs_archivo_dump(FILE* archivo_dump, t_list* hojas_particion_bs){
	int i = 1;

	char obtener_caracter_libre(t_particion_bs* particion){
		if (particion->esta_libre)
			return 'L';
		else
			return 'X';
	}

	void escribir_archivo_dump(t_particion_bs* particion){

		if (particion->esta_libre)
			fprintf(archivo_dump, "Partición %d: 0x%03x - 0x%03x.\t\t[%c]\t\tSize: %d b\n",
						i, particion->offset, particion->offset + particion->tamanio_particion - 1, obtener_caracter_libre(particion),
						particion->tamanio_particion);
		else
			fprintf(archivo_dump, "Partición %d: 0x%03x - 0x%03x.\t\t[%c]\t\tSize: %d b\t\tLRU:%d\t\tCola:%d\t\tID:%d\n",
						i, particion->offset, particion->offset + particion->tamanio_particion - 1, obtener_caracter_libre(particion),
						particion->tamanio_particion, particion->contador_uso, particion->adm_mensaje->codigo_operacion,
						particion->id);

		i++;
	}

	list_iterate(hojas_particion_bs, (void*) escribir_archivo_dump);
}


// ********************************** FUNCIONES PARTICIONES DINAMICAS ********************************** //


t_particion_dinamica* agregar_mensaje_memoria_cache_particion_dinamica(t_mensaje* mensaje,t_adm_mensaje* adm) {
	void* payload = mensaje->payload;
	int size = obtener_particion_minima_suficiente(mensaje->payload_size, mensaje->codigo_operacion);

	return guardar_payload_en_particion_dinamica_con_adm(payload, size,adm);
}

t_particion_dinamica* agregar_mensaje_memoria_cache_particion_dinamica_barra_cero(t_mensaje* mensaje, t_adm_mensaje* admin) {

	int size=obtener_particion_minima_suficiente(mensaje->payload_size, mensaje->codigo_operacion);

	void* payload = serializar_segun_codigo_sin_barra(mensaje->payload,mensaje->codigo_operacion,&size);

	t_particion_dinamica*part_dinamica=guardar_payload_en_particion_dinamica_con_adm(payload, size, admin);
	free(payload);
	return part_dinamica;
}

int obtener_particion_minima_suficiente(int tamanio_mensaje, op_code cod_op) {
	if (tamanio_mensaje > broker_config->tamanio_minimo_particion)
		return tamanio_mensaje;
	else {
		if (cod_op == CAUGHT_POKEMON)
			return broker_config->tamanio_minimo_particion;
		else
			return broker_config->tamanio_minimo_particion + 1;
	}
}

void* leer_particion_dinamica(t_particion_dinamica* particion){

	if(es_lru())
			particion->contador_uso=++contador_uso;

	void* payload = malloc(particion->tamanio_particion);

	memmove(payload, memoria_cache + particion->offset, particion->tamanio_particion);

	return payload;
}

t_particion_dinamica* guardar_payload_en_particion_dinamica_con_adm(void *payload, int tamanio, t_adm_mensaje *admin){
	//printf("entro a buscar libre\n");

	t_particion_dinamica* particion_destino = buscar_particion_dinamica_libre(tamanio);
	//printf("pude entrar a buscar libre\n");
	particion_destino->esta_libre = false;
	particion_destino->tamanio_particion = tamanio;
	particion_destino->contador_uso=++contador_uso;
	particion_destino->adm_mensaje=admin;

	//printf("Almacenando mensaje en cache en posicion %d\n",particion_destino->offset);
	crear_particion_intermedia(particion_destino);

	guardar_en_cache(payload, particion_destino->offset, particion_destino->tamanio_particion);


	return particion_destino;
}
//ESTA FUNCION ES SOLO PARA EL TEST POR DIFERENCIA DE PARAMETROS
t_particion_dinamica* guardar_payload_en_particion_dinamica(void *payload, int tamanio){
	t_particion_dinamica* particion_destino = buscar_particion_dinamica_libre(tamanio);

	particion_destino->esta_libre = false;
	particion_destino->tamanio_particion = tamanio;

	crear_particion_intermedia(particion_destino);

	guardar_en_cache(payload, particion_destino->offset, particion_destino->tamanio_particion);

	return particion_destino;

}

int calcular_tamanio_particion_intermedia(t_particion_dinamica* part_ocupada, t_particion_dinamica* sig_particion) {
	return sig_particion->offset - (part_ocupada->offset + part_ocupada->tamanio_particion);
}


t_particion_dinamica* buscar_particion_dinamica_libre(int tamanio){
	t_list* particiones_posibles = obtener_particiones_posibles(tamanio);
	ordenar_segun_algoritmo_de_particiones_libres(particiones_posibles);
	t_particion_dinamica*part=list_first(particiones_posibles);
	free(particiones_posibles);
	return part;
}


t_list* obtener_particiones_posibles(int tamanio) {
	int particiones_eliminadas = 0;

	t_list* particiones_posibles = filtar_particiones_libres_y_suficientes(tamanio);

	while (particiones_posibles==NULL) {
		free(particiones_posibles);
		if (supero_limite_de_eliminaciones(particiones_eliminadas)) {
			compactar_particiones_dinamicas();
			particiones_eliminadas = 0;
		}
		//printf("Sep");
		particiones_posibles = filtar_particiones_libres_y_suficientes(tamanio);
		//printf("Particiones posibles %d",list_size(particiones_posibles));

		if (particiones_posibles==NULL) {
			free(particiones_posibles);
			eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima();
			particiones_posibles = filtar_particiones_libres_y_suficientes(tamanio);
			particiones_eliminadas++;
		}
	}
	return particiones_posibles;
}
t_list* filtar_particiones_libres_y_suficientes(int tamanio) {
	t_list* particiones_libres = obtener_particiones_dinamicas_libres();
	if(list_is_empty(particiones_libres)){
		free(particiones_libres);
		return NULL;}

	t_list* particiones_posibles = filtrar_particiones_por_tamanio(particiones_libres, tamanio);

	if(list_is_empty(particiones_posibles)){
		free(particiones_libres);
		free(particiones_posibles);
		return NULL;}

	free(particiones_libres);
	return particiones_posibles;
}

void crear_particion_intermedia(t_particion_dinamica* particion_ocupada){
	t_particion_dinamica* particion_intermedia;
	t_particion_dinamica* siguiente_particion;
	int tamanio_intermedio;
	int offset_intermedio;

	siguiente_particion = particion_ocupada->siguiente_particion;
	particion_intermedia=NULL;

	if (siguiente_particion != NULL){
		tamanio_intermedio = calcular_tamanio_particion_intermedia(particion_ocupada, siguiente_particion);
	}
	else{
		tamanio_intermedio = broker_config->tamanio_memoria - (particion_ocupada->offset + particion_ocupada->tamanio_particion);
	}

	offset_intermedio = particion_ocupada->offset + particion_ocupada->tamanio_particion;

//	printf("Offset restante %d\n",offset_intermedio);
//	printf("Tamanio restante %d\n",tamanio_intermedio);

	//verificar que el tamaño de a particion sea mayor que el minimo dado en el archivo de config
	if(tamanio_intermedio>=broker_config->tamanio_minimo_particion){
		particion_intermedia = crear_particion_dinamica_libre(offset_intermedio, tamanio_intermedio);
		particion_intermedia->siguiente_particion=particion_ocupada->siguiente_particion;
		particion_ocupada->siguiente_particion=particion_intermedia;
		list_add(particiones_dinamicas, particion_intermedia);
	}
	else{
		particion_ocupada->siguiente_particion=NULL;
		//printf("No hay mas espacio\n");
		}

}

int supero_limite_de_eliminaciones(int particiones_eliminadas) {
	return particiones_eliminadas >= broker_config->frecuencia_compactacion;
}

bool pd_es_menor_contador_uso(t_particion_dinamica* particion, t_particion_dinamica* siguiente_particion) {
	return particion->contador_uso < siguiente_particion->contador_uso;
}

void eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima(){


	int particion_esta_libre(t_particion_dinamica* particion){
			return !particion->esta_libre;
		}

	t_list* particiones_ocupadas=list_filter(particiones_dinamicas, (void*) particion_esta_libre);
	list_sort(particiones_ocupadas, (void*)pd_es_menor_contador_uso);
	t_particion_dinamica* particion_victima= list_first(particiones_ocupadas);

		liberar_particion_dinamica(particion_victima);
		free(particiones_ocupadas);
		unir_particiones_dinamicas_libres();//consolidacion

}

void liberar_particion_dinamica(t_particion_dinamica* particion_victima){
	t_adm_mensaje* adm_mensaje = particion_victima->adm_mensaje;

	eliminar_adm_mensaje_particion_en_diccionarios(adm_mensaje);

	particion_victima->esta_libre = true;
	particion_victima->adm_mensaje = NULL;

	log_info(logger,"[PARTICIONES DINAMICAS] Eliminación de partición con posición de inicio: %d", particion_victima->offset);
}


void unir_particiones_dinamicas_libres(){

	t_list* particiones_libres=obtener_particiones_dinamicas_libres();

	list_sort(particiones_libres,(void*)pd_es_menor_offset);

	int i=0;
	void unir_libres(t_particion_dinamica* dinamica){
		printf("[PARTICIONES DINAMICAS] Inicio funcion unir_libres() con iteracion: %d\n", i);
		if(dinamica->siguiente_particion != NULL){
			printf("[PARTICIONES DINAMICAS] Particion 'dinamica' ¿es NULL?: %d\n", dinamica == NULL);
			printf("[PARTICIONES DINAMICAS] Particion 'dinamica->siguiente_particion' ¿es NULL?: %d\n", dinamica->siguiente_particion == NULL);
			printf("[PARTICIONES DINAMICAS] Particion 'dinamica->siguiente_particion' ¿esta libre?: %d\n", dinamica->siguiente_particion->esta_libre);
			if(dinamica->siguiente_particion->esta_libre){

				int counter=dinamica->orden_creacion;
				dinamica->siguiente_particion->tamanio_particion+=dinamica->tamanio_particion;
				dinamica->siguiente_particion->offset=dinamica->offset;

				bool tiene_mismo_orden_creacion(t_particion_dinamica* particion_din){
						return particion_din->orden_creacion== counter;
					}

				list_remove_by_condition(particiones_dinamicas, (void*)tiene_mismo_orden_creacion);

				free(dinamica);

			}
		}
		i++;
	}
	list_iterate(particiones_libres, (void*)unir_libres);

	free(particiones_libres);
}

void compactar_particiones_dinamicas(){

	log_info(logger,"[PARTICIONES DINAMICAS] Compactación de particiones");

	//printf("Entro a compactar\n");
	int offset_hueco=0;

	t_list* particiones_ocupadas=obtener_particiones_dinamicas_ocupadas();

	list_sort(particiones_ocupadas, (void*)pd_es_menor_offset);


	void compactar(t_particion_dinamica* particion_din){
		reubicar_particion(particion_din,offset_hueco);
		particion_din->offset=offset_hueco;
		offset_hueco=particion_din->offset + particion_din->tamanio_particion; //para la proxima particion a compactar
	}

	list_iterate(particiones_ocupadas, (void*)compactar);


	eliminar_particiones_libres();

	int offset_libre=offset_hueco; // ubico el offset al final
	int tamanio_restante=broker_config->tamanio_memoria-offset_libre; //de la ultima particion

	if(tamanio_restante>=broker_config->tamanio_minimo_particion){
			t_particion_dinamica*particion_final = crear_particion_dinamica_libre(offset_libre, tamanio_restante);
			list_add(particiones_dinamicas, particion_final);}

	free(particiones_ocupadas);

}

void reubicar_particion(t_particion_dinamica* particion_din,int hueco_particiones){
	memmove(memoria_cache+hueco_particiones,memoria_cache+particion_din->offset,particion_din->tamanio_particion);
}

void eliminar_particiones_libres(){
	t_list* particiones_libres=obtener_particiones_dinamicas_libres();

	void eliminar_particion(t_particion_dinamica* dinamica_libre){

		void eliminar_particion_free(t_particion_dinamica* dynamic){
			free(dynamic);
		}

		bool tiene_mismo_orden_creacion(t_particion_dinamica* particion_din){
			return particion_din->orden_creacion == dinamica_libre->orden_creacion;
		}

		list_remove_and_destroy_by_condition(particiones_dinamicas, (void*)tiene_mismo_orden_creacion,(void*)eliminar_particion_free);
	}

	list_iterate(particiones_libres, (void*)eliminar_particion);

	free(particiones_libres);
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

t_list* obtener_particiones_dinamicas_ocupadas() {

	int particion_esta_ocupada(t_particion_dinamica* particion){
		return !particion->esta_libre;
	}

	return list_filter(particiones_dinamicas, (void*) particion_esta_ocupada);
}

void ordenar_segun_algoritmo_de_particiones_libres(t_list* particiones){
	if (es_first_fit()){
		list_sort(particiones, (void*)pd_es_menor_offset);
	}
	else if (es_best_fit()) {
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
	particion->orden_creacion=++orden_creacion;

	return particion;
}

t_particion_dinamica* crear_particion_dinamica_libre(int offset, int tamanio){
	t_particion_dinamica* particion = crear_particion_dinamica(offset, tamanio);
	particion->esta_libre = true;

	return particion;
}

void generar_archivo_dump_particion_dinamica() {
	FILE* archivo_dump;
	char* fecha_actual = obtener_fecha_actual();
	char* hora_actual = obtener_hora_actual();

	archivo_dump = fopen("dump_cache.txt", "a+");

	if (archivo_dump != NULL){

		fputs("---------------------------------------------------------------------------------------------------------------------------------------\n", archivo_dump);
		fprintf(archivo_dump, "Dump:\t\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t\t%s\n", fecha_actual, hora_actual);

		agregar_contenido_particion_dinamica_archivo_dump(archivo_dump);

		fputs("---------------------------------------------------------------------------------------------------------------------------------------\n", archivo_dump);
	}

	free(hora_actual);
	free(fecha_actual);

	fclose(archivo_dump);
}

void agregar_contenido_particion_dinamica_archivo_dump(FILE* archivo_dump){
	int i = 1;

	char obtener_caracter_libre(t_particion_dinamica* particion){
		if (particion->esta_libre)
			return 'L';
		else
			return 'X';
	}

	list_sort(particiones_dinamicas, (void*)pd_es_menor_offset);

	void escribir_archivo_dump(t_particion_dinamica* particion){

		if (particion->esta_libre)
			fprintf(archivo_dump, "Partición %d: 0x%03x - 0x%03x.\t\t[%c]\t\tSize: %d b\n",
						i, particion->offset, particion->offset + particion->tamanio_particion - 1, obtener_caracter_libre(particion),
						particion->tamanio_particion);
		else
			fprintf(archivo_dump, "Partición %d: 0x%03x - 0x%03x.\t\t[%c]\t\tSize: %d b\t\tLRU:%d\t\tCola:%d\t\tID:%d\n",
						i, particion->offset, particion->offset + particion->tamanio_particion - 1, obtener_caracter_libre(particion),
						particion->tamanio_particion, particion->contador_uso, particion->adm_mensaje->codigo_operacion,
						particion->orden_creacion);

		/*
		 * TODO: Agregar los siguientes campos en el struct 't_particion_dinamica'
		 * - id: correspondiente a la particion
		 *
		 * Una vez agregado, cambiarlo en el codigo del else
		*/

		i++;
	}

	list_iterate(particiones_dinamicas, (void*) escribir_archivo_dump);
}

// ********************************** FINALIZACION MEMORIA CACHE ********************************** //

void finalizar_mutex_cache() {
	pthread_mutex_destroy(&m_cache);
}

void finalizar_lista_particiones() {
	if (es_buddy_system()) {
		elimimar_particiones_bs(particion_bs);
	}
	else if (es_particion_dinamica()) {
		finalizar_particiones_dinamicas();
	}
}

void elimimar_particiones_bs(t_particion_bs* particion) {

	if (particion->primer_hijo != NULL && particion->segundo_hijo != NULL) {
		t_particion_bs* primer_hijo = particion->primer_hijo;
		t_particion_bs* segundo_hijo = particion->segundo_hijo;

		free(particion);

		elimimar_particiones_bs(primer_hijo);
		elimimar_particiones_bs(segundo_hijo);
	}
	else {
		free(particion);
	}

}

static void particion_dinamica_destroy(t_particion_dinamica* particion) {
    free(particion);
}

void finalizar_particiones_dinamicas() {
	list_destroy_and_destroy_elements(particiones_dinamicas, (void*)particion_dinamica_destroy);
}

void finalizar_memoria_cache() {
	finalizar_mutex_cache();
	finalizar_lista_particiones();

	free(memoria_cache);
}
