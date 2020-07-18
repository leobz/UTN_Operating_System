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

	metadata = malloc(sizeof(t_metadata));
	metadata->block_size = 64;
	metadata->blocks = 5192;
	metadata->magic_number = "TALL_GRASS";

	//Creo archivo Bitmap.bin
	t_bitarray*bitmap = crear_bitmap(metadata->blocks);
	actualizar_archivo_bitmap(bitmap);

	//Creo archivo Metadata.bin
	t_bloque* bloque_metadata_bin = crear_bloque(
			crear_ruta("Metadata/Metadata.bin"));
	config_set_value(bloque_metadata_bin, "BLOCK_SIZE",
			string_itoa(metadata->block_size));
	config_set_value(bloque_metadata_bin, "BLOCKS",
			string_itoa(metadata->blocks));
	config_set_value(bloque_metadata_bin, "MAGIC_NUMBER",
			metadata->magic_number);

	config_save(bloque_metadata_bin);

	//free(metadata);
	//free(metadata_aux);

	//liberar_paths();

}
void inicializar_diccionarios() {
	archivos_existentes = dictionary_create();
	//archivos_abiertos=dictionary_create();
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new = deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if (dictionary_has_key(archivos_existentes, mensaje_new->pokemon)) {//si el archivo ya se encontraba

		bool abierto = archivo_esta_abierto(mensaje_new->pokemon);

		if (abierto == true) { //sie el archivo esta abierto

			pthread_mutex_lock(&mutex_abiertos[NEW_POKEMON]);

			while (abierto == true) {

				sleep(gamecard_config->tiempo_reintento_operacion);
				abierto = archivo_esta_abierto(mensaje_new->pokemon);
			}
			abierto = true;
			setear_archivo_abierto(mensaje_new->pokemon);
			pthread_mutex_unlock(&mutex_abiertos[NEW_POKEMON]);

		} else { //si el archivo esta cerrado

			pthread_mutex_lock(&mutex_setear[NEW_POKEMON]);
			setear_archivo_abierto(mensaje_new->pokemon);
			pthread_mutex_unlock(&mutex_setear[NEW_POKEMON]);
		}
	}

	else
		//si el archivo no existia
		crear_archivo_pokemon(mensaje_new);

	agregar_posicion(mensaje_new); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques
	//t_mensaje_appeared*appeared=obtener_mensaje_appeared(mensaje_new);
	//enviar_mensaje_appeared(appeared);
	sleep(gamecard_config->tiempo_retardo_operacion);
	cerrar_archivo(mensaje_new->pokemon);
}

void enviar_mensaje_localized(t_dictionary* posiciones) {
	int conexion = crear_conexion(gamecard_config->ip_broker,
			gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker")
	} else {
		int bytes;
		// TODO
		// void* a_enviar = serializar_localized_pokemon(&bytes, );
		enviar_mensaje(conexion, a_enviar, bytes);
	}
}

void procesar_get_pokemon(t_paquete_socket* paquete_socket) {
	t_mensaje_get* mensaje_get;
	mensaje_get = deserializar_mensaje_get_pokemon(paquete_socket->buffer);
	//if(dictionary_has_key(archivos_existentes,mensaje_get->pokemon)){}
	if (esta_en_diccionario(archivos_existentes, mensaje_get->pokemon)) {
		char* path_pokemon;
		bool abierto = archivo_esta_abierto(mensaje_get->pokemon);
		if (abierto) {
			pthread_mutex_lock(&mutex_abiertos[GET_POKEMON]);
			while (abierto) {
				sleep(gamecard_config->tiempo_reintento_conexion);
				abierto = archivo_esta_abierto(mensaje_get->pokemon);
			}
			path_pokemon = setear_archivo_abierto(mensaje_get->pokemon);
			pthread_mutex_unlock(&mutex_abiertos[GET_POKEMON]);
		} else {
			// TODO: codigo de leo
			// Si obtengo una lista
			t_list posiciones;
			if (!list_is_empty(posiciones)) {
				list_iterate(posiciones, enviar_mensaje_localized);
			}
		}
	} else {
		// TODO: envio de mensaje
	}
}

