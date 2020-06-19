#include "directorios.h"

typedef struct Metadata{
	int block_size;
	int blocks;
	char* magic_number;
}Metadata;

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

	Metadata *metadata=malloc(sizeof(Metadata));
	metadata->block_size=64;
	metadata->blocks=5192;
	metadata->magic_number="TALL_GRASS";

	FILE *file=fopen("/home/utnso/tall-grass/Metadata/Metadata","wb");
	if(file!=NULL){
		fwrite(metadata,sizeof(Metadata),1,file);
		fclose(file);
	}
	FILE *file2=fopen("/home/utnso/tall-grass/Metadata/Metadata","rb");
	if(file2!=NULL){
		fread(metadata,sizeof(Metadata),1,file);
		fclose(file);
	}
	printf("%d %d %s",metadata->block_size,metadata->blocks,metadata->magic_number);

}
