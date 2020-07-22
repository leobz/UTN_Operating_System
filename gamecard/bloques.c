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
	string_append_with_format(&ruta_bloque, "Blocks/%s.txt",numero_de_bloque);
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
	int contador_bloques=++contador_bloques_totales;
	if(contador_bloques< metadata->blocks)
		return string_itoa(contador_bloques);
	return NULL;
}

int escribir_archivo(t_metadata_pokemon* archivo, char* buffer_de_guardado,char*pokemon) {
	int size_buffer_de_guardado = string_length(buffer_de_guardado);
	int bloques_necesarios = cantidad_de_bloques_necesarios(size_buffer_de_guardado);
	int offset = 0;

	int i=0;
	for (i= 0; i <bloques_necesarios; i++) {
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
				actualizar_vector_de_bloques_en_metadata(archivo,pokemon);
				escribir_buffer_en_bloque(sub_string_bloque, numero_de_bloque_disponible);
			}
			else {
				printf("Error: No mas bloques disponibles\n");
				return -1;
			}
		}
	}
	while (list_size(archivo->blocks) > bloques_necesarios){ //Aca entrara cuando se tenga que eliminar un bloque
		list_remove(archivo->blocks, i);
		actualizar_vector_de_bloques_en_metadata(archivo,pokemon);
		i++;
	}
	return 1;
}


void actualizar_vector_de_bloques_en_metadata(t_metadata_pokemon*archivo,char*pokemonn){

	char*vector_de_bloques=list_to_char_array(archivo->blocks);

	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);

	t_config*pokemon_config=config_create(path_absoluta);

	config_set_value(pokemon_config,"BLOCKS",vector_de_bloques);
	config_save(pokemon_config);
	config_destroy(pokemon_config);

}

void actualizar_tamanio_archivo(char*buffer_pokemon,char*pokemonn){
	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);
	int space=strlen(buffer_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config,"SIZE",string_itoa(space));
	config_save(pokemon_config);
	config_destroy(pokemon_config);
}

char* formar_archivo_pokemon(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	string_append(&path_archivo_pokemon, "/Metadata.txt");
	return path_archivo_pokemon;
}

t_config* leer_config_pokemon(char* pokemon){
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemon));
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);
	t_config* config_pokemon = cargar_config_desde_buffer(buffer_pokemon);

	free(metadata_pokemon);
	free(buffer_pokemon);

	return config_pokemon;
}

void guardar_config_en_archivo_pokemon(t_config* config_pokemon, char *pokemon) {
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemon));
	char* buffer_pokemon = config_save_to_buffer(config_pokemon);
	actualizar_tamanio_archivo(buffer_pokemon,pokemon);
	escribir_archivo(metadata_pokemon, buffer_pokemon,pokemon);

	free(metadata_pokemon);
	free(buffer_pokemon);

}
