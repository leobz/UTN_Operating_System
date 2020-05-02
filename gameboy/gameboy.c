#include "gameboy.h"
#include "test/testing.h"

t_gameboy_config* gameboy_config;
t_log* logger;

int main(int argc, char ** argv) {
	inicializar_gameboy(&gameboy_config, &logger);

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();

	} else {

		if (argc == 6) {
			if (strcmp(argv[1], "TEAM") == 0
					&& strcmp(argv[2], "APPEARED_POKEMON") == 0) {
				char *pokemon = argv[3];
				int pos_x = atoi(argv[4]);
				int pos_y = atoi(argv[5]);

				// TODO: Conectarse a Team

				log_info(logger, "Conexion establecida con [Team]");

				// TODO: Enviar mensaje serializado al Team

				log_info(logger,
						"Mensaje enviado a [Team]: APPEARED_POKEMON %s %d %d",
						pokemon, pos_x, pos_y);

			};
		}

	}

	finalizar_gameboy(gameboy_config, logger);
	return 0;
}

void inicializar_gameboy(t_gameboy_config **gameboy_config, t_log **logger) {
	*gameboy_config = cargar_gameboy_config("gameboy.config");
	*logger = iniciar_logger("gameboy.log", "gameboy", LOG_LEVEL_INFO);
}

void finalizar_gameboy(t_gameboy_config* gameboy_config, t_log* logger) {
	destruir_gameboy_config(gameboy_config);
	destruir_logger(logger);
}

void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config) {
	gameboy_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	gameboy_config->ip_gamecard = strdup(
			config_get_string_value(config, "IP_GAMECARD"));
	gameboy_config->ip_team = strdup(
			config_get_string_value(config, "IP_TEAM"));
	gameboy_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));
	gameboy_config->puerto_gamecard = strdup(
			config_get_string_value(config, "PUERTO_GAMECARD"));
	gameboy_config->puerto_team = strdup(
			config_get_string_value(config, "PUERTO_TEAM"));
}

t_gameboy_config *cargar_gameboy_config(char *path_archivo) {
	t_config *config;
	t_gameboy_config *gameboy_config;

	config = leer_config(path_archivo);
	gameboy_config = malloc(sizeof(t_gameboy_config));

	parsear_gameboy_config(gameboy_config, config);
	destruir_config(config);
	return gameboy_config;
}

void destruir_gameboy_config(t_gameboy_config *gameboy_config) {
	free(gameboy_config->ip_broker);
	free(gameboy_config->ip_gamecard);
	free(gameboy_config->ip_team);
	free(gameboy_config->puerto_broker);
	free(gameboy_config->puerto_gamecard);
	free(gameboy_config->puerto_team);
	free(gameboy_config);
}
