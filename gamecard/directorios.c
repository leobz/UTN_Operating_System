#include "directorios.h"

//Inicializar directorios-> t config
void inicializar_directorios() {

	//Creo Metadata

	path_directorio_metadata = string_new();
	string_append(&path_directorio_metadata,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_metadata, "/Metadata");
	mkdir(path_directorio_metadata, 0777);
	crear_metadata_para_directorios(path_directorio_metadata);

	//Creo Files
	path_directorio_files = string_new();
	string_append(&path_directorio_files,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_files, "/Files");
	mkdir(path_directorio_files, 0777);
	crear_metadata_para_directorios(path_directorio_files);

	//Creo Blocks
	//TODO: escribir archivos blocks
	path_directorio_blocks = string_new();
	string_append(&path_directorio_blocks,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_blocks, "/Blocks");
	mkdir(path_directorio_blocks, 0777);
	crear_metadata_para_directorios(path_directorio_blocks);

	metadata = malloc(sizeof(t_metadata));
	metadata->block_size = 20;
	metadata->blocks = 80;
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
void inicializar_diccionarios(){
archivos_existentes = dictionary_create();
cantidad_posiciones_pokemon=dictionary_create();
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){//si el archivo ya se encontraba
		checkear_archivo_abierto(mensaje_new->pokemon,NEW_POKEMON);
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


void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_new*mensaje_get;
	mensaje_get=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_get->pokemon)){
		checkear_archivo_abierto(mensaje_get->pokemon,GET_POKEMON);
		t_posiciones*posiciones_pokemon=obtener_posiciones_pokemon(mensaje_get->pokemon);

		int cantidad_de_posiciones=dictionary_get(cantidad_posiciones_pokemon,mensaje_get->pokemon);
		//trata a posiciones_pokemon como un vector que tendra maximo indice la cantidad_de_posiciones
		//enviar_mensaje_localized(posiciones_pokemon,cantidad_de_posiciones,paquete_socket->id_mensaje);
	}
	else
		printf("No se encontro el pokemon requerido");


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

void crear_metadata_para_directorios(char*ruta_directorio){
	string_append(&ruta_directorio, "/Metadata.bin");
	t_bloque* pokemon_config=crear_bloque(ruta_directorio);
	config_set_value(pokemon_config, "DIRECTORY","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
}

void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {

	int bloque_disponible=bloque_disponible_en_bitmap();

	if(bloque_disponible!=-1){
		char*path_archivo_pokemon=crear_pokemon_metadata(mensaje_new->pokemon);

		crear_archivo_metadata(path_archivo_pokemon,bloque_disponible);
		setear_bloque_ocupado(bloque_disponible);
		dictionary_put(cantidad_posiciones_pokemon,mensaje_new->pokemon,0);
		dictionary_put(archivos_existentes,mensaje_new->pokemon,false);//indica que esta cerrado
	}
	else{
		printf("ERROR: No existen bloques disponibles para crear el archivo\n");
		exit(1);
	}
}


void crear_archivo_metadata(char *path_pokemonn, int contador_bloques) {

	t_bloque* pokemon_config=crear_bloque(path_pokemonn);

	//crear_bloque_vacio(contador_bloques);
	config_set_value(pokemon_config, "DIRECTORY","N");
	config_set_value(pokemon_config, "SIZE","0");

	char* block = string_new();
	string_append(&block, "[");
	string_append_with_format(&block, "%s", string_itoa(contador_bloques));
	string_append(&block, "]");

	config_set_value(pokemon_config, "BLOCKS", block);
	config_set_value(pokemon_config, "OPEN", "N");

	config_save(pokemon_config);

	config_destroy(pokemon_config);
}


char* crear_pokemon_metadata(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	char*path_completo=crear_ruta(path_archivo_pokemon);
	mkdir(path_completo, 0777);
	string_append(&path_completo, "/Metadata.txt");

	return path_completo;
}


void checkear_archivo_abierto(char*pokemonn,op_code cola){
	bool abierto=archivo_esta_abierto(pokemonn);

	if(abierto==true){ //sie el archivo esta abierto

		pthread_mutex_lock(&mutex_abiertos[cola]);

		while(abierto==true){

			sleep(gamecard_config->tiempo_reintento_operacion);
			abierto=archivo_esta_abierto(pokemonn);
		}
		abierto=true;
		setear_archivo_abierto(pokemonn);
		pthread_mutex_unlock(&mutex_abiertos[cola]);

	}
	else{ //si el archivo esta cerrado

		pthread_mutex_lock(&mutex_setear[cola]);
			setear_archivo_abierto(pokemonn);
		pthread_mutex_unlock(&mutex_setear[cola]);
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
	return path_pokemon;
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


void agregar_posicion(t_mensaje_new*mensaje_new){

	t_config* archivo_pokemon_config=leer_config_pokemon(mensaje_new->pokemon);

	char*posx = string_itoa(mensaje_new->posx);
	char*posy = string_itoa(mensaje_new->posy);

	char* posicion_pokemonn = string_new();
	string_append_with_format(&posicion_pokemonn, "%s",posx);
	string_append(&posicion_pokemonn,"-");
	string_append_with_format(&posicion_pokemonn, "%s",posy);

	if(config_has_property(archivo_pokemon_config, posicion_pokemonn)){//si esa posicion ya estaba en el archivo

		int cantidad_pokemon=config_get_int_value(archivo_pokemon_config,posicion_pokemonn);
		cantidad_pokemon+=mensaje_new->cantidad;
		config_set_value(archivo_pokemon_config,posicion_pokemonn,string_itoa(cantidad_pokemon));
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,mensaje_new->pokemon);
	}
	else{ //si es una nueva posicion

		int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,mensaje_new->pokemon);
		dictionary_put(cantidad_posiciones_pokemon,mensaje_new->pokemon,cant_posiciones+1);
		config_set_value(archivo_pokemon_config,posicion_pokemonn,string_itoa(mensaje_new->cantidad));
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,mensaje_new->pokemon);
	}
}

t_posiciones*obtener_posiciones_pokemon(char*pokemonn){
	int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,pokemonn);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon("Pikachu"));
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);
	char**string_vector=string_n_split(buffer_pokemon, cant_posiciones, "\n");

	t_posiciones *posiciones_pokemon;
	int iterador=0;

	void separar_y_obtener_posiciones(char*linea){
		char**vector_separado=string_split(linea,"=");
		char*posicion=vector_separado[0];
		char**posiciones_separadas=string_split(posicion,"-");
		t_posiciones estructura_posiciones;
		estructura_posiciones.posx=atoi(posiciones_separadas[0]);
		estructura_posiciones.posy=atoi(posiciones_separadas[1]);

		posiciones_pokemon[iterador]=estructura_posiciones;
		iterador++;
	}
	string_iterate_lines(string_vector, separar_y_obtener_posiciones);

	free(metadata_pokemon);
	free(buffer_pokemon);

	return posiciones_pokemon;
}
