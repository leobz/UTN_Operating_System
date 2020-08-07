/*
 * gamecard.c
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#include "gamecard.h"

void parsear_gamecard_config(t_gamecard_config* gamecard_config, t_config *config) {
	gamecard_config->tiempo_reintento_conexion = config_get_double_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	gamecard_config->tiempo_reintento_operacion = config_get_double_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	gamecard_config->tiempo_retardo_operacion = config_get_double_value(config, "TIEMPO_RETARDO_OPERACION");
	gamecard_config->punto_montaje_tallgrass = strdup(config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS"));
	gamecard_config->ip_broker = strdup(config_get_string_value(config, "IP_BROKER"));
	gamecard_config->puerto_broker = strdup(config_get_string_value(config, "PUERTO_BROKER"));
	gamecard_config->ip_gamecard = strdup(config_get_string_value(config, "IP_GAMECARD"));
	gamecard_config->puerto_gamecard = strdup(config_get_string_value(config, "PUERTO_GAMECARD"));
	gamecard_config->id_proceso = config_get_int_value(config, "ID_PROCESO");

	gamecard_config->block_size = config_get_int_value(config, "BLOCK_SIZE");
	gamecard_config->magic_number =  strdup(config_get_string_value(config, "MAGIC_NUMBER"));
	gamecard_config->blocks = config_get_int_value(config, "BLOCKS");
}


void destruir_gamecard_config(t_gamecard_config* gamecard_config) {
	free(gamecard_config->punto_montaje_tallgrass);
	free(gamecard_config->ip_broker);
	free(gamecard_config->puerto_broker);
	free(gamecard_config->ip_gamecard);
	free(gamecard_config->puerto_gamecard);
	free(gamecard_config);
}

t_gamecard_config* cargar_gamecard_config(char* path_archivo){
	t_config* config;
	t_gamecard_config* gamecard_config;

	config = leer_config(path_archivo);
	gamecard_config = malloc(sizeof(t_gamecard_config));

	parsear_gamecard_config(gamecard_config, config);
	destruir_config(config);

	return gamecard_config;
}

void inicializar_gamecard() {
	gamecard_config = cargar_gamecard_config("gamecard.config");
	logger = iniciar_logger("gamecard.log", "gamecard", LOG_LEVEL_INFO);
	logger_debug = iniciar_logger("gamecard_debug.log", "gamecard", LOG_LEVEL_INFO);

}
void inicializar_diccionarios(){
	archivos_existentes = dictionary_create();
	cantidad_posiciones_pokemon=dictionary_create();
	cargar_pokemones_existentes();
	cargar_posiciones_existentes();
}
void cargar_pokemones_existentes(){

	char first_letter;
	DIR *dp;
	struct dirent *ep;
	char*path_files=crear_ruta("Files");

	dp = opendir (path_files);

	if (dp != NULL)
	{
	  while ((ep = readdir(dp)) != NULL){
		 first_letter=ep->d_name[0];
		  if((strcmp(ep->d_name, "Metadata.bin") != 0)&&(isalpha(first_letter)))
			  dictionary_put(archivos_existentes,ep->d_name,false);
	  }

	  (void) closedir (dp);
	}
	else
	  perror ("Couldn't open the directory");

	log_info(logger_debug,"Cantidad de pokemons: %d", dictionary_size(archivos_existentes));
	free(path_files);

}

void cargar_posiciones_existentes(){

	int cantidad_posiciones_del_pokemon=0;

	void cargar_posiciones(char*pokemon,void*value){
		t_config* archivo_pokemon_config = leer_config_pokemon(pokemon);
		cantidad_posiciones_del_pokemon=archivo_pokemon_config->properties->elements_amount;
		log_info(logger_debug,"Cantidad de posiciones de %s: %d",pokemon,cantidad_posiciones_del_pokemon);
		dictionary_put(cantidad_posiciones_pokemon,pokemon,cantidad_posiciones_del_pokemon);
		config_destroy(archivo_pokemon_config);
	}

	if((dictionary_size(archivos_existentes))!=0){
		dictionary_iterator(archivos_existentes,cargar_posiciones);
	}
}

void inicializar_semaforos() {
	pthread_mutex_init(&mutex_abrir_archivos, NULL);
	pthread_mutex_init(&mutex_bitmap, NULL);
}


void liberar_paths(){
	free(path_directorio_metadata);
	free(path_directorio_files);
	free(path_directorio_blocks);
}

void finalizar_gamecard() {
	liberar_paths();
	destruir_gamecard_config(gamecard_config);
	destruir_logger(logger);
	destruir_logger(logger_debug);
}



/////////////////////////BLOQUES/////////////////////////////////////////////////////


