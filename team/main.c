#include "main.h"
t_log* logger;

int main(){
	logger = iniciar_logger("team.log", "team", LOG_LEVEL_INFO);

	// TODO:HardCodeo datos de conexion con gameboy(puerto e IP)
	//Borrar cuando pueda cargarse por archivo de configuracion
	char* ip = "127.0.0.1";
	char* puerto = "4444";

	inicializar_objetivo_global();

	// TODO:HardCodeo dos pikachus en el Objetivo global.
	//Borrar cuando pueda cargarse por archivo de configuracion
	agregar_pokemon_a_objetivo_global("pikachu", 2);

	crear_pokemon_requeridos();

	int socket_servidor = iniciar_servidor(ip, puerto);

    while(1)
    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

    destruir_objetivo_global();
    destruir_pokemon_requeridos();

    return 0;
}
