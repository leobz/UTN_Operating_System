#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <commons/config.h>
#include "../utils/config.h"
#include "../utils/log.h"
#include "../utils/serializacion.h"

// ESTRUCTURAS
typedef struct {
	char *ip_broker;
	char *ip_team;
	char *ip_gamecard;
	char *puerto_broker;
	char *puerto_team;
	char *puerto_gamecard;
} t_gameboy_config;

// CONFIGURACION
void inicializar_gameboy(t_gameboy_config **gameboy_config, t_log **logger);
void finalizar_gameboy(t_gameboy_config* gameboy_config, t_log* logger);
void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config);
t_gameboy_config *cargar_gameboy_config(char *path_archivo);
void destruir_gameboy_config(t_gameboy_config *gameboy_config);

#endif /* GAMEBOY_H_ */
