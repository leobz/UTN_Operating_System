#ifndef GAMEBOY_H_
#define GAMEBOY_H_

// ESTRUCTURAS
typedef struct {
	char *ip_broker;
	char *ip_team;
	char *ip_gamecard;
	int puerto_broker;
	int puerto_team;
	int puerto_gamecard;
} t_gameboy_config;

// CONFIGURACION
t_gameboy_config *cargar_gameboy_config(char *path_archivo);
void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config);
void destruir_gameboy_config(t_gameboy_config *gameboy_config);

#endif /* GAMEBOY_H_ */
