#include "directorios.h"

//Inicializar directorios-> t config
void inicializar_directorios(t_gamecard_config* gamecard_config){

	char* creacion_path_directorio = string_new();
	//Creo Metadata
	string_append(&creacion_path_directorio,gamecard_config->punto_montaje_tallgrass);
	string_append(&creacion_path_directorio,"/Metadata");
	mkdir(creacion_path_directorio,0777);

	//Creo Files
	creacion_path_directorio=NULL;
	creacion_path_directorio = string_new();
	string_append(&creacion_path_directorio,gamecard_config->punto_montaje_tallgrass);
	string_append(&creacion_path_directorio,"/Files");
	mkdir(creacion_path_directorio,0777);

	//Creo Blocks
	creacion_path_directorio=NULL;
	creacion_path_directorio = string_new();
	string_append(&creacion_path_directorio,gamecard_config->punto_montaje_tallgrass);
	string_append(&creacion_path_directorio,"/Blocks");
	mkdir(creacion_path_directorio,0777);
	free(creacion_path_directorio);
		creacion_path_directorio=NULL;

}
