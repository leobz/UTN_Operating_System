#include "main.h"

int main(){
	inicializar_gamecard();
	suscribirme_al_broker(gamecard_config);
	/* TODO Aca no me queda claro si no me suscribo porque tengo que seguir con lo demas.
	   Que mensaje voy a esperar */

	// Creo la conexion para escuchar
	/*
	char* ip = gamecard_config->ip_gamecard;
	char* puerto = gamecard_config->puerto_gamecard;
	int socket_servidor = iniciar_servidor(ip, puerto);
	while (1){
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}
	*/
	finalizar_gamecard();
}
