#include "directorios.h"

//Inicializar directorios-> t config
void inicializar_directorios() {

	//Creo Metadata

	path_directorio_metadata = string_new();
	string_append(&path_directorio_metadata,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_metadata, "/Metadata");
	mkdir(path_directorio_metadata, 0777);

	//Creo Files
	path_directorio_files = string_new();
	string_append(&path_directorio_files,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_files, "/Files");
	mkdir(path_directorio_files, 0777);

	//Creo Blocks
	//TODO: escribir archivos blocks
	path_directorio_blocks = string_new();
	string_append(&path_directorio_blocks,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_blocks, "/Blocks");
	mkdir(path_directorio_blocks, 0777);

	metadata = malloc(sizeof(t_metadata));
	metadata->block_size = 64;
	metadata->blocks = 5192;
	metadata->magic_number = "TALL_GRASS";
	//t_metadata* metadata_aux = malloc(sizeof(t_metadata));

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

	//free(metadata);
	//free(metadata_aux);

	//liberar_paths();

}
void inicializar_diccionarios(){
archivos_existentes = dictionary_create();
//archivos_abiertos=dictionary_create();
}

void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {

	contador_bloques_totales++;

	if(contador_bloques_totales< metadata->blocks){

		char* path_archivo_pokemon = string_new();
		string_append(&path_archivo_pokemon, "Files/");
		string_append_with_format(&path_archivo_pokemon, "%s",mensaje_new->pokemon);
		mkdir(path_archivo_pokemon, 0777);
		string_append(&path_archivo_pokemon, "/Metadata.bin");

		t_bloque* pokemon_config=crear_bloque(crear_ruta(path_archivo_pokemon));

		config_set_value(pokemon_config, "DIRECTORY","N");
		config_set_value(pokemon_config, "SIZE","0");
		char* block = string_new();
		string_append(&block, "[");
		string_append_with_format(&block, "%s",string_itoa(contador_bloques_totales));
		string_append(&block, "]");
		config_set_value(pokemon_config, "BLOCKS",block);
		config_set_value(pokemon_config, "OPEN","N");

		dictionary_put(archivos_existentes,mensaje_new->pokemon,0);//indica que esta cerrado

		destruir_config(pokemon_config);
	}
	else{
		printf("ERROR: No existen bloques disponibles para crear el archivo\n");
	}
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

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){

			while(archivo_esta_abierto(mensaje_new->pokemon))
				sleep(gamecard_config->tiempo_reintento_operacion);

		pthread_mutex_lock(&mutex_abiertos[NEW_POKEMON]);
			char*path_pokemonn=setear_archivo_abierto(mensaje_new->pokemon);
		pthread_mutex_unlock(&mutex_abiertos[NEW_POKEMON]);

		t_archivo* archivo_pokemon_config=leer_archivo_de_datos(path_pokemonn);

		//agregar_posicion(mensaje_new,archivo_pokemon_config); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques

		cerrar_archivo(mensaje_new->pokemon);
	}

	else{

		crear_archivo_pokemon(mensaje_new);
		}
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

	/*FILE* pokemon_metadata = fopen(pokemon_metadata, "rb");

	if (pokemon_metadata == NULL) {
		printf("no se encontro el directorio %s", &path_archivo_pokemon);
		pokemon_metadata = crear_archivo_pokemon(mensaje_new);
	}

	while (esta_abierto(pokemon_metadata)) {
//		TODO
	}

	setear_abierto(pokemon_metadata);
*/
}


void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_get;
	mensaje_get=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(esta_en_diccionario(archivos_existentes,mensaje_get->pokemon)){}

}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_catch;
	mensaje_catch=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

		if(esta_en_diccionario(archivos_existentes,mensaje_catch->pokemon)){}

}

bool archivo_esta_abierto(char *pokemonn){
	return dictionary_get(archivos_existentes,pokemonn);
}

char* setear_archivo_abierto(char*pokemonn){

	dictionary_put(archivos_existentes,pokemonn,1);

	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	return path_absoluta;
}

void cerrar_archivo(char* pokemonn){

	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","N");
	config_save(pokemon_config);
	config_destroy(pokemon_config);

	dictionary_put(archivos_existentes,pokemonn,0);
}

char*formar_archivo_pokemon(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	return path_archivo_pokemon;
}

