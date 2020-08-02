/*
 * test_algoritmos_dinamicas.c
 *
 *  Created on: Jul 8, 2020
 *      Author: diego
 */

#include "test_algoritmos_dinamicas.h"


void agregar_tests_algoritmos(){
	CU_pSuite suite_configuracion = CU_add_suite("Memoria Caché : Algoritmos Particiones Dinamicas", NULL, NULL);

	CU_add_test(suite_configuracion, "Eliminacion y consolidacion", test_eliminacion_y_consolidacion_dinamicas);
	CU_add_test(suite_configuracion, "Best Fit y Lru", test_bestfit_y_lru_dinamicas);
	CU_add_test(suite_configuracion, "First Fit y FIFO", test_firstfit_y_fifo_dinamicas);
	CU_add_test(suite_configuracion, "Compactacion Dinamicas", test_compactacion_dinamicas);

}

void inicializar_test_algoritmos_bf(){

	logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);
	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = TAMANO_MEMORIA;
	free(broker_config->algoritmo_memoria);
	broker_config->algoritmo_memoria = strdup("PARTICIONES");
	free(broker_config->algoritmo_particion_libre);
	broker_config->algoritmo_particion_libre = strdup("BF");
	free(broker_config->algoritmo_reemplazo);
	broker_config->algoritmo_reemplazo = strdup("LRU");
	broker_config->frecuencia_compactacion=3;
	broker_config->tamanio_minimo_particion=4;

	inicializar_memoria_cache();
}

void inicializar_test_algoritmos_ff(){

	logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);

	broker_config = cargar_broker_config("broker.config.sample");
	broker_config->tamanio_memoria = TAMANO_MEMORIA;
	free(broker_config->algoritmo_memoria);
	broker_config->algoritmo_memoria = strdup("PARTICIONES");
	free(broker_config->algoritmo_particion_libre);
	broker_config->algoritmo_particion_libre = strdup("FF");
	free(broker_config->algoritmo_reemplazo);
	broker_config->algoritmo_reemplazo = strdup("FIFO");
	broker_config->frecuencia_compactacion=3;
	broker_config->tamanio_minimo_particion=4;

	inicializar_memoria_cache();
}

void test_eliminacion_y_consolidacion_dinamicas(){

		inicializar_test_algoritmos_bf();
		iniciar_listas_y_diccionarios();

		iniciar_envio_de_mensajes();


		char *picachuputo = "PicachuPuto";
		int picachuputo_length = strlen(picachuputo) + 1;

		t_adm_mensaje*adm_picachuputo;
		adm_picachuputo=iniciar_adm(adm_picachuputo,NEW_POKEMON);

		t_particion_dinamica* particion_picachuputo=guardar_payload_en_particion_dinamica_con_adm(picachuputo,picachuputo_length,adm_picachuputo);


		t_list*libres=obtener_particiones_dinamicas_libres();
		list_sort(libres,(void*)pd_es_menor_offset);

		t_particion_dinamica*particion_first=list_first(libres);
		t_particion_dinamica*particion_last=list_last(libres);

		assert_particion_tiene_offset(particion_first, 12);
		assert_particion_tiene_offset(particion_last, 45);
		assert_particion_tiene_offset(particion_picachuputo, 0);
		assert_particiones_libres(libres,2);

		free(libres);

		liberar_listas_y_diccionarios();
		finalizar_test_algoritmos();
}

