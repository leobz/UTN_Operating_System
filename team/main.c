#include "team.h"

int main(){

	t_team_config *team_config = cargar_team_config("team.config");

	char* puerto = team_config->puerto_broker;
	char* ip = team_config->ip_broker;

	cargar_objetivo_global();
	//crear_tcb_entrenadores(t_team_config);

	crear_pokemon_requeridos();

//	int socket_servidor = iniciar_servidor(ip, puerto);
//
//    while(1)
//    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

    destruir_objetivo_global();
    destruir_pokemon_requeridos();
    destruir_team_config(team_config);
    return 0;
}
