#include "config.h"



void parsear_socket_config(t_socket_config *process_config, t_config *config) {
	process_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	process_config->ip_gamecard = strdup(
			config_get_string_value(config, "IP_GAMECARD"));
	process_config->ip_team = strdup(
			config_get_string_value(config, "IP_TEAM"));
	process_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));
	process_config->puerto_gamecard = strdup(
			config_get_string_value(config, "PUERTO_GAMECARD"));
	process_config->puerto_team = strdup(
			config_get_string_value(config, "PUERTO_TEAM"));
}

void destruir_socket_config(t_socket_config *process_config) {
	free(process_config->ip_broker);
	free(process_config->ip_gamecard);
	free(process_config->ip_team);
	free(process_config->puerto_broker);
	free(process_config->puerto_gamecard);
	free(process_config->puerto_team);
	free(process_config);
}

t_config* leer_config(char* path_archivo)
{
	return config_create(path_archivo);
}

void destruir_config(t_config* config)
{
	config_destroy(config);
}
