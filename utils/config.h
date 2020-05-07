#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include<commons/config.h>



// ESTRUCTURAS
typedef struct {
	char *ip_broker;
	char *ip_team;
	char *ip_gamecard;
	char *puerto_broker;
	char *puerto_team;
	char *puerto_gamecard;
} t_socket_config;

t_config* leer_config(char* path_archivo);
void destruir_config(t_config* config);

void parsear_socket_config(t_socket_config *process_config, t_config *config);
void destruir_socket_config(t_socket_config *process_config);

#endif
