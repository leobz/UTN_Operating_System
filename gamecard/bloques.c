#include "bloques.h"

t_bloque* crear_bloque(char* ruta_archivo) {
	FILE* archivo;
	archivo = fopen(ruta_archivo, "wb");
	fclose(archivo);

	t_bloque* bloque = config_create(ruta_archivo);
	return bloque;
}


t_config *cargar_bloque_desde_buffer(char* un_buffer) {
	t_config *config = malloc(sizeof(t_config));

	config->properties = dictionary_create();

	char* buffer = strdup(un_buffer);

	char** lines = string_split(buffer, "\n");

	void add_cofiguration(char *line) {
		if (!string_starts_with(line, "#")) {
			char** keyAndValue = string_n_split(line, 2, "=");
			dictionary_put(config->properties, keyAndValue[0], keyAndValue[1]);

			free(keyAndValue[0]);
			free(keyAndValue);
		}
	}

	string_iterate_lines(lines, add_cofiguration);
	string_iterate_lines(lines, (void*) free);

	free(lines);
	free(buffer);

	return config;
}

char* crear_ruta(char* ruta) {
	char* path_ruta_absoluta = string_new();
	string_append(&path_ruta_absoluta, gamecard_config->punto_montaje_tallgrass);
	string_append(&path_ruta_absoluta, "/");
	string_append(&path_ruta_absoluta, ruta);

	return path_ruta_absoluta;
}

char* archivo_a_string(char* ruta) {
	char* file_name = crear_ruta(ruta);

	char* buffer_leido = string_new();
	struct stat s;
	int fd = open(file_name, O_RDWR | O_CREAT, (mode_t) 0600);
	int status = fstat(fd, &s);
	int size = s.st_size;

	buffer_leido = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);

	return buffer_leido;
}


t_archivo* leer_archivo(char* ruta) {
	t_archivo* archivo = malloc(sizeof(t_archivo));
	t_bloque* bloque_metadata_archivo = config_create(crear_ruta(ruta));

	archivo->directory = config_get_string_value(bloque_metadata_archivo, "DIRECTORY");
	archivo->blocks = strings_to_list(config_get_array_value(bloque_metadata_archivo, "BLOCKS"));
	archivo->open = config_get_string_value(bloque_metadata_archivo, "OPEN");
	archivo->size = config_get_int_value(bloque_metadata_archivo, "SIZE");

	return archivo;
}

char* ruta_blocks(char* numero_de_bloque) {
	char* ruta_bloque = string_new();
	string_append_with_format(&ruta_bloque, "Blocks/%s.bin",numero_de_bloque);
	return ruta_bloque;
}

char* buffer_archivo(t_archivo* archivo) {
	char* buffer_archivo = string_new();

	void unir_bloques(char* numero_de_bloque) {
		char* ruta_bloque = ruta_blocks(numero_de_bloque);
		char* buffer_bloque = archivo_a_string(ruta_bloque);
		string_append(&buffer_archivo, buffer_bloque);
		//TODO: Liberar ruta bloque y buffer bloque

	}

	list_iterate(archivo->blocks, unir_bloques);

	return buffer_archivo;
}

