#include "directorios.h"

//Inicializar directorios-> t config
void inicializar_directorios() {

	//Creo Metadata

	path_directorio_metadata = string_new();
	string_append(&path_directorio_metadata,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_metadata, "/Metadata");
	mkdir(path_directorio_metadata, 0777);

	//Creo Files
	path_directorio_files = string_new();
	string_append(&path_directorio_files,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_files, "/Files");
	mkdir(path_directorio_files, 0777);

	//Creo Blocks
	//TODO: escribir archivos blocks
	path_directorio_blocks = string_new();
	string_append(&path_directorio_blocks,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_blocks, "/Blocks");
	mkdir(path_directorio_blocks, 0777);

	t_metadata* metadata = malloc(sizeof(t_metadata));
	metadata->block_size = 64;
	metadata->blocks = 5192;
	metadata->magic_number = "TALL_GRASS";
	t_metadata* metadata_aux = malloc(sizeof(t_metadata));

	//Creo archivo Bitmap.bin
	FILE* bitmap_file;
	char* path_bitmap = string_new();
	string_append(&path_bitmap, path_directorio_metadata);
	string_append(&path_bitmap, "/Bitmap.bin");
	bitmap_file = fopen(path_bitmap, "wb");
	free(path_bitmap);
	fclose(bitmap_file);

	//Creo archivo Metadata.bin
	t_bloque* bloque_metadata_bin = crear_bloque(crear_ruta("Metadata/Metadata.bin"));
	config_set_value(bloque_metadata_bin, "BLOCK_SIZE", string_itoa(metadata->block_size));
	config_set_value(bloque_metadata_bin, "BLOCKS", string_itoa(metadata->blocks));
	config_set_value(bloque_metadata_bin, "MAGIC_NUMBER", metadata->magic_number);

	config_save(bloque_metadata_bin);

	free(metadata);
	free(metadata_aux);

	//liberar_paths();

}

FILE* crear_archivo_pokemon(t_mensaje_new* mensaje_new) {
	char* path_archivo_pokemon = string_new();
	string_append_with_format(&path_archivo_pokemon, "%s",
			mensaje_new->pokemon);
	mkdir(path_archivo_pokemon, 0777);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	FILE* metadata_file = fopen(path_archivo_pokemon, "wb");

	char* metada_chars = string_new();
	string_append(&metada_chars, "DIRECTORY=N\n");
	string_append(&metada_chars, "SIZE=0\n");
	string_append(&metada_chars, "BLOCKS=[]");
	string_append(&metada_chars, "OPEN=N");
	fputs(metada_chars, metadata_file);

	return metadata_file;
}

bool esta_abierto(FILE* archivo) {
//	TODO
}

void setear_abierto(FILE* archivo) {
//	TODO
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(esta_en_diccionario(pokemones_gamecard,mensaje_new->pokemon)){} //ya que el archio persistira es lo mismo que buscarlo
																		//por el diccionario, sin embargo puede cambiarse
																		//incluso para testear, buscarlo en el archivo
	/* Existe pokemon?
	 * NO
	 * 	Crear direcotorio del nuevo pokemon
	 * 	Crear el archivo del metadata
	 * 	Completar el metadata.bin
	 * SI
	 * 	Se encuentra abierto?
	 * 	SI
	 * 	 Reintentar el tiempo que esta configurado
	 * 	NO
	 * 	 Abrir todos los blocks
	 * 	 Concatenar
	 * 	 Buscar la posicion
	 * 	 Existe la posicion?
	 * 	 NO
	 */
	char* path_archivo_pokemon = string_new();
	string_append_with_format(&path_archivo_pokemon, "%s",&path_directorio_files);
	string_append_with_format(&path_archivo_pokemon, "%s",mensaje_new->pokemon);
	string_append(&path_archivo_pokemon, "/Metadata.bin");

	FILE* pokemon_metadata = fopen(pokemon_metadata, "rb");

	if (pokemon_metadata == NULL) {
		printf("no se encontro el directorio %s", &path_archivo_pokemon);
		pokemon_metadata = crear_archivo_pokemon(mensaje_new);
	}

	while (esta_abierto(pokemon_metadata)) {
//		TODO
	}

	setear_abierto(pokemon_metadata);

}


void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_get;
	mensaje_get=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(esta_en_diccionario(pokemones_gamecard,mensaje_get->pokemon)){}

}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_catch;
	mensaje_catch=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

		if(esta_en_diccionario(pokemones_gamecard,mensaje_catch->pokemon)){}

}



