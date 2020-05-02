#include "gameboy.h"
#include "test/testing.h"

int main(int argc, char ** argv) {
	if (argc >= 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();
	} else {
		t_gameboy_config *gameboy_config = cargar_gameboy_config(
				"gameboy.config");
		destruir_gameboy_config(gameboy_config);
		return 0;
	}
}

void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config) {
	gameboy_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	gameboy_config->ip_gamecard = strdup(
			config_get_string_value(config, "IP_GAMECARD"));
	gameboy_config->ip_team = strdup(
			config_get_string_value(config, "IP_TEAM"));
	gameboy_config->puerto_broker = atoi(
			config_get_string_value(config, "PUERTO_BROKER"));
	gameboy_config->puerto_gamecard = atoi(
			config_get_string_value(config, "PUERTO_GAMECARD"));
	gameboy_config->puerto_team = atoi(
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
	free(gameboy_config);
}
