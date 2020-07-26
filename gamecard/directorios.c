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
	t_bloque* bloque_metadata_bin = crear_bloque(crear_ruta("Metadata/Metadata.bin"));
	config_set_value(bloque_metadata_bin, "BLOCK_SIZE",string_itoa(metadata->block_size));
	config_set_value(bloque_metadata_bin, "BLOCKS",string_itoa(metadata->blocks));
	config_set_value(bloque_metadata_bin, "MAGIC_NUMBER",metadata->magic_number);

	config_save(bloque_metadata_bin);

	//free(metadata);
	//free(metadata_aux);

	//liberar_paths();

}


void enviar_mensaje_appeared(t_paquete_socket* paquete_socket, t_mensaje_new* mensaje_new){
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker");
	}
	else {
		int bytes=0;
		void* a_enviar = serializar_appeared_pokemon(&bytes, mensaje_new->pokemon,mensaje_new->posx, mensaje_new->posy,0, paquete_socket->id_mensaje);
		t_mensaje_appeared* mensaje_appeared_recibido=deserializar_paquete_appeared_pokemon(a_enviar);

		//printf("Mensaje recibido APPEARED_POKEMON %s %d %d\n",mensaje_appeared_recibido->pokemon,mensaje_appeared_recibido->posx,mensaje_appeared_recibido->posy);

		printf("Enviando mensaje %s \n",op_code_to_string(APPEARED_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);


	}
	free(mensaje_new->pokemon);
	free(mensaje_new);
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new=deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	//log_info(logger,"Mensaje recibido NEW_POKEMON %s %d %d %d",mensaje_new->pokemon,mensaje_new->posx,mensaje_new->posy,mensaje_new->cantidad);

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){//si el archivo ya se encontraba
		checkear_archivo_abierto(mensaje_new->pokemon);
	}

	else//si el archivo no existia
		crear_archivo_pokemon(mensaje_new);

	agregar_posicion(mensaje_new); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques

	sleep(gamecard_config->tiempo_retardo_operacion);
	cerrar_archivo(mensaje_new->pokemon);

	enviar_mensaje_appeared(paquete_socket, mensaje_new);

}

void enviar_mensaje_localized(void* a_enviar,int bytes){
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker");
	}
	else {

		printf("Enviando mensaje %s\n",op_code_to_string(LOCALIZED_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);
	}
}

void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_get* mensaje_get;
	mensaje_get=deserializar_mensaje_get_pokemon(paquete_socket->buffer);

	if(dictionary_has_key(archivos_existentes,mensaje_get->pokemon)){

		checkear_archivo_abierto(mensaje_get->pokemon);

		t_posiciones* posiciones_pokemon=obtener_posiciones_pokemon(mensaje_get->pokemon);
		cerrar_archivo(mensaje_get->pokemon);


		int bytes=0;
		int cantidad_de_posiciones=dictionary_get(cantidad_posiciones_pokemon,mensaje_get->pokemon);

		void*localized_package=serializar_localized_pokemon(&bytes, mensaje_get->pokemon,cantidad_de_posiciones,posiciones_pokemon,0, paquete_socket->id_mensaje);

		enviar_mensaje_localized(localized_package,bytes);
	}
	else{
		printf("No se encontro el pokemon :%s\n",mensaje_get->pokemon);
		log_info(logger,"No se encontro el pokemon :%s",mensaje_get->pokemon);}
}

void enviar_mensaje_caught(t_paquete_socket* paquete_socket, int estado) {
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_info(logger, "[ERROR][BROKER] Error de conexion con el broker");
	}
	else {
		int bytes;
		void* a_enviar = serializar_caught_pokemon(&bytes, estado,paquete_socket->id_correlativo, paquete_socket->id_mensaje);
		printf("Enviando mensaje %s \n",op_code_to_string(CAUGHT_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);
	}
}


void procesar_catch_pokemon(t_paquete_socket* paquete_socket) {
	t_mensaje_catch* mensaje_catch;
	mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);
	if(dictionary_has_key(archivos_existentes,mensaje_catch->pokemon)){//si el archivo ya se encontraba
		checkear_archivo_abierto(mensaje_catch->pokemon);
		int resultado = decrementar_cantidad(mensaje_catch);
		cerrar_archivo(mensaje_catch->pokemon);
		enviar_mensaje_caught(paquete_socket, resultado);
	}
	else{
		printf("No se encontro el pokemon :%s\n",mensaje_catch->pokemon);
		log_info(logger,"No se encontro el pokemon :%s",mensaje_catch->pokemon);
	}
}

