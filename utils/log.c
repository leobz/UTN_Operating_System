#include "log.h"

t_log* iniciar_logger(char* nombre_archivo, char* nombre_programa, t_log_level nivel_detalle)
{
	return log_create(nombre_archivo, nombre_programa, 0, nivel_detalle);
}

void destruir_logger(t_log* logger)
{
	log_destroy(logger);
}
