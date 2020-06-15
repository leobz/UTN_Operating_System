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
	creacion_path_directorio=NULL;

	//Creo Blocks
	creacion_path_directorio = string_new();
	string_append(&creacion_path_directorio,gamecard_config->punto_montaje_tallgrass);
	string_append(&creacion_path_directorio,"/Blocks");
	mkdir(creacion_path_directorio,0777);
	creacion_path_directorio=NULL;
	free(creacion_path_directorio);

	//Creo archivo Bitmap.bin
		FILE *bitmap;
		bitmap=fopen("/home/utnso/tall-grass/Metadata/Bitmap.bin","wb");
		fclose(bitmap);

	//Creo archivo Metadata.bin

		t_file_metadata* metadata;
		metadata{
					block_size=64;
					blocks=5192;
					magic_number="TALL_GRASS";
				}
		FILE *file=fopen("/home/utnso/tall-grass/Metadata/Metadata.bin","wb");
		fwrite(&metadata->block_size,sizeof(int),1,file);
		fwrite(&metadata->blocks,sizeof(metadata->blocks),1,file);
		fwrite(&metadata->magic_number,sizeof(metadata->magic_number),1,file);
		fclose(file);
		free(metadata);

		//EXISTENCIA POKEMON-> NO CREAR CARPETA CREAR CARPETA
		//
}


