#include "test_buddy_system.h"

void agregar_tests_buddy_system() {
	CU_pSuite suite_configuracion = CU_add_suite("Memoria cache - Buddy System", NULL, NULL);

	CU_add_test(suite_configuracion,
			"En base al tamanio de la memoria en el config, obtener el tamanio real de la memoria para bs",
			test_obtener_tamanio_real_memoria_cache_segun_config);

	CU_add_test(suite_configuracion,
			"En base al tamanio del mensaje, obtener el tamanio de particion necesaria",
			test_obtener_tamanio_particion_necesario_segun_tamanio_mensaje);

	CU_add_test(suite_configuracion,
			"Elegir las particiones libres para guardar un mensaje",
			test_hojas_libres_para_guardar_mensaje);

	CU_add_test(suite_configuracion,
			"Ordenar las hojas libres segun el algoritmo de particion libre: FF",
			test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_FF);

	CU_add_test(suite_configuracion,
			"Ordenar las hojas libres segun el algoritmo de particion libre: BF",
			test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_BF);

	CU_add_test(suite_configuracion,
			"Dividir particion elegida hasta ser el minimo superior al tamanio necesario para el mensaje",
			test_division_particion_elegida_acorde_a_tamanio_mensaje);

	CU_add_test(suite_configuracion,
			"Obtener las hojas que pueden ser posibles victimas para reemplazo",
			test_obtener_posibles_victimas_reemplazo);

	CU_add_test(suite_configuracion,
			"Ordernar las hojas que pueden ser posibles victimas segun algoritmo de reemplazo: FIFO",
			test_ordenar_hojas_posibles_victimas_segun_FIFO);

	CU_add_test(suite_configuracion,
			"Ordernar las hojas que pueden ser posibles victimas segun algoritmo de reemplazo: LRU",
			test_ordenar_hojas_posibles_victimas_segun_LRU);

	CU_add_test(suite_configuracion,
			"Liberar particion elegida como victima de reemplazo",
			test_liberar_particion_victima);

	CU_add_test(suite_configuracion,
			"Consolidar particion victima con companieros",
			test_consolidar_particion_victima_companieros);

	CU_add_test(suite_configuracion,
			"Liberar hojas usando el algoritmo de reeemplazo sin tener en cuenta la consolidacion",
			test_liberar_hoja_sin_compactacion_segun_algoritmo_reemplazo);

	CU_add_test(suite_configuracion,
			"Liberar hojas usando el algoritmo de reeemplazo teniendo en cuenta la consolidacion",
			test_liberar_hoja_con_compactacion_segun_algoritmo_reemplazo);

	CU_add_test(suite_configuracion,
				"Agregar mensaje a memoria cache mediante bs",
				test_agregar_mensaje_a_memoria_cache_bs);

	CU_add_test(suite_configuracion,
			"Agregar mensaje a memoria cache con un reemplazo y consolidacion mediante bs",
			test_agregar_mensaje_con_reemplazo_consolidacion_a_memoria_cache_bs);

	CU_add_test(suite_configuracion,
			"Leer mensaje guardado en memoria cache mediante bs",
			test_leer_mensaje_de_memoria_cache_bs);
}

void inicializar_test_bs() {
	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = 1200;
	free(broker_config->algoritmo_memoria);
	broker_config->algoritmo_memoria = strdup("BS");

	administracion_por_id = dictionary_create();
	administracion_por_cod = dictionary_create();

	for (int j = 0; j < 6; j++){
		administradores[j]=list_create();
		meter_en_diccionario(administracion_por_cod,j,administradores[j]);
	}

	inicializar_memoria_cache();
}

void finalizar_test_bs() {

	void eliminar_administracion_por_id(t_adm_mensaje* adm_mensaje){
		free(adm_mensaje);
	}

	dictionary_destroy_and_destroy_elements(administracion_por_id, (void*)eliminar_administracion_por_id);

	void eliminar_administracion_por_cod(t_list* l_adm_mensajes){

		void eliminar_lista_adm_mensajes(t_adm_mensaje* adm_mensaje){
			free(adm_mensaje);
		}

		list_destroy_and_destroy_elements(l_adm_mensajes, (void*)eliminar_lista_adm_mensajes);
	}

	dictionary_destroy_and_destroy_elements(administracion_por_cod, (void*)eliminar_administracion_por_cod);

	finalizar_memoria_cache();
	destruir_broker_config(broker_config);
}

