#include "main.h"

t_cola_proceso *proceso;

int main() {

	inicializar_broker(&broker_config,&logger);
	char*ip=broker_config->ip_broker;
	char*puerto=broker_config->puerto_broker;
	int socket_servidor = iniciar_servidor(ip, puerto);

	for(int i = 0; i < 6; i++)
	     sem_init(&cola_vacia[i], 0, 0);

	for(int j = 0; j < 6; j++)
		     sem_init(&sem_proceso[j], 0, 0);

	for(int j = 0; j < 6; j++)
		inicializar_lista(j);



	pthread_create(&sem_mensajes[NEW_POKEMON],NULL,(void*)extraer_new_pokemon,NULL);
	pthread_create(&sem_mensajes[GET_POKEMON],NULL,(void*)extraer_get_pokemon,NULL);
	pthread_create(&sem_mensajes[CATCH_POKEMON],NULL,(void*)enviar_mensajes_en_cola,CATCH_POKEMON);
	pthread_create(&sem_mensajes[APPEARED_POKEMON],NULL,(void*)extraer_appeared_pokemon,NULL);
	pthread_create(&sem_mensajes[LOCALIZED_POKEMON],NULL,(void*)extraer_localized_pokemon,NULL);
	pthread_create(&sem_mensajes[CAUGHT_POKEMON],NULL,(void*)extraer_caught_pokemon,NULL);



	while (1) {
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}

	pthread_detach(sem_mensajes[NEW_POKEMON]);
	pthread_detach(sem_mensajes[GET_POKEMON]);
	pthread_detach(sem_mensajes[CATCH_POKEMON]);
	pthread_detach(sem_mensajes[APPEARED_POKEMON]);
	pthread_detach(sem_mensajes[LOCALIZED_POKEMON]);
	pthread_detach(sem_mensajes[CAUGHT_POKEMON]);

	finalizar_broker(broker_config,logger);
	return 0;
}




