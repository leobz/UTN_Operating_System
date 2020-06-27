#include "main.h"
#include "test/testing.h"

void inicializar_diccionarios() {
	administracion_por_id = dictionary_create();
	administracion_por_cod = dictionary_create();
	subscribers = dictionary_create();
}

void inicalizar_lista_de_todos_las_colas() {
	for (int i = 0; i < 6; i++)
		sem_init(&cola_vacia[i], 0, 0);
	for (int j = 0; j < 6; j++)
		sem_init(&sem_proceso[j], 0, 0);
	for (int j = 0; j < 6; j++)
		inicializar_listas(j);
}

int main(int argc, char ** argv) {

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();
	}
	else {
		inicializar_broker();
		inicializar_memoria_cache();
		inicializar_diccionarios();
		inicalizar_lista_de_todos_las_colas();

		char*ip=broker_config->ip_broker;
		char*puerto=broker_config->puerto_broker;
		int socket_servidor = iniciar_servidor(ip, puerto);



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


	case NEW_POKEMON:
		mensaje_new_recibido = deserializar_paquete_new_pokemon(sent_package);

			log_info(logger,"Mensaje recibido NEW_POKEMON %s %d %d %d",mensaje_new_recibido->pokemon,
					mensaje_new_recibido->posx,mensaje_new_recibido->posy,mensaje_new_recibido->cantidad);

			free(mensaje_new_recibido->pokemon);
			free(mensaje_new_recibido);

		break;

	case GET_POKEMON:
		mensaje_get_recibido= deserializar_paquete_get_pokemon(sent_package);

			log_info(logger,"Mensaje recibido GET_POKEMON %s",mensaje_get_recibido->pokemon);

			free(mensaje_get_recibido->pokemon);
			free(mensaje_get_recibido);


				break;

	case CATCH_POKEMON:
		mensaje_catch_recibido = deserializar_paquete_catch_pokemon(sent_package);
		log_info(logger, "Mensaje recibido CATCH_POKEMON %s %d %d",
				mensaje_catch_recibido->pokemon, mensaje_catch_recibido->posx,
				mensaje_catch_recibido->posy);

		free(mensaje_catch_recibido->pokemon);
		free(mensaje_catch_recibido);
		break;

	case APPEARED_POKEMON:

		mensaje_appeared_recibido= deserializar_paquete_appeared_pokemon(sent_package);

		log_info(logger,"Mensaje recibido APPEARED_POKEMON %s %d %d",mensaje_appeared_recibido->pokemon,
				mensaje_appeared_recibido->posx,mensaje_appeared_recibido->posy);

		free(mensaje_appeared_recibido->pokemon);
		free(mensaje_appeared_recibido);


				break;

	case CAUGHT_POKEMON:
		mensaje_caught_recibido= deserializar_paquete_caught_pokemon(sent_package);

		log_info(logger,"Mensaje recibido CAUGHT_POKEMON %d %s",mensaje_caught_recibido->id_correlativo,
				value_to_state(mensaje_caught_recibido->resultado));

			free(mensaje_caught_recibido);

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

		t_adm_mensaje* administrator=iniciar_administracion(mensaje[codigo_de_operacion]);

		pthread_mutex_lock(&m_cache);
			agregar_mensaje_memoria_cache(administrator, mensaje[codigo_de_operacion]);
		pthread_mutex_unlock(&m_cache);

		if(list_size(suscriptores[codigo_de_operacion])==0)
					sem_wait(&sem_proceso[codigo_de_operacion]);



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


		free(sent_package);
		free(mensaje[codigo_de_operacion]->payload);
		free(mensaje[codigo_de_operacion]);

	}
}