void test_obtener_tamanio_real_memoria_cache_segun_config() {
	inicializar_test_bs();
	int tamanio_memoria_cache_real = 0;

	tamanio_memoria_cache_real = obtener_tamanio_memoria_cache_bs();

	CU_ASSERT_EQUAL_FATAL(tamanio_memoria_cache_real, 1024);
	finalizar_test_bs();
}

void test_obtener_tamanio_particion_necesario_segun_tamanio_mensaje() {
	int tamanio_mensaje = 70;
	int tamanio_particion_necesaria = 0;

	tamanio_particion_necesaria = obtener_tamanio_particion_necesaria(tamanio_mensaje);

	CU_ASSERT_EQUAL_FATAL(tamanio_particion_necesaria, 128);
}

void test_hojas_libres_para_guardar_mensaje() {
	inicializar_test_bs();

	int tamanio_particion_necesaria = 256;
	t_list* hojas_libres = list_create();
	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));

	primer_hijo_primer_hijo->esta_libre = true;
	primer_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_primer_hijo->primer_hijo = NULL;
	primer_hijo_primer_hijo->segundo_hijo = NULL;
	primer_hijo_segundo_hijo->esta_libre = true;
	primer_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_segundo_hijo->primer_hijo = NULL;
	primer_hijo_segundo_hijo->segundo_hijo = NULL;

	primer_hijo->esta_libre = true;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->primer_hijo = primer_hijo_primer_hijo;
	primer_hijo->segundo_hijo = primer_hijo_segundo_hijo;

	segundo_hijo_primer_hijo->esta_libre = false;
	segundo_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_primer_hijo->primer_hijo = NULL;
	segundo_hijo_primer_hijo->segundo_hijo = NULL;
	segundo_hijo_segundo_hijo->esta_libre = true;
	segundo_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_segundo_hijo->primer_hijo = NULL;
	segundo_hijo_segundo_hijo->segundo_hijo = NULL;

	segundo_hijo->esta_libre = true;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = segundo_hijo_primer_hijo;
	segundo_hijo->segundo_hijo = segundo_hijo_segundo_hijo;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	obtener_hojas_libres_con_espacio_suficiente(hojas_libres, particion_bs, tamanio_particion_necesaria);

	CU_ASSERT_EQUAL_FATAL(list_size(hojas_libres), 3);
	CU_ASSERT_EQUAL_FATAL(list_get(hojas_libres, 0), primer_hijo_primer_hijo);
	CU_ASSERT_EQUAL_FATAL(list_get(hojas_libres, 1), primer_hijo_segundo_hijo);
	CU_ASSERT_EQUAL_FATAL(list_get(hojas_libres, 2), segundo_hijo_segundo_hijo);

	list_clean(hojas_libres);
	free(hojas_libres);

	finalizar_test_bs();
}

void test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_FF() {
	inicializar_test_bs();
	free(broker_config->algoritmo_particion_libre);
	broker_config->algoritmo_particion_libre = strdup("FF");

	t_list* hojas_libres = list_create();
	t_particion_bs* primer_hoja_libre = malloc(sizeof(t_particion_bs));
	t_particion_bs* segunda_hoja_libre = malloc(sizeof(t_particion_bs));
	t_particion_bs* tercera_hoja_libre = malloc(sizeof(t_particion_bs));

	primer_hoja_libre->offset = 0;
	segunda_hoja_libre->offset = 256;
	tercera_hoja_libre->offset = 512;

	list_add(hojas_libres, segunda_hoja_libre);
	list_add(hojas_libres, tercera_hoja_libre);
	list_add(hojas_libres, primer_hoja_libre);

	ordenar_hojas_libres_segun_algoritmo_particion_libre(hojas_libres);

	CU_ASSERT_EQUAL_FATAL(list_get(hojas_libres, 0), primer_hoja_libre);

	free(primer_hoja_libre);
	free(segunda_hoja_libre);
	free(tercera_hoja_libre);
	list_clean(hojas_libres);
	free(hojas_libres);
	finalizar_test_bs();
}