void enviar_mensajes_en_cola(int codigo_de_operacion){
	while(1){
		sem_wait(&cola_vacia[codigo_de_operacion]); //existen mensajes en la cola

		pthread_mutex_lock(&mutex[codigo_de_operacion]);
		mensaje[codigo_de_operacion] = extraer_mensaje(codigo_de_operacion);
		pthread_mutex_unlock(&mutex[codigo_de_operacion]);

		int bytes=0;
		void *sent_package=empaquetar_mensaje_broker(mensaje[codigo_de_operacion],&bytes);
		t_mensaje_catch *mensaje_catch= deserializar_paquete_catch_pokemon(sent_package);

		log_info(logger,"Mensaje recibido CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);

		if(list_size(suscriptores[codigo_de_operacion])==0){
			sem_wait(&sem_proceso[codigo_de_operacion]);
		}

		void enviar_a_suscriptores_catch(int socket){

			enviar_mensaje_nofree(socket,sent_package,bytes);
			log_info(logger,"Mensaje enviado CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);

		}

		list_iterate(suscriptores[codigo_de_operacion],&enviar_a_suscriptores_catch);
		free(mensaje_catch->pokemon);
		free(mensaje_catch);


		free(sent_package);
		free(mensaje[codigo_de_operacion]->payload);
		free(mensaje[codigo_de_operacion]);

	}
}


void extraer_new_pokemon(){
	while(1){

		sem_wait(&cola_vacia[NEW_POKEMON]); //existen mensajes en la cola


			pthread_mutex_lock(&mutex[NEW_POKEMON]);
			mensaje[NEW_POKEMON] = extraer_mensaje(NEW_POKEMON);
			pthread_mutex_unlock(&mutex[NEW_POKEMON]);

			int bytes=0;
			void *sent_package=empaquetar_mensaje_broker(mensaje[NEW_POKEMON],&bytes);

			if(list_size(suscriptores[NEW_POKEMON])==0){
				sem_wait(&sem_proceso[NEW_POKEMON]);
			}


			void enviar_a_suscriptores_new(int socket){
				enviar_mensaje_nofree(socket,sent_package,bytes);
			}

			list_iterate(suscriptores[NEW_POKEMON],&enviar_a_suscriptores_new);


			free(sent_package);
			free(mensaje[NEW_POKEMON]->payload);
			free(mensaje[NEW_POKEMON]);


	}
}



void extraer_get_pokemon(){
	while(1){
		sem_wait(&cola_vacia[GET_POKEMON]); //existen mensajes en la cola


			pthread_mutex_lock(&mutex[GET_POKEMON]);
			mensaje[GET_POKEMON] = extraer_mensaje(GET_POKEMON);
			pthread_mutex_unlock(&mutex[GET_POKEMON]);

			int bytes=0;
			void *sent_package=empaquetar_mensaje_broker(mensaje[GET_POKEMON],&bytes);

			if(list_size(suscriptores[GET_POKEMON])==0){
				sem_wait(&sem_proceso[GET_POKEMON]);
			}


			void enviar_a_suscriptores_get(int socket){
				enviar_mensaje_nofree(socket,sent_package,bytes);
			}

			list_iterate(suscriptores[GET_POKEMON],&enviar_a_suscriptores_get);


			free(sent_package);
			free(mensaje[GET_POKEMON]->payload);
			free(mensaje[GET_POKEMON]);


	}
}



void extraer_catch_pokemon(){
	while(1){


		sem_wait(&cola_vacia[CATCH_POKEMON]); //existen mensajes en la cola


		pthread_mutex_lock(&mutex[CATCH_POKEMON]);
		mensaje[CATCH_POKEMON] = extraer_mensaje(CATCH_POKEMON);
		pthread_mutex_unlock(&mutex[CATCH_POKEMON]);

		int bytes=0;
		void *sent_package=empaquetar_mensaje_broker(mensaje[CATCH_POKEMON],&bytes);
		t_mensaje_catch *mensaje_catch= deserializar_paquete_catch_pokemon(sent_package);



		log_info(logger,"Mensaje recibido CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);


		if(list_size(suscriptores[CATCH_POKEMON])==0){
			sem_wait(&sem_proceso[CATCH_POKEMON]);
		}


		void enviar_a_suscriptores_catch(int socket){

			enviar_mensaje_nofree(socket,sent_package,bytes);
			log_info(logger,"Mensaje enviado CATCH_POKEMON %s %d %d",mensaje_catch->pokemon, mensaje_catch->posx,mensaje_catch->posy);

		}


		list_iterate(suscriptores[CATCH_POKEMON],&enviar_a_suscriptores_catch);
		free(mensaje_catch->pokemon);
		free(mensaje_catch);


		free(sent_package);
		free(mensaje[CATCH_POKEMON]->payload);
		free(mensaje[CATCH_POKEMON]);

	}
}

void extraer_appeared_pokemon(){
	while(1){
		sem_wait(&cola_vacia[APPEARED_POKEMON]); //existen mensajes en la cola


			pthread_mutex_lock(&mutex[APPEARED_POKEMON]);
			mensaje[APPEARED_POKEMON] = extraer_mensaje(APPEARED_POKEMON);
			pthread_mutex_unlock(&mutex[APPEARED_POKEMON]);

			int bytes=0;
			void *sent_package=empaquetar_mensaje_broker(mensaje[APPEARED_POKEMON],&bytes);

			if(list_size(suscriptores[APPEARED_POKEMON])==0){
				sem_wait(&sem_proceso[APPEARED_POKEMON]);
			}


			void enviar_a_suscriptores_appeared(int socket){
				enviar_mensaje_nofree(socket,sent_package,bytes);
			}

			list_iterate(suscriptores[APPEARED_POKEMON],&enviar_a_suscriptores_appeared);


			free(sent_package);
			free(mensaje[APPEARED_POKEMON]->payload);
			free(mensaje[APPEARED_POKEMON]);


	}
}

void extraer_localized_pokemon(){
	while(1){
		sem_wait(&cola_vacia[LOCALIZED_POKEMON]); //existen mensajes en la cola


			pthread_mutex_lock(&mutex[LOCALIZED_POKEMON]);
			mensaje[LOCALIZED_POKEMON] = extraer_mensaje(LOCALIZED_POKEMON);
			pthread_mutex_unlock(&mutex[LOCALIZED_POKEMON]);

			int bytes=0;
			void *sent_package=empaquetar_mensaje_broker(mensaje[LOCALIZED_POKEMON],&bytes);

			if(list_size(suscriptores[LOCALIZED_POKEMON])==0){
				sem_wait(&sem_proceso[LOCALIZED_POKEMON]);
			}


			void enviar_a_suscriptores_localized(int socket){
				enviar_mensaje_nofree(socket,sent_package,bytes);
			}

			list_iterate(suscriptores[LOCALIZED_POKEMON],&enviar_a_suscriptores_localized);


			free(sent_package);
			free(mensaje[LOCALIZED_POKEMON]->payload);
			free(mensaje[LOCALIZED_POKEMON]);


	}
}

void extraer_caught_pokemon(){
	while(1){
		sem_wait(&cola_vacia[CAUGHT_POKEMON]); //existen mensajes en la cola


			pthread_mutex_lock(&mutex[CAUGHT_POKEMON]);
			mensaje[CAUGHT_POKEMON] = extraer_mensaje(CAUGHT_POKEMON);
			pthread_mutex_unlock(&mutex[CAUGHT_POKEMON]);

			int bytes=0;
			void *sent_package=empaquetar_mensaje_broker(mensaje[CAUGHT_POKEMON],&bytes);

			if(list_size(suscriptores[CAUGHT_POKEMON])==0){
				sem_wait(&sem_proceso[CAUGHT_POKEMON]);
			}


			void enviar_a_suscriptores_caught(int socket){
				enviar_mensaje_nofree(socket,sent_package,bytes);
			}

			list_iterate(suscriptores[CAUGHT_POKEMON],&enviar_a_suscriptores_caught);


			free(sent_package);
			free(mensaje[CAUGHT_POKEMON]->payload);
			free(mensaje[CAUGHT_POKEMON]);


	}
}


