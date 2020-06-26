/*
 * test_eliminacion_particiones_dinamicas.c
 *
 *  Created on: Jun 25, 2020
 *      Author: diego
 */
#include "test_eliminacion_particiones_dinamicas.h"

void inicializar_test_con_particiones(){
	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = 100;
	free(broker_config->algoritmo_memoria);
	broker_config->algoritmo_memoria = strdup("PARTICIONES");
	broker_config->tamanio_minimo_particion=6;
	broker_config->algoritmo_reemplazo=strdup("FIFO");

	inicializar_memoria_cache();
}

void finalizar_test_eliminacion() {
	finalizar_memoria_cache();
	destruir_broker_config(broker_config);
}
/*
void test_eliminar_particion(){
	char *pokemon = strdup("")
	int pos_x = atoi(argv[4]);
	int pos_y = atoi(argv[5]);
	int cantidad = atoi(argv[6]);
	int id_mensaje=0;
	int id_correlativo = 0;

	int caught=3;
	void*payload=malloc(sizeof(caught));
	payload=&caught;
}
*/
