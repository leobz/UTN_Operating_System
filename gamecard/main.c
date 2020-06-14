#include "main.h"

int main(){
	inicializar_gamecard();
	suscribirme_al_broker(gamecard_config);

	// Creo la conexion para escuchar
	char* ip = gamecard_config->ip_gamecard;
	char* puerto = gamecard_config->puerto_gamecard;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1){
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}

	finalizar_gamecard();
}
