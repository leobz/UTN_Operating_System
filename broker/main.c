#include "broker.h"
#include "server.h"

int main(){



	int socket_servidor = iniciar_servidor(IP, PUERTO);

    while(1){
    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);}


	return 0;
}
