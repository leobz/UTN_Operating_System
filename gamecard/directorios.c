#include "directorios.h"

//Inicializar directorios-> t config
void inicializar_directorios(){

	//Creo Metadata
	path_directorio_metadata = string_new();
	string_append(&path_directorio_metadata,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_metadata,"/Metadata");
	mkdir(path_directorio_metadata,0777);

	//Creo Files
	path_directorio_files = string_new();
	string_append(&path_directorio_files,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_files,"/Files");
	mkdir(path_directorio_files,0777);

	//Creo Blocks
	path_directorio_blocks = string_new();
	string_append(&path_directorio_blocks,gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_blocks,"/Blocks");
	mkdir(path_directorio_blocks,0777);

	t_metadata* metadata = malloc(sizeof(t_metadata));
	metadata->block_size = 64;
	metadata->blocks = 5192;
	metadata->magic_number = "TALL_GRASS";
	t_metadata* metadata_aux = malloc(sizeof(t_metadata));

	//Creo archivo Bitmap.bin
	FILE* bitmap_file;
	char* path_bitmap = string_new();
	string_append(&path_bitmap, path_directorio_metadata);
	string_append(&path_bitmap, "/Bitmap.bin");
	bitmap_file = fopen(path_bitmap, "wb");
	free(path_bitmap);
	fclose(bitmap_file);

	//Creo archivo Metadata.bin
	FILE* metadata_file;
	char* path_metadata = string_new();
	string_append(&path_metadata, path_directorio_metadata);
	string_append(&path_metadata, "/Metadata.bin");
	metadata_file = fopen(path_metadata, "wb");

	char* metada_chars = string_new();
	string_append(&metada_chars, "BLOCK_SIZE=");
	string_append_with_format(&metada_chars, "%d\n", metadata->block_size);
	string_append(&metada_chars, "BLOCKS=");
	string_append_with_format(&metada_chars, "%d\n", metadata->blocks);
	string_append(&metada_chars, "MAGIC_NUMBER=");
	string_append_with_format(&metada_chars, "%s\n", metadata->magic_number);
	fputs(metada_chars, metadata_file);

	free(metada_chars);
	free(metada_chars);
	free(metadata);
	free(metadata_aux);
	free(path_metadata);
	fclose(metadata_file);

	liberar_paths();

}

void liberar_paths(){
	free(path_directorio_metadata);
	free(path_directorio_files);
	free(path_directorio_blocks);
}
