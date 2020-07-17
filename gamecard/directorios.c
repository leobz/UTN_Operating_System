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

		dictionary_put(archivos_existentes,mensaje_new->pokemon,false);//indica que esta cerrado

		destruir_config(pokemon_config);
	}
	else{
		printf("ERROR: No existen bloques disponibles para crear el archivo\n");
	}
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){

		char*path_pokemonn;
		bool abierto=archivo_esta_abierto(mensaje_new->pokemon);

		if(abierto==true){ //sie el archivo esta abierto

			pthread_mutex_lock(&mutex_abiertos[NEW_POKEMON]);

			while(abierto==true){

				sleep(gamecard_config->tiempo_reintento_operacion);
				abierto=archivo_esta_abierto(mensaje_new->pokemon);
			}
			abierto=true;
			path_pokemonn=setear_archivo_abierto(mensaje_new->pokemon);
			pthread_mutex_unlock(&mutex_abiertos[NEW_POKEMON]);

		}
		else{ //si el archivo esta cerrado

			pthread_mutex_lock(&mutex_setear[NEW_POKEMON]);
				path_pokemonn=setear_archivo_abierto(mensaje_new->pokemon);
			pthread_mutex_unlock(&mutex_setear[NEW_POKEMON]);
		}

		t_archivo* archivo_pokemon_config=leer_archivo_de_datos(path_pokemonn);

		//agregar_posicion(mensaje_new,archivo_pokemon_config); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques

		cerrar_archivo(mensaje_new->pokemon);
	}

	else{
		crear_archivo_pokemon(mensaje_new);
		}
}

void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_get;
	mensaje_get=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(esta_en_diccionario(archivos_existentes,mensaje_get->pokemon)){}

void enviar_mensaje_caught(t_paquete_socket* paquete_socket, int estado){
	int conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
	if (conexion == -1){
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker")
	}
	else {
		int bytes;
		void* a_enviar = serializar_caught_pokemon(&bytes, estado, paquete_socket->id_correlativo, paquete_socket->id_mensaje);
		enviar_mensaje(conexion, a_enviar, bytes);
	}
}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_catch* mensaje_catch;
	mensaje_catch = deserializar_paquete_catch_pokemon(paquete_socket->buffer);
	if(esta_en_diccionario(archivos_existentes, mensaje_catch->pokemon)){
		char* path_pokemon;
		bool abierto = archivo_esta_abierto(mensaje_catch->pokemon);
		if(abierto){
			pthread_mutex_lock(&mutex_abiertos[CATCH_POKEMON]);
			while(abierto){
				sleep(gamecard_config->tiempo_reintento_conexion);
				abierto = archivo_esta_abierto(mensaje_catch->pokemon);
			}
			path_pokemon = setear_archivo_abierto(mensaje_catch->pokemon);
			pthread_mutex_unlock(&mutex_abiertos[CATCH_POKEMON]);
		}
		else{
			// TODO: codigo de leo
		}
	}
	else {
		enviar_mensaje_caught(paquete_socket, FAIL);
	}
}

bool archivo_esta_abierto(char *pokemonn){
	return dictionary_get(archivos_existentes,pokemonn);
}

char* setear_archivo_abierto(char*pokemonn){

	dictionary_put(archivos_existentes,pokemonn,true);

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

	dictionary_put(archivos_existentes,pokemonn,false);
}

char*formar_archivo_pokemon(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	return path_archivo_pokemon;
}