void test_bestfit_y_lru_dinamicas(){
	inicializar_test_algoritmos_bf();
	iniciar_listas_y_diccionarios();
	iniciar_envio_de_mensajes();


	char *picachupu = "PicachuPu";
	int picachupu_length = strlen(picachupu) + 1;

	char *dani = "Dani";
	int dani_length = strlen(dani) + 1;

	char *davide = "Davide";
	int davide_length = strlen(davide) + 1;

	char *leo = "Leo";
	int leo_length = strlen(leo) + 1;


	t_adm_mensaje*adm_picachupu;
	adm_picachupu=iniciar_adm(adm_picachupu,NEW_POKEMON);

	t_adm_mensaje*adm_dani;
	adm_dani=iniciar_adm(adm_dani,NEW_POKEMON);

	t_adm_mensaje*adm_davide;
	adm_davide=iniciar_adm(adm_davide,NEW_POKEMON);

	t_adm_mensaje*adm_leo;
	adm_leo=iniciar_adm(adm_leo,NEW_POKEMON);


	t_particion_dinamica* particion_picachupu=guardar_payload_en_particion_dinamica_con_adm(picachupu,picachupu_length,adm_picachupu);
	t_particion_dinamica* particion_dani=guardar_payload_en_particion_dinamica_con_adm(dani,dani_length,adm_dani);
	t_particion_dinamica* particion_davide=guardar_payload_en_particion_dinamica_con_adm(davide,davide_length,adm_davide);

	adm_picachupu->particion_dinamica=particion_picachupu;
	adm_dani->particion_dinamica=particion_dani;
	adm_davide->particion_dinamica=particion_davide;

	leer_particiones_de_cola(APPEARED_POKEMON);

	t_particion_dinamica* particion_leo=guardar_payload_en_particion_dinamica_con_adm(leo,leo_length,adm_leo);
	adm_leo->particion_dinamica=particion_leo;


	t_list*libres=obtener_particiones_dinamicas_libres();

	t_particion_dinamica*particion_first=list_first(libres);

	assert_particion_tiene_offset(particion_first, 32);
	assert_particion_tiene_offset(particion_dani, 45);
	assert_particion_tiene_offset(particion_leo, 28);
	assert_particiones_libres(libres,1);

	free(libres);

	liberar_listas_y_diccionarios();
	finalizar_test_algoritmos();

}


void test_firstfit_y_fifo_dinamicas(){
	inicializar_test_algoritmos_ff();
	iniciar_listas_y_diccionarios();
	iniciar_envio_de_mensajes();


	char *picachupu = "PicachuPu";//10
	int picachupu_length = strlen(picachupu) + 1;

	char *dani = "Dani";//5
	int dani_length = strlen(dani) + 1;

	char *davide = "Davide";//7
	int davide_length = strlen(davide) + 1;

	char *leo = "Leote";//6
	int leo_length = strlen(leo) + 1;


	t_adm_mensaje*adm_picachupu;
	adm_picachupu=iniciar_adm(adm_picachupu,NEW_POKEMON);

	t_adm_mensaje*adm_dani;
	adm_dani=iniciar_adm(adm_dani,NEW_POKEMON);

	t_adm_mensaje*adm_davide;
	adm_davide=iniciar_adm(adm_davide,NEW_POKEMON);

	t_adm_mensaje*adm_leo;
	adm_leo=iniciar_adm(adm_leo,NEW_POKEMON);


	t_particion_dinamica* particion_picachupu=guardar_payload_en_particion_dinamica_con_adm(picachupu,picachupu_length,adm_picachupu);
	t_particion_dinamica* particion_dani=guardar_payload_en_particion_dinamica_con_adm(dani,dani_length,adm_dani);
	t_particion_dinamica* particion_davide=guardar_payload_en_particion_dinamica_con_adm(davide,davide_length,adm_davide);

	adm_picachupu->particion_dinamica=particion_picachupu;
	adm_dani->particion_dinamica=particion_dani;
	adm_davide->particion_dinamica=particion_davide;

	leer_particiones_de_cola(NEW_POKEMON);

	t_particion_dinamica* particion_leo=guardar_payload_en_particion_dinamica_con_adm(leo,leo_length,adm_leo);
	adm_leo->particion_dinamica=particion_leo;


	t_list*libres=obtener_particiones_dinamicas_libres();

	t_particion_dinamica*particion_first=list_first(libres);

	//assert_particion_tiene_offset(particion_first, 32);
	assert_particion_tiene_offset(particion_dani, 10);
	assert_particion_tiene_offset(particion_leo, 22);
	assert_particiones_libres(libres,1);

	free(libres);

	liberar_listas_y_diccionarios();
	finalizar_test_algoritmos();

}


void test_compactacion_dinamicas(){
	inicializar_test_algoritmos_bf();
	iniciar_listas_y_diccionarios();
	iniciar_envio_de_mensajes();


	char *picachuputo = "PicachuPutos";
	int picachuputo_length = strlen(picachuputo) + 1;




	leer_particiones_de_cola(NEW_POKEMON);

	t_adm_mensaje*adm_picachuputo;
	adm_picachuputo=iniciar_adm(adm_picachuputo,NEW_POKEMON);


	t_particion_dinamica* particion_picachuputo=guardar_payload_en_particion_dinamica_con_adm(picachuputo,picachuputo_length,adm_picachuputo);
	adm_picachuputo->particion_dinamica=particion_picachuputo;


	t_list*libres=obtener_particiones_dinamicas_libres();
	t_particion_dinamica*particion_first=list_first(libres);

	assert_particion_tiene_offset(particion_first, 32);
	assert_particion_tiene_offset(particion_picachuputo, 19);
	assert_particiones_libres(libres,1);

	free(libres);

	liberar_listas_y_diccionarios();
	finalizar_test_algoritmos();

}

