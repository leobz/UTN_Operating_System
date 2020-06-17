#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "servidor_gameboy.h"

// CONFIGURACION
void inicializar_gameboy(t_gameboy_config **gameboy_config, t_log **logger);
void finalizar_gameboy(t_gameboy_config* gameboy_config, t_log* logger);
void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config);


t_gameboy_config *cargar_gameboy_config(char *path_archivo);
void destruir_gameboy_config(t_gameboy_config *gameboy_config);


#endif /* GAMEBOY_H_ */
