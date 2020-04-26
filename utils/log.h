#ifndef LOGGER_H_
#define LOGGER_H_

#include <commons/log.h>

t_log* iniciar_logger(char* nombre_archivo, char* nombre_programa, t_log_level nivel_detalle);
void destruir_logger(t_log* logger);

#endif
