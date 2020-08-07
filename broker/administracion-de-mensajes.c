#include "administracion-de-mensajes.h"

t_adm_mensaje* iniciar_administracion(t_mensaje*mensaje){
	t_adm_mensaje *administrador = malloc(sizeof(t_adm_mensaje));

	administrador->id_mensaje= mensaje->id_mensaje;
	administrador->id_correlativo=mensaje->id_correlativo;
	administrador->codigo_operacion = mensaje->codigo_operacion;
	administrador->suscriptores_confirmados=list_create();
	administrador->suscriptores_enviados=list_create();

	meter_en_diccionario(administracion_por_id,mensaje->id_mensaje,administrador);

	list_add(administradores[mensaje->codigo_operacion],administrador);

	return administrador;
}

void* generar_mensaje(t_adm_mensaje* actual_administrator, int*bytes){

	void* payload; //para el mensaje en cache
	int payload_size = 0; //para el tamaño del mensaje en cache

	if (es_particion_dinamica()){ //aplicar funcion es_particion_dinamica
		payload = leer_particion_dinamica(actual_administrator->particion_dinamica);
		payload_size = actual_administrator->particion_dinamica->tamanio_particion;
	}

	if(es_buddy_system()){ //aplicar funcion es_buddy_system
		payload = leer_particion_bs(actual_administrator->particion_bs);
		payload_size = actual_administrator->particion_bs->size_mensaje;
	}

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->codigo_operacion = actual_administrator->codigo_operacion;
	mensaje->id_mensaje = actual_administrator->id_mensaje;
	mensaje->id_correlativo = actual_administrator->id_correlativo;
	mensaje->payload= serializar_segun_codigo_con_barra(payload,mensaje->codigo_operacion,&payload_size);
	mensaje->payload_size = payload_size;

	void*payd=empaquetar_mensaje_broker(mensaje, bytes);

	free(mensaje->payload);
	free(mensaje);
	return payd;
}

void agregar_mensaje_memoria_cache(t_adm_mensaje* actual_administrator, t_mensaje* mensaje) {
	if (es_particion_dinamica()){
		actual_administrator->particion_dinamica = agregar_mensaje_memoria_cache_particion_dinamica_barra_cero(mensaje,actual_administrator);
		log_info(logger,"Almacenando mensaje con ID %d en cache en posicion %d",mensaje->id_mensaje, actual_administrator->particion_dinamica->offset);
	}
	else if(es_buddy_system()){
		actual_administrator->particion_bs = agregar_mensaje_memoria_cache_bs_barra_cero(mensaje, actual_administrator);
		log_info(logger,"Almacenando mensaje con ID %d en cache en posicion %d",mensaje->id_mensaje, actual_administrator->particion_bs->offset);
	}
}





void eliminar_adm_mensaje_particion_en_diccionarios(t_adm_mensaje* admins_mensaje){

	int id_mensajee = admins_mensaje->id_mensaje;

	op_code cod_ope = admins_mensaje->codigo_operacion;

	char*id_men=pasar_a_char(id_mensajee);
	dictionary_remove(administracion_por_id, id_men);
	free(id_men);


	//Elimino adm_mensaje de la lista asociada al diccionario 'administracion_por_cod'

	char*cod=pasar_a_char(cod_ope);
	t_list* lista_adm_mensajes = dictionary_get(administracion_por_cod, cod);
	free(cod);

	bool tiene_mismo_id_mensaje(t_adm_mensaje* elem_adm_mensaje){
		return elem_adm_mensaje->id_mensaje == id_mensajee;
	}

	t_adm_mensaje* men=list_remove_by_condition(lista_adm_mensajes, (void*)tiene_mismo_id_mensaje);

	log_info(logger_debug,"Removiendo: Id: %d, Cod %d",men->id_mensaje, men->codigo_operacion);


	if(list_size(admins_mensaje->suscriptores_enviados)!=0)
		list_clean(admins_mensaje->suscriptores_enviados);
	if(list_size(admins_mensaje->suscriptores_confirmados)!=0)
		list_clean(admins_mensaje->suscriptores_confirmados);


	free(admins_mensaje->suscriptores_enviados);
	free(admins_mensaje->suscriptores_confirmados);
	free(admins_mensaje);
}


void leer_particion_dinamica_sin_payload(t_particion_dinamica* particion){

	if(es_lru()){
			particion->contador_uso=++contador_uso;
	}
}

void leer_particiones_de_cola(int num_cola){

	void leer_su_particion(t_adm_mensaje*adm){
		leer_particion_dinamica_sin_payload(adm->particion_dinamica);
	}

	list_iterate(administradores[num_cola],&leer_su_particion);
}
