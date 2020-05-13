#include "log.h"

t_log* iniciar_logger(char* nombre_archivo, char* nombre_programa,
		t_log_level nivel_detalle) {
	t_log* logger;

	if ((logger = log_create(nombre_archivo, nombre_programa, 0, nivel_detalle))
			== NULL) {
		printf("\nERROR: No se pudo crear Logger\n");
		exit(1);
	}

	return logger;
}

void destruir_logger(t_log* logger) {
	log_destroy(logger);
}
