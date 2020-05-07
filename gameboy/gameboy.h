#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <commons/config.h>
#include "../utils/config.h"
#include "../utils/log.h"
#include "../utils/serializacion.h"
#include "../utils/cliente.h"



// CONFIGURACION
void inicializar_gameboy(t_socket_config **gameboy_config, t_log **logger);
void finalizar_gameboy(t_socket_config* gameboy_config, t_log* logger);
void parsear_socket_config(t_socket_config *gameboy_config, t_config *config);
t_socket_config *cargar_gameboy_config(char *path_archivo);
void destruir_gameboy_config(t_socket_config *gameboy_config);

#endif /* GAMEBOY_H_ */
