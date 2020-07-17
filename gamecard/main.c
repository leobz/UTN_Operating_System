#include "main.h"


int main(){
	inicializar_gamecard();
	inicializar_directorios();
	inicializar_diccionarios();

	pthread_create(&suscripcion_broker, NULL, (void*)suscribirme_al_broker, NULL);

	// Creo la conexion para escuchar
	char* ip = gamecard_config->ip_gamecard;
	char* puerto = gamecard_config->puerto_gamecard;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1){
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}

	pthread_detach(suscripcion_broker);

	finalizar_gamecard();
}
