
#include "main.h"



int main() {

	inicializar_broker(&broker_config,&logger);
	char*ip=broker_config->ip_broker;
	char*puerto=broker_config->puerto_broker;
	int socket_servidor = iniciar_servidor(ip, puerto);


	for(int i = 0; i < 6; i++)
	     sem_init(&cola_vacia[i], 0, 0);

	pthread_create(&sem_mensajes[NEW_POKEMON],NULL,(void*)extraer_new_pokemon,NULL);

	while (1) {
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}


	pthread_detach(sem_mensajes[NEW_POKEMON]);
	finalizar_broker(broker_config,logger);
	return 0;
}

void extraer_new_pokemon(){
	while(1){
		sem_wait(&cola_vacia[NEW_POKEMON]);
		mensaje[NEW_POKEMON] = extraer_mensaje(NEW_POKEMON);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[NEW_POKEMON]==false){
			sem_wait(&sem_proceso[NEW_POKEMON]);
				proceso=desencolar_proceso(NEW_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[NEW_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "mensaje: %d",mensaje[NEW_POKEMON]->id_correlativo);
		free(mensaje[NEW_POKEMON]->payload);
		free(mensaje[NEW_POKEMON]);
	}
}