void test_hojas_libres_ordenadas_segun_algoritmo_particion_libre_BF() {
	inicializar_test_bs();
	free(broker_config->algoritmo_particion_libre);
	broker_config->algoritmo_particion_libre = strdup("BF");

	t_list* hojas_libres = list_create();
	t_particion_bs* primer_hoja_libre = malloc(sizeof(t_particion_bs));
	t_particion_bs* segunda_hoja_libre = malloc(sizeof(t_particion_bs));
	t_particion_bs* tercera_hoja_libre = malloc(sizeof(t_particion_bs));

	primer_hoja_libre->tamanio_particion = 256;
	segunda_hoja_libre->tamanio_particion = 128;
	tercera_hoja_libre->tamanio_particion = 512;

	list_add(hojas_libres, tercera_hoja_libre);
	list_add(hojas_libres, segunda_hoja_libre);
	list_add(hojas_libres, primer_hoja_libre);

	ordenar_hojas_libres_segun_algoritmo_particion_libre(hojas_libres);

	CU_ASSERT_EQUAL_FATAL(list_get(hojas_libres, 0), segunda_hoja_libre);

	free(primer_hoja_libre);
	free(segunda_hoja_libre);
	free(tercera_hoja_libre);
	list_clean(hojas_libres);
	free(hojas_libres);
	finalizar_test_bs();
}

void test_division_particion_elegida_acorde_a_tamanio_mensaje() {
	inicializar_test_bs();

	t_particion_bs* particion_elegida = NULL;
	int tamanio_particion_necesaria = 256;

	particion_elegida = dividir_particion_elegida(particion_bs, tamanio_particion_necesaria);

	CU_ASSERT_EQUAL_FATAL(particion_elegida->esta_libre, true);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->offset, 0);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->tamanio_particion, tamanio_particion_necesaria);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->primer_hijo, NULL);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->segundo_hijo, NULL);

	CU_ASSERT_NOT_EQUAL_FATAL(particion_bs->primer_hijo, NULL);
	CU_ASSERT_NOT_EQUAL_FATAL(particion_bs->segundo_hijo, NULL);

	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->offset, 0);
	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->tamanio_particion, 512);
	CU_ASSERT_EQUAL_FATAL(particion_bs->segundo_hijo->offset, 512);
	CU_ASSERT_EQUAL_FATAL(particion_bs->segundo_hijo->tamanio_particion, 512);

	CU_ASSERT_NOT_EQUAL_FATAL(particion_bs->primer_hijo->primer_hijo, NULL);
	CU_ASSERT_NOT_EQUAL_FATAL(particion_bs->primer_hijo->segundo_hijo, NULL);
	CU_ASSERT_EQUAL_FATAL(particion_bs->segundo_hijo->primer_hijo, NULL);
	CU_ASSERT_EQUAL_FATAL(particion_bs->segundo_hijo->segundo_hijo, NULL);

	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->primer_hijo->offset, 0);
	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->primer_hijo->tamanio_particion, 256);
	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->segundo_hijo->offset, 256);
	CU_ASSERT_EQUAL_FATAL(particion_bs->primer_hijo->segundo_hijo->tamanio_particion, 256);

	finalizar_test_bs();
}

