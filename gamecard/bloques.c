#include "bloques.h"

t_bloque* crear_bloque(char* ruta_archivo) {
	FILE* archivo;
	archivo = fopen(ruta_archivo, "wb");
	fclose(archivo);

	t_bloque* bloque = config_create(ruta_archivo);
	return bloque;
}
