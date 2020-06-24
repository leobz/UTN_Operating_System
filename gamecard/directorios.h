#ifndef DIRECTORIOS_H_
#define DIRECTORIOS_H_
#include <sys/stat.h>
#include "gamecard.h"
#include "commons/string.h"
#include "commons/collections/list.h"
//Estructuras
typedef struct{
	int block_size;
	t_list* blocks;
	char* magic_number;
}t_file_metadata;
void inicializar_directorios(t_gamecard_config* gamecard_config);
int size_metadata_blocks(t_list* lista);

#endif  //DIRECTORIOS_H_
