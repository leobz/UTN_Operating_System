#include "test_cache_particiones_dinamicas.h"
#define TAMANIO_MEMORIA 100

void agregar_tests_particiones_dinamicas(){
	CU_pSuite suite_configuracion = CU_add_suite("Memoria Caché : Particiones Dinamicas", NULL, NULL);

	CU_add_test(suite_configuracion, "Caché inicia con partición libre", test_inicializar_cache_crea_particion_libre);
	CU_add_test(suite_configuracion, "Guardar payload en Partición", test_guardar_un_payload);
	CU_add_test(suite_configuracion, "Leer payload desde Partición", test_leer_payload_desde_particion);
	CU_add_test(suite_configuracion, "Guardar varios payloads no afecta a particiones anteriores",
			test_guardar_varias_particiones_no_afecta_particiones_previas);
	CU_add_test(suite_configuracion, "Guardar Particion intermedia funciona correctamente",
			test_guardar_crea_particion_intermedia);

}

void inicializar_test_con_particion_dinamica(){
	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = TAMANIO_MEMORIA;
	free(broker_config->algoritmo_memoria);
	broker_config->algoritmo_memoria = strdup("PARTICIONES");

	inicializar_memoria_cache();
}

void finalizar_test() {
	finalizar_memoria_cache();
	destruir_broker_config(broker_config);
}

void test_inicializar_cache_crea_particion_libre(){
	inicializar_test_con_particion_dinamica();
	t_particion_dinamica* particion_inicial = list_first(particiones_dinamicas);

	CU_ASSERT_EQUAL(list_size(particiones_dinamicas), 1);
	assert_particion_esta_libre(particion_inicial);
	assert_particion_tiene_el_tamanio(particion_inicial, TAMANIO_MEMORIA);

	finalizar_test();
}

void test_guardar_un_payload(){
	inicializar_test_con_particion_dinamica();

	char const *a_guardar = "someString";
	int tamanio = strlen(a_guardar) + 1;

	t_particion_dinamica* particion_de_guardado = guardar_payload_en_particion_dinamica(a_guardar, tamanio);

	assert_particion_esta_ocupada(particion_de_guardado);
	assert_particion_tiene_el_tamanio(particion_de_guardado, tamanio);
	assert_particion_tiene_offset(particion_de_guardado, 0);

	finalizar_test();
}

void test_leer_payload_desde_particion(){
	inicializar_test_con_particion_dinamica();

	char const *a_guardar = "someString";
	int tamanio = strlen(a_guardar) + 1;

	t_particion_dinamica* particion_de_guardado = guardar_payload_en_particion_dinamica(a_guardar, tamanio);

	char const *payload_leido = leer_particion_dinamica(particion_de_guardado);

	CU_ASSERT_STRING_EQUAL(a_guardar, payload_leido);

	free(payload_leido);
	finalizar_test();
}

void test_guardar_varias_particiones_no_afecta_particiones_previas(){
	inicializar_test_con_particion_dinamica();

	char const *a_guardar_a = "first";
	char const *a_guardar_b = "second";
	char const *a_guardar_c = "third";

	t_particion_dinamica* particion_escrita_a = guardar_string_en_particion(a_guardar_a);
	t_particion_dinamica* particion_escrita_b = guardar_string_en_particion(a_guardar_b);
	t_particion_dinamica* particion_escrita_c = guardar_string_en_particion(a_guardar_c);

	CU_ASSERT_STRING_EQUAL(a_guardar_a, leer_particion_dinamica(particion_escrita_a));
	CU_ASSERT_STRING_EQUAL(a_guardar_b, leer_particion_dinamica(particion_escrita_b));
	CU_ASSERT_STRING_EQUAL(a_guardar_c, leer_particion_dinamica(particion_escrita_c));

	finalizar_test();
}

void test_guardar_crea_particion_intermedia(){
	inicializar_test_con_particion_dinamica();

	t_particion_dinamica* particion;
	t_particion_dinamica* particion_intermedia;

	particion = crear_particion_dinamica(0, 10);

	list_clean(particiones_dinamicas);
	crear_particion_intermedia(particion);
	particion_intermedia = list_first(particiones_dinamicas);

	assert_particion_esta_libre(particion_intermedia);
	assert_particion_tiene_el_tamanio(particion_intermedia, TAMANIO_MEMORIA - particion->tamanio_particion);
	assert_particion_tiene_offset(
			particion_intermedia,
			particion->offset + particion->tamanio_particion
			);

	free(particion);
	finalizar_test();
}

t_particion_dinamica* guardar_string_en_particion(const char* a_guardar) {
	int tamanio = strlen(a_guardar) + 1;
	return  guardar_payload_en_particion_dinamica(a_guardar, tamanio);
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
