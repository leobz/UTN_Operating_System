#ifndef BLOQUES_H_
#define BLOQUES_H_

#include "../utils/config.h"

typedef t_config t_bloque;


/**
* @NAME: crear_bloque(char* ruta_del_archivo)
* @DESC: Abre un archivo de texto de tipo bloque, lee todas sus keys
* y las parsea a un formato t_bloque(t_config). Si no existe, lo crea
*/
t_bloque* crear_bloque(char* ruta_archivo);

#endif /* BLOQUES_H_ */