void crear_metadata_para_directorios(char*ruta_directorio){
	string_append(&ruta_directorio, "/Metadata.bin");
	t_bloque* pokemon_config=crear_bloque(ruta_directorio);
	config_set_value(pokemon_config, "DIRECTORY","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
}
void crear_diccionario_semaforo(char*pokemonn){
	pthread_mutex_t pokemon_sem;
	dictionary_put(pokemon_semaphores,pokemonn,&pokemon_sem);
}

void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {
	int bloque_disponible=bloque_disponible_en_bitmap();
	if(bloque_disponible!=-1){
		char*path_archivo_pokemon=crear_pokemon_metadata(mensaje_new->pokemon);
		crear_archivo_metadata(path_archivo_pokemon,bloque_disponible);
		crear_diccionario_semaforo(mensaje_new->pokemon);
		setear_bloque_ocupado(bloque_disponible);
		dictionary_put(cantidad_posiciones_pokemon,mensaje_new->pokemon,0);
		dictionary_put(archivos_existentes,mensaje_new->pokemon,true);//indica que esta abierto
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


void checkear_archivo_abierto(char*pokemonn){
	bool abierto=archivo_esta_abierto(pokemonn);

	pthread_mutex_t *pokemon_sem;
	pokemon_sem=dictionary_get(pokemon_semaphores,pokemonn);

	if(abierto==true){ //sie el archivo esta abierto

		//ESTO ES LO HAY Q VER ESTE SEMAFORO
		//pthread_mutex_lock(pokemon_sem);

		while(abierto==true){

			sleep(gamecard_config->tiempo_reintento_operacion);
			abierto=archivo_esta_abierto(pokemonn);
		}
		abierto=true;
		setear_archivo_abierto(pokemonn);

		//pthread_mutex_unlock(pokemon_sem);

	}
	else{ //si el archivo esta cerrado


		//Y ESTE
		//pthread_mutex_lock(pokemon_sem);
			setear_archivo_abierto(pokemonn);
		//pthread_mutex_unlock(pokemon_sem);
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
		cant_posiciones+=1;
		dictionary_put(cantidad_posiciones_pokemon,mensaje_new->pokemon,cant_posiciones);
		printf("Cant Posiciones: %d\n",dictionary_get(cantidad_posiciones_pokemon,mensaje_new->pokemon));
		config_set_value(archivo_pokemon_config,posicion_pokemonn,string_itoa(mensaje_new->cantidad));
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,mensaje_new->pokemon);
	}
}

t_posiciones*obtener_posiciones_pokemon(char*pokemonn){
	printf("En obtencion\n");
	int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,pokemonn);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemonn));
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

int decrementar_cantidad(t_mensaje_catch* mensaje_catch) {
	int resultado = FAIL;
	t_config* archivo_pokemon_config = leer_config_pokemon(mensaje_catch->pokemon);

	char* posx = string_itoa(mensaje_catch->posx);
	char* posy = string_itoa(mensaje_catch->posy);
	char* posicion_pokemon = string_new();
	string_append_with_format(&posicion_pokemon, "%s", posx);
	string_append_with_format(&posicion_pokemon, "-%s", posy);


	if (config_has_property(archivo_pokemon_config, posicion_pokemon)) {

		int cantidad_pokemon = config_get_int_value(archivo_pokemon_config, posicion_pokemon);
		if (cantidad_pokemon <= 1) {
			int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,mensaje_catch->pokemon);
			dictionary_put(cantidad_posiciones_pokemon,mensaje_catch->pokemon,cant_posiciones-1);
			config_remove_key(archivo_pokemon_config, posicion_pokemon);
		}
		else {
			cantidad_pokemon-=1;
			config_set_value(archivo_pokemon_config, posicion_pokemon, string_itoa(cantidad_pokemon));
		}
		guardar_config_en_archivo_pokemon(archivo_pokemon_config, mensaje_catch->pokemon);
		resultado = OK;
	}
	return resultado;
}

