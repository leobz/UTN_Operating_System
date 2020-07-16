#include "CUnit/Basic.h"
#include "testing.h"


void imprimir_diccionario(t_dictionary* diccionario) {
	void imprimir_clave_valor(char* key, int value) {
		printf("	%s -> %d\n", key, value);
	}

	dictionary_iterator(diccionario, imprimir_clave_valor);
}

void imprimir_posicion(t_posicion* posicion){
	printf("	(%d, %d)\n", posicion->x, posicion->y);

}

void imprimir_posiciones(t_list* posiciones) {
	t_posicion* posicion = list_get(posiciones, 0);
	//TODO: sustiturir por ASSERTS
	imprimir_posicion(posicion);
}

void mostrar_lista_entrenadores(t_team_config* team_config) {
	for (int i = 0; i < team_config->cantidad_entrenadores; i++) {
		t_tcb_entrenador* entrenador = list_get(new, i);

		printf("\nEntrenador %d\n", i);
		printf("  Posicion:\n");
		imprimir_posicion(entrenador->posicion);

		printf("  Objetivos:\n");
		imprimir_diccionario(entrenador->objetivos);

		printf("  Capturados:\n");
		imprimir_diccionario(entrenador->pokemones_capturados);
	}
}

void correrTests() {
	CU_initialize_registry();
	agregar_tests_lista_ready();
	agregar_tests_rafagas();
	agregar_tests_entrenador_cercano_a_ready();
	agregar_tests_movimiento_entrenador();
	agregar_tests_lista_blocked();
	agregar_tests_appeared();
	agregar_tests_caught();
	agregar_tests_algoritmos_sjf();
	agregar_tests_deadlock();
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

}