void test_obtener_posibles_victimas_reemplazo(){
	inicializar_test_bs();

	t_list* hojas_posibles_victimas = list_create();
	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));

	primer_hijo_primer_hijo->esta_libre = true;
	primer_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_primer_hijo->primer_hijo = NULL;
	primer_hijo_primer_hijo->segundo_hijo = NULL;
	primer_hijo_segundo_hijo->esta_libre = false;
	primer_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_segundo_hijo->size_mensaje = 30;
	primer_hijo_segundo_hijo->primer_hijo = NULL;
	primer_hijo_segundo_hijo->segundo_hijo = NULL;

	primer_hijo->esta_libre = true;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->primer_hijo = primer_hijo_primer_hijo;
	primer_hijo->segundo_hijo = primer_hijo_segundo_hijo;

	segundo_hijo_primer_hijo->esta_libre = false;
	segundo_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_primer_hijo->size_mensaje = 40;
	segundo_hijo_primer_hijo->primer_hijo = NULL;
	segundo_hijo_primer_hijo->segundo_hijo = NULL;
	segundo_hijo_segundo_hijo->esta_libre = true;
	segundo_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_segundo_hijo->primer_hijo = NULL;
	segundo_hijo_segundo_hijo->segundo_hijo = NULL;

	segundo_hijo->esta_libre = true;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = segundo_hijo_primer_hijo;
	segundo_hijo->segundo_hijo = segundo_hijo_segundo_hijo;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	obtener_hojas_posibles_victimas(hojas_posibles_victimas, particion_bs);

	CU_ASSERT_EQUAL_FATAL(list_size(hojas_posibles_victimas), 2);
	CU_ASSERT_EQUAL_FATAL(list_get(hojas_posibles_victimas, 0), primer_hijo_segundo_hijo);
	CU_ASSERT_EQUAL_FATAL(list_get(hojas_posibles_victimas, 1), segundo_hijo_primer_hijo);

	list_clean(hojas_posibles_victimas);
	free(hojas_posibles_victimas);

	finalizar_test_bs();
}

void test_ordenar_hojas_posibles_victimas_segun_FIFO(){
	inicializar_test_bs();
	free(broker_config->algoritmo_reemplazo);
	broker_config->algoritmo_reemplazo = strdup("FIFO");

	t_list* hojas_posibles_victimas = list_create();
	t_particion_bs* primer_hoja = malloc(sizeof(t_particion_bs));
	t_particion_bs* segunda_hoja = malloc(sizeof(t_particion_bs));
	t_particion_bs* tercera_hoja = malloc(sizeof(t_particion_bs));

	primer_hoja->orden_creacion = 15;
	segunda_hoja->orden_creacion = 3;
	tercera_hoja->orden_creacion = 7;

	list_add(hojas_posibles_victimas, tercera_hoja);
	list_add(hojas_posibles_victimas, segunda_hoja);
	list_add(hojas_posibles_victimas, primer_hoja);

	ordenar_hojas_posibles_victimas_segun_algoritmo_reemplazo(hojas_posibles_victimas);

	CU_ASSERT_EQUAL(list_get(hojas_posibles_victimas,0), segunda_hoja);

	free(primer_hoja);
	free(segunda_hoja);
	free(tercera_hoja);
	list_clean(hojas_posibles_victimas);
	free(hojas_posibles_victimas);

	finalizar_test_bs();
}

void test_ordenar_hojas_posibles_victimas_segun_LRU(){
	inicializar_test_bs();
	free(broker_config->algoritmo_reemplazo);
	broker_config->algoritmo_reemplazo = strdup("LRU");

	t_list* hojas_posibles_victimas = list_create();
	t_particion_bs* primer_hoja = malloc(sizeof(t_particion_bs));
	t_particion_bs* segunda_hoja = malloc(sizeof(t_particion_bs));
	t_particion_bs* tercera_hoja = malloc(sizeof(t_particion_bs));

	primer_hoja->contador_uso = 4;
	segunda_hoja->contador_uso = 12;
	tercera_hoja->contador_uso = 0;

	list_add(hojas_posibles_victimas, tercera_hoja);
	list_add(hojas_posibles_victimas, segunda_hoja);
	list_add(hojas_posibles_victimas, primer_hoja);

	ordenar_hojas_posibles_victimas_segun_algoritmo_reemplazo(hojas_posibles_victimas);

	CU_ASSERT_EQUAL(list_get(hojas_posibles_victimas,0), tercera_hoja);

	free(primer_hoja);
	free(segunda_hoja);
	free(tercera_hoja);
	list_clean(hojas_posibles_victimas);
	free(hojas_posibles_victimas);

	finalizar_test_bs();
}

