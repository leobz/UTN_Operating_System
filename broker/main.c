#include "main.h"


int main(int argc, char ** argv) {
	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();
	}
	else {
		inicializar_broker();
		inicializar_memoria_cache();

		char*ip=broker_config->ip_broker;
		char*puerto=broker_config->puerto_broker;
		int socket_servidor = iniciar_servidor(ip, puerto);

for(int i = 0; i < 6; i++)
		     sem_init(&cola_vacia[i], 0, 0);

	for(int j = 0; j < 6; j++)
		 sem_init(&sem_proceso[j], 0, 0);

	administracion_por_id=dictionary_create();
	administracion_por_cod=dictionary_create();
	subscribers= dictionary_create();

	for(int j = 0; j < 6; j++)
		inicializar_listas(j);


		pthread_create(&sem_mensajes[NEW_POKEMON],NULL,(void*)enviar_mensajes_en_cola,NEW_POKEMON);
		pthread_create(&sem_mensajes[GET_POKEMON],NULL,(void*)enviar_mensajes_en_cola,GET_POKEMON);
		pthread_create(&sem_mensajes[CATCH_POKEMON],NULL,(void*)enviar_mensajes_en_cola,CATCH_POKEMON);
		pthread_create(&sem_mensajes[APPEARED_POKEMON],NULL,(void*)enviar_mensajes_en_cola,APPEARED_POKEMON);
		pthread_create(&sem_mensajes[LOCALIZED_POKEMON],NULL,(void*)enviar_mensajes_en_cola,LOCALIZED_POKEMON);
		pthread_create(&sem_mensajes[CAUGHT_POKEMON],NULL,(void*)enviar_mensajes_en_cola,CAUGHT_POKEMON);

		while (1) {
			esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
		}

		pthread_detach(sem_mensajes[NEW_POKEMON]);
		pthread_detach(sem_mensajes[GET_POKEMON]);
		pthread_detach(sem_mensajes[CATCH_POKEMON]);
		pthread_detach(sem_mensajes[APPEARED_POKEMON]);
		pthread_detach(sem_mensajes[LOCALIZED_POKEMON]);
		pthread_detach(sem_mensajes[CAUGHT_POKEMON]);

		finalizar_memoria_cache();
		finalizar_broker(broker_config,logger);

		return 0;
	}

}


void loggear_mensaje_enviado(int socket, int codigo_de_operacion) {
	log_info(logger, "Mensaje enviado %s a socket: %d", op_code_to_string(codigo_de_operacion), socket);
}

void loggear_mensaje_recibido(int codigo_de_operacion, void* sent_package) {
	// Esta funcion no es de tanta prioridad, terminar solo despues de hacer la adminstracion de mensajes completa
	switch (codigo_de_operacion) {
	case CATCH_POKEMON:
		mensaje_catch = deserializar_paquete_catch_pokemon(sent_package);
		log_info(logger, "Mensaje recibido CATCH_POKEMON %s %d %d",
				mensaje_catch->pokemon, mensaje_catch->posx,
				mensaje_catch->posy);

		free(mensaje_catch->pokemon);
		free(mensaje_catch);
		break;
	default:
		//printf("ERROR, CODIGO DE OPERACION INCORRECTO\n");
		//exit(-1);
		break;
	}
}

void enviar_mensajes_en_cola(int codigo_de_operacion){
	while(1){
		sem_wait(&cola_vacia[codigo_de_operacion]); //existen mensajes en la cola

		pthread_mutex_lock(&mutex[codigo_de_operacion]);
		mensaje[codigo_de_operacion] = extraer_mensaje(codigo_de_operacion);
		pthread_mutex_unlock(&mutex[codigo_de_operacion]);

		int bytes=0;
		void *sent_package = empaquetar_mensaje_broker(mensaje[codigo_de_operacion],&bytes);

		loggear_mensaje_recibido(codigo_de_operacion, sent_package); //Por ahora de prueba

		if(list_size(suscriptores[codigo_de_operacion])==0)
			sem_wait(&sem_proceso[codigo_de_operacion]);

		t_adm_mensaje* administrator;
		administrator=iniciar_administracion(mensaje[codigo_de_operacion]);

////////////////////////////////////////////////////////////////////////////////////////////
		void enviar_a_suscriptores(t_proceso* proceso){

			int validez=enviar_mensaje_con_retorno(proceso->socket,sent_package,bytes);
			//log_info(logger,"Validez: %d",validez);

			if(validez!=1){ //si se pudo enviar se agrega el proceso a la lista de suscriptores_enviados
				list_add(administrator->suscriptores_enviados,proceso);
				loggear_mensaje_enviado(proceso->socket, codigo_de_operacion); //Por ahora de prueba
			}
		}


		list_iterate(suscriptores[codigo_de_operacion],&enviar_a_suscriptores);

		//agregar_mensaje_memoria_cache(mensaje[codigo_de_operacion]);

		free(sent_package);
		free(mensaje[codigo_de_operacion]->payload);
		free(mensaje[codigo_de_operacion]);

	}
}

t_adm_mensaje*iniciar_administracion(t_mensaje*mensaje){

	t_adm_mensaje *administrador=malloc(sizeof(administrador));
		administrador->id_mensaje= mensaje->id_mensaje;
		administrador->id_correlativo=mensaje->id_correlativo;
		administrador->codigo_operacion=mensaje->codigo_operacion;
		administrador->suscriptores_confirmados=list_create();
		administrador->suscriptores_enviados=list_create();

		meter_en_diccionario(administracion_por_id,mensaje->id_mensaje,administrador);

		list_add(administradores[mensaje->codigo_operacion],administrador);

		return administrador;
}

