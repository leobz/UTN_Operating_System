#include "config.h"



t_config* leer_config(char* path_archivo)
{
	return config_create(path_archivo);
}

void destruir_config(t_config* config)
{
	config_destroy(config);
}
