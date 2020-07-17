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
	t_bitarray*bitmap=crear_bitmap(metadata->blocks);
	actualizar_archivo_bitmap(bitmap);

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


void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){//si el archivo ya se encontraba

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

		t_metadata_pokemon* archivo_pokemon_config=leer_metadata_pokemon(path_pokemonn);

		//agregar_posicion(mensaje_new,archivo_pokemon_config); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques

		cerrar_archivo(mensaje_new->pokemon);
	}

	else{ //si el archivo no existia
		crear_archivo_pokemon(mensaje_new);

		}
}


void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_get;
	mensaje_get=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_get->pokemon)){}

}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_catch;
	mensaje_catch=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

		if(dictionary_has_key(archivos_existentes,mensaje_catch->pokemon)){}

}


void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {

	int contador_bloques=++contador_bloques_totales;

	if(contador_bloques_totales< metadata->blocks){
		char*path_archivo_pokemon=formar_archivo_pokemon(mensaje_new->pokemon,true);
		crear_archivo_metadata(path_archivo_pokemon,contador_bloques);
		setear_bloque_ocupado(contador_bloques);
		dictionary_put(archivos_existentes,mensaje_new->pokemon,false);//indica que esta cerrado
	}
	else{
		printf("ERROR: No existen bloques disponibles para crear el archivo\n");
	}
}

bool archivo_esta_abierto(char *pokemonn){
	return dictionary_get(archivos_existentes,pokemonn);
}

char* setear_archivo_abierto(char*pokemonn){

	dictionary_put(archivos_existentes,pokemonn,true);

	char*path_pokemon=formar_archivo_pokemon(pokemonn,false);
	char*path_absoluta=crear_ruta(path_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	return path_pokemon;
}

void cerrar_archivo(char* pokemonn){

	char*path_pokemon=formar_archivo_pokemon(pokemonn,false);
	char*path_absoluta=crear_ruta(path_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","N");
	config_save(pokemon_config);
	config_destroy(pokemon_config);

	dictionary_put(archivos_existentes,pokemonn,false);
}

void crear_archivo_metadata(char *path_pokemonn,int contador_bloques){

	t_bloque* pokemon_config=crear_bloque(crear_ruta(path_pokemonn));

	config_set_value(pokemon_config, "DIRECTORY","N");
	config_set_value(pokemon_config, "SIZE","0");

	char* block = string_new();
	string_append(&block, "[");
	string_append_with_format(&block, "%s",string_itoa(contador_bloques));
	string_append(&block, "]");

	config_set_value(pokemon_config, "BLOCKS",block);
	config_set_value(pokemon_config, "OPEN","N");

	config_save(pokemon_config);

	config_destroy(pokemon_config);
}

char* formar_archivo_pokemon(char*pokemonn, bool creacion){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	if(creacion) //si quiero crear el pokemon
		mkdir(path_archivo_pokemon, 0777);
	string_append(&path_archivo_pokemon, "/Metadata.bin");
	return path_archivo_pokemon;
}

void setear_bloque_ocupado(int numero_bloque){
	t_bitarray*bitmap=leer_bitmap();
	modificar_bit(numero_bloque,true,bitmap);
	actualizar_archivo_bitmap(bitmap);
}
void modificar_bit(int bit,bool valor,t_bitarray*bitmap){

	if(valor) //si esta libre ese bloque
		bitarray_set_bit(bitmap, bit);

	else   //si esta ocupado ese bloque
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

	t_bitarray * bitmap=bitarray_create_with_mode(bitarray, bytes, LSB_FIRST);

	for(int cont=0; cont < cant_bloques; cont++){ //Limpia los bits del bitarray (Los pone en 0)
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

char*ruta_bitmap(){
		char* path_bitmap = string_new();
		string_append(&path_bitmap,	"Metadata");
		string_append(&path_bitmap, "/Bitmap.bin");
		return crear_ruta(path_bitmap);
}