void test_liberar_particion_victima(){
	inicializar_test_bs();
	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* particion_victima = malloc(sizeof(t_particion_bs));

	adm_mensaje->id_mensaje = 1;
	adm_mensaje->codigo_operacion = NEW_POKEMON;
	adm_mensaje->suscriptores_enviados = list_create();
	adm_mensaje->suscriptores_confirmados = list_create();

	dictionary_put(administracion_por_id, "1", adm_mensaje);

	list_add(administradores[adm_mensaje->codigo_operacion], adm_mensaje);

	particion_victima->esta_libre = false;
	particion_victima->size_mensaje = 30;
	particion_victima->adm_mensaje = adm_mensaje;

	liberar_particion_victima(particion_victima);

	CU_ASSERT_EQUAL(particion_victima->esta_libre, true);
	CU_ASSERT_EQUAL(particion_victima->size_mensaje, 0);
	CU_ASSERT_EQUAL(particion_victima->adm_mensaje, NULL);
	CU_ASSERT_TRUE_FATAL(dictionary_get(administracion_por_id, "1") == NULL);
	CU_ASSERT_TRUE_FATAL(list_size(administradores[NEW_POKEMON]) == NULL);

	finalizar_test_bs();
}

void test_consolidar_particion_victima_companieros() {
	inicializar_test_bs();

	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));

	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));


	primer_hijo->esta_libre = true;
	primer_hijo->padre = particion_bs;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->primer_hijo = NULL;
	primer_hijo->segundo_hijo = NULL;

	segundo_hijo_primer_hijo->esta_libre = true;
	segundo_hijo_primer_hijo->padre = segundo_hijo;
	segundo_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_primer_hijo->primer_hijo = NULL;
	segundo_hijo_primer_hijo->segundo_hijo = NULL;
	segundo_hijo_segundo_hijo->esta_libre = true;
	segundo_hijo_segundo_hijo->padre = segundo_hijo;
	segundo_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	segundo_hijo_segundo_hijo->primer_hijo = NULL;
	segundo_hijo_segundo_hijo->segundo_hijo = NULL;

	segundo_hijo->esta_libre = true;
	segundo_hijo->padre = particion_bs;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = segundo_hijo_primer_hijo;
	segundo_hijo->segundo_hijo = segundo_hijo_segundo_hijo;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	consolidar_particiones_companieros(primer_hijo);

	CU_ASSERT_TRUE_FATAL(particion_bs->primer_hijo == NULL);
	CU_ASSERT_TRUE_FATAL(particion_bs->segundo_hijo == NULL);

	finalizar_test_bs();
}

void test_liberar_hoja_sin_compactacion_segun_algoritmo_reemplazo(){
	inicializar_test_bs();

	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));

	adm_mensaje->id_mensaje = 1;
	adm_mensaje->codigo_operacion = NEW_POKEMON;
	adm_mensaje->suscriptores_enviados = list_create();
	adm_mensaje->suscriptores_confirmados = list_create();

	dictionary_put(administracion_por_id, "1", adm_mensaje);

	list_add(administradores[adm_mensaje->codigo_operacion], adm_mensaje);

	orden_creacion++;
	primer_hijo_segundo_hijo->esta_libre = false;
	primer_hijo_segundo_hijo->orden_creacion = orden_creacion;
	primer_hijo_segundo_hijo->adm_mensaje = adm_mensaje;
	primer_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_segundo_hijo->size_mensaje = (particion_bs->tamanio_particion / 4) - 1;
	primer_hijo_segundo_hijo->padre = primer_hijo;
	primer_hijo_segundo_hijo->primer_hijo = NULL;
	primer_hijo_segundo_hijo->segundo_hijo = NULL;

	orden_creacion++;
	primer_hijo_primer_hijo->esta_libre = false;
	primer_hijo_primer_hijo->orden_creacion = orden_creacion;
	primer_hijo_primer_hijo->adm_mensaje = adm_mensaje;
	primer_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_primer_hijo->size_mensaje = (particion_bs->tamanio_particion / 4) - 1;
	primer_hijo_primer_hijo->padre = primer_hijo;
	primer_hijo_primer_hijo->primer_hijo = NULL;
	primer_hijo_primer_hijo->segundo_hijo = NULL;

	primer_hijo->esta_libre = true;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->primer_hijo = primer_hijo_primer_hijo;
	primer_hijo->segundo_hijo = primer_hijo_segundo_hijo;

	segundo_hijo->esta_libre = true;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = NULL;
	segundo_hijo->segundo_hijo = NULL;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	liberar_hoja_segun_algoritmo_reemplazo();

	CU_ASSERT_TRUE_FATAL(particion_bs->primer_hijo != NULL);
	CU_ASSERT_TRUE_FATAL(particion_bs->segundo_hijo != NULL);
	CU_ASSERT_TRUE_FATAL(particion_bs->primer_hijo->primer_hijo != NULL);
	CU_ASSERT_TRUE_FATAL(particion_bs->primer_hijo->segundo_hijo != NULL);

	CU_ASSERT_EQUAL_FATAL(primer_hijo_segundo_hijo->orden_creacion, 3);
	CU_ASSERT_TRUE_FATAL(primer_hijo_segundo_hijo->esta_libre);
	CU_ASSERT_EQUAL_FATAL(primer_hijo_segundo_hijo->size_mensaje, 0);
	CU_ASSERT_TRUE_FATAL(primer_hijo_segundo_hijo->adm_mensaje == NULL);

	finalizar_test_bs();
}

