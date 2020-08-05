#include "bloques.h"

void destruir_metadata_pokemon(t_metadata_pokemon* metadata_pokemon){
	list_destroy_and_destroy_elements(metadata_pokemon->blocks, (void*) free);
	free(metadata_pokemon->directory);
	free(metadata_pokemon);
}

t_bloque* crear_bloque(char* ruta_archivo) {
	FILE* archivo;
	archivo = fopen(ruta_archivo, "w");
	fclose(archivo);

	t_bloque* bloque = config_create(ruta_archivo);
	free(ruta_archivo);
	return bloque;
}
void cerrar_bloque(char*bloque_restante){
	char*ruta=ruta_blocks(bloque_restante);
	fclose(fopen(ruta, "w")); //permite limpiar el contenido de dicho bloque
	free(ruta);
}

t_config *cargar_config_desde_buffer(char* un_buffer) {
	t_config *config = malloc(sizeof(t_config));

	config->properties = dictionary_create();
	config->path = malloc(1);

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


char* archivo_a_string(char* ruta_absoluta) {
	char* ruta = ruta_absoluta;

	FILE* file = fopen(ruta, "rb");

	if (file == NULL) {
		file = fopen(ruta, "wb");
	}

	struct stat stat_file;
	stat(ruta, &stat_file);

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	fclose(file);

	return buffer;
}

void liberar_char_doble_asterisco(char** char_doble_asterisco) {
	int i = 0;
	while (char_doble_asterisco[i] != NULL) {
		free(char_doble_asterisco[i]);
		i++;
	}
	free(char_doble_asterisco);
}

t_metadata_pokemon* leer_metadata_pokemon(char* ruta_al_metadata_bin_del_archivo) {
	t_metadata_pokemon* archivo = malloc(sizeof(t_metadata_pokemon));

	char* ruta = crear_ruta(ruta_al_metadata_bin_del_archivo);
	t_bloque* bloque_metadata_archivo = config_create(ruta);
	archivo->directory = strdup(config_get_string_value(bloque_metadata_archivo, "DIRECTORY"));
	char** string_blocks = config_get_array_value(bloque_metadata_archivo, "BLOCKS");
	archivo->blocks = strings_to_list(string_blocks);
	archivo->open = strdup(config_get_string_value(bloque_metadata_archivo, "OPEN"));
	archivo->size = config_get_int_value(bloque_metadata_archivo, "SIZE");

	free(ruta);

	liberar_char_doble_asterisco(string_blocks);
	config_destroy(bloque_metadata_archivo);

	return archivo;
}

char* ruta_blocks(char* numero_de_bloque) {
	char* ruta_bloque = string_new();
	string_append_with_format(&ruta_bloque, "Blocks/%s.bin",numero_de_bloque);
	char* ruta = crear_ruta(ruta_bloque);
	free(ruta_bloque);
	return ruta;
}

char* buffer_del_archivo_completo(t_metadata_pokemon* archivo) {
	char* buffer_archivo = string_new();
	void unir_bloques(char* numero_de_bloque) {
		char* buffer_bloque = string_new();
		char* ruta = ruta_blocks(numero_de_bloque);
		buffer_bloque = archivo_a_string(ruta);
		string_append(&buffer_archivo, buffer_bloque);
		free(buffer_bloque);
		free(ruta);
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

int escribir_buffer_en_bloque(char* buffer, char* numero_de_bloque) {
	char* ruta_bloque = ruta_blocks(numero_de_bloque);
	FILE* file = fopen(ruta_bloque, "wb+");

	if (file == NULL) {
			return -1;
	}

	int result = fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
	free(buffer);
	free(ruta_bloque);
	return result;
}

char* obtener_numero_de_bloque_disponible(){
	pthread_mutex_lock(&mutex_bitmap);
	int bloque_disponible= bloque_disponible_en_bitmap();
	pthread_mutex_unlock(&mutex_bitmap);

		return string_itoa(bloque_disponible);
	return NULL;
}

int escribir_archivo(t_metadata_pokemon* archivo, char* buffer_de_guardado,char*pokemon) {
	int size_buffer_de_guardado = string_length(buffer_de_guardado);
	int bloques_necesarios = cantidad_de_bloques_necesarios(size_buffer_de_guardado);
	int offset = 0;
	int i;

	for (i = 0; i < bloques_necesarios; i++) {
		char* sub_string_bloque = string_substring(buffer_de_guardado, offset, metadata->block_size);
		offset += metadata->block_size;
		char* numero_de_bloque = list_get(archivo->blocks, i);

		if(numero_de_bloque != NULL) {
			escribir_buffer_en_bloque(sub_string_bloque , numero_de_bloque);
		}
		else{
			char* numero_de_bloque_disponible = obtener_numero_de_bloque_disponible();
			if (numero_de_bloque_disponible != NULL) { ///Aca entrara cuando se tenga que agregar un nuevo bloque
				list_add(archivo->blocks,numero_de_bloque_disponible);
				setear_bloque_ocupado(atoi(numero_de_bloque_disponible));
				actualizar_vector_de_bloques_en_metadata(archivo,pokemon);
				escribir_buffer_en_bloque(sub_string_bloque, numero_de_bloque_disponible);
			}
			else {
				printf("Error: No mas bloques disponibles\n");
				// TODO que dato envio.
				// return -1;
			}
		}
	}

	while (list_size(archivo->blocks) > bloques_necesarios){ //Aca entrara cuando se tenga que eliminar un bloque
		char*bloque_restante = list_remove(archivo->blocks, i);
		setear_bloque_libre(atoi(bloque_restante));
		cerrar_bloque(bloque_restante);
		actualizar_vector_de_bloques_en_metadata(archivo,pokemon);
		i++;
	}
	return archivo->blocks;
}


void actualizar_vector_de_bloques_en_metadata(t_metadata_pokemon* archivo, char* pokemonn){

	char*vector_de_bloques = list_to_char_array(archivo->blocks);

	char*path_pokemon = formar_archivo_pokemon(pokemonn);
	char*path_absoluta = crear_ruta(path_pokemon);

	t_config*pokemon_config = config_create(path_absoluta);

	config_set_value(pokemon_config,"BLOCKS",vector_de_bloques);
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	free(path_pokemon);
	free(path_absoluta);
}

void actualizar_tamanio_archivo(char*buffer_pokemon,char*pokemonn){
	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);
	int space=strlen(buffer_pokemon);
	char* space_st = string_itoa(space);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config,"SIZE",space_st);
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	free(path_pokemon);
	free(path_absoluta);
	free(space_st);
}

char* formar_archivo_pokemon(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	return path_archivo_pokemon;
}

t_config* leer_config_pokemon(char* pokemon){
	char* archivo_pokemon = formar_archivo_pokemon(pokemon);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(archivo_pokemon);
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);
	t_config* config_pokemon = cargar_config_desde_buffer(buffer_pokemon);

	free(archivo_pokemon);
	destruir_metadata_pokemon(metadata_pokemon);
	free(buffer_pokemon);

	return config_pokemon;
}

void guardar_config_en_archivo_pokemon(t_config* config_pokemon, char *pokemon) {
	char* archivo_pokemon = formar_archivo_pokemon(pokemon);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(archivo_pokemon);
	char* buffer_pokemon = config_save_to_buffer(config_pokemon);
	actualizar_tamanio_archivo(buffer_pokemon,pokemon);
	escribir_archivo(metadata_pokemon, buffer_pokemon, pokemon);

	free(archivo_pokemon);
	destruir_metadata_pokemon(metadata_pokemon);
	free(buffer_pokemon);

}
