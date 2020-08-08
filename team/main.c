#include "main.h"
#include "test/testing.h"

t_log* logger;
t_team_config* team_config;

void finalizar_team(t_team_config* team_config) {
	log_info(logger_debug,"Cerrando programa...");
	destruir_metricas();
	destruir_objetivo_global();
	//destruir_pokemon_requeridos();
	destruir_team_config(team_config);
	destruir_logger(logger_debug);
	destruir_logger(logger);
	destroy_all_tcbs();

	exit(1);
}

int main(int argc, char ** argv) {

	//TODO: Tomar id proceso por archivo de configuracion

	team_cumplio_objetivo = false;

	team_config = cargar_team_config("team.config");
	logger = iniciar_logger(team_config->path_logger, "team", LOG_LEVEL_INFO);
	logger_debug = iniciar_logger("team_debug.log", "team", LOG_LEVEL_INFO);

	//CAMBIAR POR CONFIGURACION
	int id_proceso=team_config->id_proceso;
	char* ip_team=team_config->ip_team;
	char* puerto_team = team_config->puerto_team;

	inicializar_listas();
	inicializar_diccionarios(team_config);
	inicializar_metricas();
	crear_tcb_entrenadores(team_config);

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			mostrar_lista_entrenadores(team_config);
			correrTests();
	} else {
		int socket_servidor = iniciar_servidor(ip_team, puerto_team);

		iniciar_planificador();

		t_datos_suscripcion* datos_suscripcion_appeared = crear_datos_suscripcion(id_proceso, APPEARED_POKEMON);
		t_datos_suscripcion* datos_suscripcion_localized = crear_datos_suscripcion(id_proceso, LOCALIZED_POKEMON);
		t_datos_suscripcion* datos_suscripcion_caught = crear_datos_suscripcion(id_proceso, CAUGHT_POKEMON);

		iniciar_hilo_suscripcion(datos_suscripcion_appeared);
		iniciar_hilo_suscripcion(datos_suscripcion_localized);
		iniciar_hilo_suscripcion(datos_suscripcion_caught);

		enviar_get_pokemon();

		while (!team_cumplio_objetivo)
			esperar_cliente(socket_servidor, &procesar_mensaje_recibido_cliente);

		finalizar_team(team_config);
		return 0;
	}

}