void test_liberar_hoja_con_compactacion_segun_algoritmo_reemplazo(){
	inicializar_test_bs();

	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_adm_mensaje* adm_mensaje1 = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* primer_hijo_segundo_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));

	adm_mensaje->id_mensaje = 1;
	adm_mensaje->codigo_operacion = NEW_POKEMON;
	adm_mensaje->suscriptores_enviados = list_create();
	adm_mensaje->suscriptores_confirmados = list_create();

	adm_mensaje1->id_mensaje = 2;
	adm_mensaje1->codigo_operacion = NEW_POKEMON;
	adm_mensaje1->suscriptores_enviados = list_create();
	adm_mensaje1->suscriptores_confirmados = list_create();

	dictionary_put(administracion_por_id, "1", adm_mensaje);
	dictionary_put(administracion_por_id, "2", adm_mensaje1);

	list_add(administradores[adm_mensaje->codigo_operacion], adm_mensaje);
	list_add(administradores[adm_mensaje1->codigo_operacion], adm_mensaje1);

	orden_creacion++;
	primer_hijo_segundo_hijo->esta_libre = true;
	primer_hijo_segundo_hijo->orden_creacion = orden_creacion;
	primer_hijo_segundo_hijo->adm_mensaje = adm_mensaje;
	primer_hijo_segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_segundo_hijo->padre = primer_hijo;
	primer_hijo_segundo_hijo->primer_hijo = NULL;
	primer_hijo_segundo_hijo->segundo_hijo = NULL;

	orden_creacion++;
	primer_hijo_primer_hijo->esta_libre = false;
	primer_hijo_primer_hijo->orden_creacion = orden_creacion;
	primer_hijo_primer_hijo->adm_mensaje = adm_mensaje1;
	primer_hijo_primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 4;
	primer_hijo_primer_hijo->size_mensaje = (particion_bs->tamanio_particion / 4) - 1;
	primer_hijo_primer_hijo->padre = primer_hijo;
	primer_hijo_primer_hijo->primer_hijo = NULL;
	primer_hijo_primer_hijo->segundo_hijo = NULL;

	primer_hijo->esta_libre = true;
	primer_hijo->padre = particion_bs;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->primer_hijo = primer_hijo_primer_hijo;
	primer_hijo->segundo_hijo = primer_hijo_segundo_hijo;

	segundo_hijo->esta_libre = true;
	segundo_hijo->padre = particion_bs;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = NULL;
	segundo_hijo->segundo_hijo = NULL;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	liberar_hoja_segun_algoritmo_reemplazo();

	CU_ASSERT_TRUE_FATAL(particion_bs->primer_hijo == NULL);
	CU_ASSERT_TRUE_FATAL(particion_bs->segundo_hijo == NULL);

	finalizar_memoria_cache();
	destruir_broker_config(broker_config);
}

