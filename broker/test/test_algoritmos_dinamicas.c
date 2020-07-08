/*
 * test_algoritmos_dinamicas.c
 *
 *  Created on: Jul 8, 2020
 *      Author: diego
 */

#include "test_algoritmos_dinamicas.h"

void inicializar_test_algoritmos(){
	broker_configure = cargar_broker_config("broker.config.sample");
	broker_configure->tamanio_memoria = TAMANIO_MEMORIA;
	free(broker_configure->algoritmo_memoria);
	broker_configure->algoritmo_memoria = strdup("PARTICIONES");
	free(broker_configure->algoritmo_particion_libre);
	broker_configure->algoritmo_particion_libre = strdup("BF");
	free(broker_configure->algoritmo_reemplazo);
	broker_configure->algoritmo_reemplazo = strdup("LRU");
	broker_configure->frecuencia_compactacion=3;

	inicializar_memoria_cache();
}
/*
void test_eliminacion_dinamicas(){


			char *pokemon = strdup("Putin");
			int pos_x = 2;
			int pos_y = 3;
			int cantidad = 1;
			int id_mensaje=0;
			int id_correlativo = 0;

			int bytes;
			void *a_enviar = serializar_new_pokemon(&bytes, pokemon, pos_x,pos_y, cantidad,id_mensaje, id_correlativo);
}
*/


void finalizar_test_algoritmos() {
	finalizar_memoria_cache();
	destruir_broker_config(broker_configure);
}


