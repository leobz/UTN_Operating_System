#include "bloques.h"

t_bloque* crear_bloque(char* ruta_archivo) {
	FILE* archivo;
	archivo = fopen(ruta_archivo, "wb");
	fclose(archivo);

	t_bloque* bloque = config_create(ruta_archivo);
	return bloque;
}


t_config *cargar_config_desde_buffer(char* un_buffer) {
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

char* archivo_a_string(char* ruta_absoluta) {
	char* ruta = ruta_absoluta;

	FILE* file = fopen(ruta, "r");

	if (file == NULL) {
		printf("Error: Archivo %s no existe, creandolo...\n", ruta);
		file = fopen(ruta, "wb");
	}

	struct stat stat_file;
	stat(ruta, &stat_file);

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	fclose(file);

	return buffer;
}


t_metadata_pokemon* leer_metadata_pokemon(char* ruta_al_metadata_bin_del_archivo) {
	t_metadata_pokemon* archivo = malloc(sizeof(t_metadata_pokemon));
	t_bloque* bloque_metadata_archivo = config_create(crear_ruta(ruta_al_metadata_bin_del_archivo));

	archivo->directory = config_get_string_value(bloque_metadata_archivo, "DIRECTORY");
	archivo->blocks = strings_to_list(config_get_array_value(bloque_metadata_archivo, "BLOCKS"));
	archivo->open = config_get_string_value(bloque_metadata_archivo, "OPEN");
	archivo->size = config_get_int_value(bloque_metadata_archivo, "SIZE");

	return archivo;
}

char* ruta_blocks(char* numero_de_bloque) {
	char* ruta_bloque = string_new();
	string_append_with_format(&ruta_bloque, "Blocks/%s.bin",numero_de_bloque);
	return crear_ruta(ruta_bloque);
}

char* buffer_del_archivo_completo(t_metadata_pokemon* archivo) {
	char* buffer_archivo = string_new();

	void unir_bloques(char* numero_de_bloque) {
		char* buffer_bloque = archivo_a_string(ruta_blocks(numero_de_bloque));

		string_append(&buffer_archivo, buffer_bloque);
		//TODO: Liberar ruta bloque y buffer bloque

	}

	list_iterate(archivo->blocks, unir_bloques);

	return buffer_archivo;
}

char* config_save_to_buffer(t_config *self) {
	char* lines = string_new();
	void add_line(char* key, void* value) {
		string_append_with_format(&lines, "%s=%s\n", key, value);
	}

	dictionary_iterator(self->properties, add_line);
	return lines;
}

int cantidad_de_bloques_necesarios(int size_buffer_de_guardado) {
	double cantidad = (double) size_buffer_de_guardado / (double)metadata->block_size;
	return ceil(cantidad);
}

int escribir_buffer_en_bloque(char* buffer, char*  numero_de_bloque) {
	char* ruta_bloque = ruta_blocks(numero_de_bloque);
	FILE* file = fopen(ruta_bloque, "wb+");

	if (file == NULL) {
			return -1;
	}

	int result = fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
	free(buffer);
	return result;
}

char* obtener_numero_de_bloque_disponible(){
	//TODO : Hacer esta funcion (Ahora esta hardcodeada)
	int contador_bloques=contador_bloques_totales++;
	if(contador_bloques< metadata->blocks)
		return string_itoa(numero_de_bloque_disponible);
	return NULL;
}

int escribir_archivo(t_metadata_pokemon* archivo, char* buffer_de_guardado) {
	int size_buffer_de_guardado = string_length(buffer_de_guardado);
	int bloques_necesarios = cantidad_de_bloques_necesarios(size_buffer_de_guardado);
	int offset = 0;

	for (int i= 0; i <bloques_necesarios; i++) {
		char* sub_string_bloque = string_substring(buffer_de_guardado, offset, metadata->block_size);
		offset += metadata->block_size;
		char* numero_de_bloque = list_get(archivo->blocks, i);

		if(numero_de_bloque != NULL) {
			escribir_buffer_en_bloque(sub_string_bloque , numero_de_bloque);

		}
		else{
			char* numero_de_bloque_disponible = obtener_numero_de_bloque_disponible();
			if (numero_de_bloque_disponible != NULL) {
				escribir_buffer_en_bloque(sub_string_bloque, numero_de_bloque_disponible);
			}
			else {
				printf("Error: No mas bloques disponibles\n");
				return -1;
			}
		}
	}
	return 1;
}

t_config* leer_config_pokemon(char* pokemon){
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemon, false));
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);
	t_config* config_pokemon = cargar_config_desde_buffer(buffer_pokemon);

	free(metadata_pokemon);
	free(buffer_pokemon);

	return config_pokemon;
}

void guardar_config_en_archivo_pokemon(t_config* config_pokemon, char *pokemon) {
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemon, false));
	char* buffer_pokemon = config_save_to_buffer(config_pokemon);
	escribir_archivo(metadata_pokemon, buffer_pokemon);

	free(metadata_pokemon);
	free(buffer_pokemon);
}