void test_agregar_mensaje_a_memoria_cache_bs() {
	inicializar_test_bs();

	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* particion_elegida = NULL;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->payload = "Mensaje para guardar en la memoria cache segun algoritmo BS";
	mensaje->payload_size = strlen(mensaje->payload) + 1;

	particion_elegida = agregar_mensaje_memoria_cache_bs(mensaje, adm_mensaje);

	CU_ASSERT_NOT_EQUAL_FATAL(particion_elegida, NULL);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->esta_libre, false);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->offset, 0);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->adm_mensaje, adm_mensaje);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->size_mensaje, mensaje->payload_size);
	CU_ASSERT_TRUE_FATAL(particion_elegida->tamanio_particion >= mensaje->payload_size);

	free(mensaje);
	free(adm_mensaje);
	finalizar_test_bs();
}

void test_agregar_mensaje_con_reemplazo_consolidacion_a_memoria_cache_bs() {
	inicializar_test_bs();

	particion_bs->tamanio_particion = 64;

	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* particion_elegida = NULL;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->payload = "Mensaje para guardar en la memoria cache segun algoritmo BS";
	mensaje->payload_size = strlen(mensaje->payload) + 1;

	t_adm_mensaje* adm_mensaje1 = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* primer_hijo = malloc(sizeof(t_particion_bs));
	t_particion_bs* segundo_hijo = malloc(sizeof(t_particion_bs));

	adm_mensaje1->id_mensaje = 1;
	adm_mensaje1->codigo_operacion = NEW_POKEMON;
	adm_mensaje1->suscriptores_enviados = list_create();
	adm_mensaje1->suscriptores_confirmados = list_create();

	dictionary_put(administracion_por_id, "1", adm_mensaje1);

	list_add(administradores[adm_mensaje1->codigo_operacion], adm_mensaje1);

	primer_hijo->esta_libre = false;
	primer_hijo->padre = particion_bs;
	primer_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	primer_hijo->adm_mensaje = adm_mensaje1;
	primer_hijo->primer_hijo = NULL;
	primer_hijo->segundo_hijo = NULL;

	segundo_hijo->esta_libre = true;
	segundo_hijo->padre = particion_bs;
	segundo_hijo->tamanio_particion = particion_bs->tamanio_particion / 2;
	segundo_hijo->primer_hijo = NULL;
	segundo_hijo->segundo_hijo = NULL;

	particion_bs->primer_hijo = primer_hijo;
	particion_bs->segundo_hijo = segundo_hijo;

	particion_elegida = agregar_mensaje_memoria_cache_bs(mensaje, adm_mensaje);

	CU_ASSERT_NOT_EQUAL_FATAL(particion_elegida, NULL);
	CU_ASSERT_EQUAL_FATAL(particion_elegida, particion_bs);
	CU_ASSERT_TRUE_FATAL(particion_elegida->primer_hijo == NULL);
	CU_ASSERT_TRUE_FATAL(particion_elegida->segundo_hijo == NULL);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->esta_libre, false);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->offset, 0);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->adm_mensaje, adm_mensaje);
	CU_ASSERT_EQUAL_FATAL(particion_elegida->size_mensaje, mensaje->payload_size);
	CU_ASSERT_TRUE_FATAL(particion_elegida->tamanio_particion >= mensaje->payload_size);

	free(mensaje);
	free(adm_mensaje);
	finalizar_test_bs();
}

void test_leer_mensaje_de_memoria_cache_bs() {
	inicializar_test_bs();

	t_adm_mensaje* adm_mensaje = malloc(sizeof(t_adm_mensaje));
	t_particion_bs* particion_elegida = NULL;
	void* mensaje_leido = NULL;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->payload = "Mensaje para guardar en la memoria cache segun algoritmo BS";
	mensaje->payload_size = strlen(mensaje->payload) + 1;

	particion_elegida = agregar_mensaje_memoria_cache_bs(mensaje, adm_mensaje);

	mensaje_leido = leer_particion_bs(particion_elegida);

	CU_ASSERT_NOT_EQUAL_FATAL(mensaje_leido, NULL);
	CU_ASSERT_STRING_EQUAL_FATAL(mensaje_leido, mensaje->payload);
	CU_ASSERT_EQUAL_FATAL(strlen(mensaje_leido)+ 1, mensaje->payload_size);

	free(mensaje_leido);
	free(mensaje);
	free(adm_mensaje);
	finalizar_test_bs();
}