void enviar_mensaje_caught(t_paquete_socket* paquete_socket, int estado) {
	int conexion = crear_conexion(gamecard_config->ip_broker,
			gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker")
	} else {
		int bytes;
		void* a_enviar = serializar_caught_pokemon(&bytes, estado,
				paquete_socket->id_correlativo, paquete_socket->id_mensaje);
		enviar_mensaje(conexion, a_enviar, bytes);
	}
}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket) {
	t_mensaje_catch* mensaje_catch;
	mensaje_catch = deserializar_paquete_catch_pokemon(paquete_socket->buffer);
	if (esta_en_diccionario(archivos_existentes, mensaje_catch->pokemon)) {
		char* path_pokemon;
		bool abierto = archivo_esta_abierto(mensaje_catch->pokemon);
		if (abierto) {
			pthread_mutex_lock(&mutex_abiertos[CATCH_POKEMON]);
			while (abierto) {
				sleep(gamecard_config->tiempo_reintento_conexion);
				abierto = archivo_esta_abierto(mensaje_catch->pokemon);
			}
			path_pokemon = setear_archivo_abierto(mensaje_catch->pokemon);
			pthread_mutex_unlock(&mutex_abiertos[CATCH_POKEMON]);
		} else {
			// TODO: codigo de leo
		}
	} else {
		enviar_mensaje_caught(paquete_socket, FAIL);
	}
}

void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {

	int contador_bloques = ++contador_bloques_totales;

	if (contador_bloques_totales < metadata->blocks) {
		char*path_archivo_pokemon = formar_archivo_pokemon(mensaje_new->pokemon,
				true);
		crear_archivo_metadata(path_archivo_pokemon, contador_bloques);
		setear_bloque_ocupado(contador_bloques);
		dictionary_put(archivos_existentes, mensaje_new->pokemon, false);//indica que esta cerrado
	} else {
		printf("ERROR: No existen bloques disponibles para crear el archivo\n");
	}
}

bool archivo_esta_abierto(char *pokemonn) {
	return dictionary_get(archivos_existentes, pokemonn);
}

char* setear_archivo_abierto(char*pokemonn) {

	dictionary_put(archivos_existentes, pokemonn, true);

	char*path_pokemon = formar_archivo_pokemon(pokemonn, false);
	char*path_absoluta = crear_ruta(path_pokemon);
	t_config*pokemon_config = config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN", "Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	return path_pokemon;
}

void cerrar_archivo(char* pokemonn) {

	char*path_pokemon = formar_archivo_pokemon(pokemonn, false);
	char*path_absoluta = crear_ruta(path_pokemon);
	t_config*pokemon_config = config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN", "N");
	config_save(pokemon_config);
	config_destroy(pokemon_config);

	dictionary_put(archivos_existentes, pokemonn, false);
}

void crear_archivo_metadata(char *path_pokemonn, int contador_bloques) {

	t_bloque* pokemon_config = crear_bloque(crear_ruta(path_pokemonn));

	config_set_value(pokemon_config, "DIRECTORY", "N");
	config_set_value(pokemon_config, "SIZE", "0");

	char* block = string_new();
	string_append(&block, "[");
	string_append_with_format(&block, "%s", string_itoa(contador_bloques));
	string_append(&block, "]");

	config_set_value(pokemon_config, "BLOCKS", block);
	config_set_value(pokemon_config, "OPEN", "N");

	config_save(pokemon_config);

	config_destroy(pokemon_config);
}

char* formar_archivo_pokemon(char*pokemonn, bool creacion) {
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s", pokemonn);
	if (creacion) //si quiero crear el pokemon
		mkdir(path_archivo_pokemon, 0777);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	return path_archivo_pokemon;
}

void setear_bloque_ocupado(int numero_bloque) {
	t_bitarray*bitmap = leer_bitmap();
	modificar_bit(numero_bloque, true, bitmap);
	actualizar_archivo_bitmap(bitmap);
}
void modificar_bit(int bit, bool valor, t_bitarray*bitmap) {

	if (valor) //si esta libre ese bloque
		bitarray_set_bit(bitmap, bit);

	else
		//si esta ocupado ese bloque
		bitarray_clean_bit(bitmap, bit);
}

