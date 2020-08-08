#ifndef BLOQUES_H_
#define BLOQUES_H_

#include "bitmap.h"

typedef struct{
	int block_size;
	t_list* blocks;
	char* magic_number;
}t_file_metadata;


typedef struct{
	t_list* blocks;
	int size;
	char* directory;
	bool open;
}t_metadata_pokemon;


typedef t_config t_bloque;

t_dictionary* cantidad_posiciones_pokemon;

t_config* leer_config_pokemon(char* pokemon);
void guardar_config_en_archivo_pokemon(t_config* config_pokemon, char *pokemon);


/**
* @NAME: ruta_blocks(char* numero_de_bloque)
* @DESC: Retorna la ruta absoluta de un numero de block
* Ejemplo: ruta_blocks("1") => "home/../punto-de-montaje/Blocks/1.bin"
*/
char* ruta_blocks(char* numero_de_bloque);


/**
* @NAME: crear_bloque(char* ruta_del_archivo)
* @DESC: Abre un archivo de texto de tipo bloque, lee todas sus keys
* y las parsea a un formato t_bloque(t_config). Si no existe, lo crea
*/
t_bloque* crear_bloque(char* ruta_archivo);

/**
* @NAME: buffer_
* del_archivo_completo(t_archivo* archivo)
* @DESC: Dado un t_archivo, busca el contenido de cada bloque y los une en un solo char*
* Ejemplo: {1.bin="hola", 2.bin=" chau"} ==> "hola chau"
*/
char* buffer_del_archivo_completo(t_metadata_pokemon* archivo);


/**
* @NAME: leer_archivo(char* ruta)
* @DESC: Crea un struct t_archivo desde un Metadata.bin de un POKEMON (No sirve para el del FS)
* Ejemplo: leer_archivo_de_datos("Files/Pikachu/Metadata.bin") => t_archivo.size = 12, t_archivo.blocks =....
*/
t_metadata_pokemon* leer_metadata_pokemon(char* ruta_al_metadata_bin_del_archivo);


/**
* @NAME: archivo_a_string(char* ruta)
* @DESC: Convierte a string un archivo
*/
char* archivo_a_string(char* ruta);

/**
* @NAME: cargar_bloque_desde_buffer(char* un_buffer);
* @DESC: Crea un t_config/t_bloque nuevo desde un char* en vez de desde un archivo
*/
t_config *cargar_config_desde_buffer(char* un_buffer);

void destruir_metadata_pokemon(t_metadata_pokemon* metadata_pokemon);
char* config_save_to_buffer(t_config *self) ;
int cantidad_de_bloques_necesarios(int size_buffer_de_guardado) ;
int escribir_buffer_en_bloque(char* buffer, char*  numero_de_bloque);
char* obtener_numero_de_bloque_disponible();
int escribir_archivo(t_metadata_pokemon* archivo, char* buffer_de_guardado,char*pokemon);
char*formar_archivo_pokemon(char*pokemonn);
void actualizar_vector_de_bloques_en_metadata(t_metadata_pokemon*archivo,char*pokemonn);
void actualizar_tamanio_archivo(char*buffer_pokemon,char*pokemonn);
void cerrar_bloque(char*bloque_restante);
#endif /* BLOQUES_H_ */
