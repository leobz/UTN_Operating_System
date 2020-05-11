#include "config.h"

t_config* leer_config(char* path_archivo) {
	t_config* config;
	if ((config = config_create(path_archivo)) == NULL) {
		printf(
				"ERROR: Archivo de configuracion '%s' no existe o es invalido\n",
				path_archivo);
		exit(1);
	}

	return config;
}

void destruir_config(t_config* config) {
	config_destroy(config);
}