void actualizar_archivo_bitmap(t_bitarray*bitmap) {
	char *ruta = ruta_bitmap();

	FILE *bitmap_file = fopen(ruta, "wb");
	free(ruta);

	if (bitmap_file == NULL) {
		printf("No se pudo actualizar el archivo Bitmap.bin");
	}

	fwrite(bitmap->bitarray, sizeof(char), bitmap->size, bitmap_file);

	fclose(bitmap_file);
	bitarray_destroy(bitmap);
}

/*
 t_bitarray * crear_bitmap(int cant_bloques){

 int tamanio_bitarray=cant_bloques/8;
 if(cant_bloques % 8 != 0){
 tamanio_bitarray++;
 }

 char* bits=malloc(tamanio_bitarray);

 t_bitarray * bitarray = bitarray_create_with_mode(bits,tamanio_bitarray,LSB_FIRST);


 for(int cont=0; cont < tamanio_bitarray*8; cont++){ //Limpia los bits del bitarray (Los pone en 0)
 bitarray_clean_bit(bitarray, cont);
 }

 return bitarray;
 }*/
t_bitarray *crear_bitmap(int cant_bloques) {

	size_t bytes = BIT_SIZE(cant_bloques, CHAR_BIT);

	//Duda sizeof(char)
	char *bitarray = calloc(bytes, sizeof(char));

	t_bitarray * bitmap = bitarray_create_with_mode(bitarray, bytes, LSB_FIRST);

	for (int cont = 0; cont < cant_bloques; cont++) { //Limpia los bits del bitarray (Los pone en 0)
		bitarray_clean_bit(bitmap, cont);
	}

	return bitmap;
}

t_bitarray *leer_bitmap() {
	char *ruta = ruta_bitmap();

	FILE *bitmap_file = fopen(ruta, "rb");
	free(ruta);

	size_t bitarray_size = BIT_SIZE(metadata->blocks, CHAR_BIT); // CHAR_BIT = cantidad bits x char

	char *bitarray = malloc(bitarray_size);

	//creo que seria sizeof(char) en vez de 1
	size_t read_bytes = fread(bitarray, 1, bitarray_size, bitmap_file);

	if (read_bytes != bitarray_size) {
		fclose(bitmap_file);
		free(bitarray);
		printf("El Bitmap esta incompleto\n");
		return NULL;
	}

	fclose(bitmap_file);

	return bitarray_create_with_mode(bitarray, bitarray_size, LSB_FIRST);
}

char*ruta_bitmap() {
	char* path_bitmap = string_new();
	string_append(&path_bitmap, "Metadata");
	string_append(&path_bitmap, "/Bitmap.bin");
	return crear_ruta(path_bitmap);
}

void agregar_posicion(t_mensaje_new*mensaje_new) {
	t_config* archivo_pokemon_config = leer_config_pokemon(
			mensaje_new->pokemon);

	char*posx = string_itoa(mensaje_new->posx);
	char*posy = string_itoa(mensaje_new->posy);

	char* posicion_pokemonn = string_new();
	string_append_with_format(&posicion_pokemonn, "%s", posx);
	string_append(&posicion_pokemonn, "-");
	string_append_with_format(&posicion_pokemonn, "%s", posy);

	if (config_has_property(archivo_pokemon_config, posicion_pokemonn)) { //si esa posicion ya estaba en el archivo
		int cantidad_pokemon = config_get_int_value(archivo_pokemon_config,
				posicion_pokemonn);
		cantidad_pokemon += mensaje_new->cantidad;
		config_set_value(archivo_pokemon_config, posicion_pokemonn,
				string_itoa(cantidad_pokemon));
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,
				mensaje_new->pokemon);
	} else { //si es una nueva posicion
		config_set_value(archivo_pokemon_config, posicion_pokemonn,
				string_itoa(mensaje_new->cantidad));
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,
				mensaje_new->pokemon);
	}
}

