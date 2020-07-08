#include "main.h"
#include "test/testing.h"

t_log* logger;
t_team_config* team_config;

int main(int argc, char ** argv) {

	//TODO: Tomar id proceso por archivo de configuracion
	int id_proceso = 999;

	logger = iniciar_logger("team.log", "team", LOG_LEVEL_INFO);
	team_config = cargar_team_config("team.config");

	char* puerto = team_config->puerto_broker;
	char* ip = team_config->ip_broker;

	char* puerto_gameboy = "4444";

	inicializar_listas();
	inicializar_diccionarios(team_config);
	crear_tcb_entrenadores(team_config);

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			mostrar_lista_entrenadores(team_config);
			correrTests();
	} else {
		int socket_servidor = iniciar_servidor(ip, puerto_gameboy);

		iniciar_planificador();

		iniciar_suscripcion_appeared(id_proceso, ip, puerto);

		while (1)
			esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

		destruir_objetivo_global();
		destruir_pokemon_requeridos();
		destruir_team_config(team_config);
		return 0;
	}

}
