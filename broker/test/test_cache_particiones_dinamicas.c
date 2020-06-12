#include "test_cache_particiones_dinamicas.h"
#define TAMANIO_MEMORIA 100

void agregar_tests_particiones_dinamicas(){
	CU_pSuite suite_configuracion = CU_add_suite("Memoria Caché : Particiones Dinamicas", NULL, NULL);

	CU_add_test(suite_configuracion, "Caché inicia con partición libre", inicaliza_cache_con_particion_libre);
	CU_add_test(suite_configuracion, "Guardar payload en Partición", test_guardar_un_payload);
	CU_add_test(suite_configuracion, "Leer payload desde Partición", test_leer_payload_desde_particion);
}

void inicalizar_test(){
	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = TAMANIO_MEMORIA;
	broker_config->algoritmo_memoria = strdup("PARTICIONES");

	inicializar_memoria_cache();
}

void inicaliza_cache_con_particion_libre(){
	inicalizar_test();
	t_particion_dinamica* particion_inical = list_first(particiones_dinamicas);

	CU_ASSERT_EQUAL(list_size(particiones_dinamicas), 1);
	assert_particion_esta_libre(particion_inical);
	assert_particion_tiene_el_tamanio(particion_inical, TAMANIO_MEMORIA);

	finalizar_memoria_cache();
}

void test_guardar_un_payload(){
	inicalizar_test();

	char const *a_guardar = "someString";
	int tamanio = strlen(a_guardar) + 1;

	t_particion_dinamica* particion_de_guardado = guardar_payload_en_particion_dinamica(a_guardar, tamanio);

	assert_particion_esta_ocupada(particion_de_guardado);
	assert_particion_tiene_el_tamanio(particion_de_guardado, tamanio);
	assert_particion_tiene_offset(particion_de_guardado, 0);

	free(particion_de_guardado);
	finalizar_memoria_cache();
}

void test_leer_payload_desde_particion(){
	inicalizar_test();

	char const *a_guardar = "someString";
	int tamanio = strlen(a_guardar) + 1;

	t_particion_dinamica* particion_de_guardado = guardar_payload_en_particion_dinamica(a_guardar, tamanio);
	char const *payload_leido = leer_particion_dinamica(particion_de_guardado);

	CU_ASSERT_STRING_EQUAL(a_guardar, payload_leido);

	free(particion_de_guardado);
	finalizar_memoria_cache();
}

void assert_particion_esta_libre(t_particion_dinamica* particion) {
	CU_ASSERT_TRUE(particion->esta_libre);
}

void assert_particion_esta_ocupada(t_particion_dinamica* particion) {
	CU_ASSERT_FALSE(particion->esta_libre);
}

void assert_particion_tiene_el_tamanio(t_particion_dinamica* particion, int tamanio) {
	CU_ASSERT_EQUAL(particion->tamanio_particion, tamanio);
}

void assert_particion_tiene_offset(t_particion_dinamica* particion, int offset) {
	CU_ASSERT_EQUAL(particion->offset, offset);
}