void iniciar_listas_y_diccionarios(){
	administracion_por_id = dictionary_create();
	administracion_por_cod = dictionary_create();

	for (int j = 0; j < 6; j++)
		iniciar_listas_diccionarios_colas(j);

}

void iniciar_listas_diccionarios_colas(int cola){
		administradores[cola]=list_create();
		meter_en_diccionario(administracion_por_cod,cola,administradores[cola]);
}

t_adm_mensaje* iniciar_adm(t_adm_mensaje *adm,op_code cola){

	adm=malloc(sizeof(t_adm_mensaje));
	adm->suscriptores_confirmados=list_create();
	adm->suscriptores_enviados=list_create();
	adm->id_correlativo=++contador_id_correlativo;
	adm->id_mensaje=++contador_id_mensaje;
	adm->codigo_operacion=cola;
	meter_en_diccionario(administracion_por_id,adm->id_mensaje,adm);
	list_add(administradores[cola],adm);

	return adm;
}

void iniciar_envio_de_mensajes(){


	char *picachu = "Picachu"; //8
	int picachu_length = strlen(picachu) + 1;

	char *charizar = "Charizar";//9
	int charizar_length = strlen(charizar) + 1;

	char *charmander = "Charmander";//11
	int charmander_length = strlen(charmander) + 1;

	char *diegojose = "DiegoJose";//10
	int diegojose_length = strlen(diegojose) + 1;

	char *marico = "Marico";//7
	int marico_length = strlen(marico) + 1;

	//TOTAL:45

	t_adm_mensaje*adm_picachu;
	adm_picachu=iniciar_adm(adm_picachu,GET_POKEMON);

	t_adm_mensaje*adm_charizar;
	adm_charizar=iniciar_adm(adm_charizar,NEW_POKEMON);

	t_adm_mensaje*adm_charmander;
	adm_charmander=iniciar_adm(adm_charmander,APPEARED_POKEMON);

	t_adm_mensaje*adm_diegojose;
	adm_diegojose=iniciar_adm(adm_diegojose,NEW_POKEMON);

	t_adm_mensaje*adm_marico;
	adm_marico=iniciar_adm(adm_marico,APPEARED_POKEMON);

	t_particion_dinamica* particion_picachu=guardar_payload_en_particion_dinamica_con_adm(picachu, picachu_length,adm_picachu);
	t_particion_dinamica* particion_charizar=guardar_payload_en_particion_dinamica_con_adm(charizar, charizar_length,adm_charizar);
	t_particion_dinamica* particion_charmander=guardar_payload_en_particion_dinamica_con_adm(charmander,charmander_length,adm_charmander);
	t_particion_dinamica* particion_diegojose=guardar_payload_en_particion_dinamica_con_adm(diegojose,diegojose_length,adm_diegojose);
	t_particion_dinamica* particion_marico=guardar_payload_en_particion_dinamica_con_adm(marico,marico_length,adm_marico);


	adm_picachu->particion_dinamica=particion_picachu;
	adm_charizar->particion_dinamica=particion_charizar;
	adm_charmander->particion_dinamica=particion_charmander;
	adm_diegojose->particion_dinamica=particion_diegojose;
	adm_marico->particion_dinamica=particion_marico;

}

void liberar_listas_y_diccionarios(){


	for (int j = 0; j < 6; j++){
		if(list_size(administradores[j])!=0){

			void eliminar_adm(t_adm_mensaje*adm){

				free(adm->suscriptores_enviados);
				free(adm->suscriptores_confirmados);
				free(adm);
			}

			list_destroy_and_destroy_elements(administradores[j], (void*)eliminar_adm);
		}
		else
			free(administradores[j]);
	}

	free(administracion_por_id);
	free(administracion_por_cod);
}

void finalizar_test_algoritmos() {
	finalizar_memoria_cache();
	destruir_broker_config(broker_config);
	destruir_logger(logger);
}


