#include "../utils/team.h"

int main(){
	char* ip = "127.0.0.1";
	char* puerto = "4444";

	inicializar_objetivo_global();
	crear_pokemon_requeridos();

//	int socket_servidor = iniciar_servidor(ip, puerto);
//
//    while(1)
//    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

    destruir_objetivo_global();
    destruir_pokemon_requeridos();

    return 0;
}
