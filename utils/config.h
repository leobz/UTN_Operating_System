#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include<commons/config.h>


t_config* leer_config(char* path_archivo);
void destruir_config(t_config* config);


#endif
