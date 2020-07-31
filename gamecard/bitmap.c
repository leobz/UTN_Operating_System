/*
 * bitmap.c
 *
 *  Created on: Jul 22, 2020
 *      Author: diego
 */

#include "bitmap.h"


char* crear_ruta(char* ruta) {
	char* path_ruta_absoluta = string_new();
	string_append(&path_ruta_absoluta, gamecard_config->punto_montaje_tallgrass);
	string_append(&path_ruta_absoluta, "/");
	string_append(&path_ruta_absoluta, ruta);

	return path_ruta_absoluta;
}

char*ruta_bitmap(){
	char* path_bitmap = string_new();
	string_append(&path_bitmap,	"Metadata");
	string_append(&path_bitmap, "/Bitmap.bin");
	char* ruta = crear_ruta(path_bitmap);
	free(path_bitmap);
	return ruta;
}

void modificar_bit(t_bitarray*bitmap,bool valor,int bit){

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
	free(bitmap->bitarray);
	free(bitmap);
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
	//printf("Tamanio bitmap :%d\n",bytes);

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

void setear_bloque_ocupado(int numero_bloque){
	t_bitarray*bitmap=leer_bitmap();
	modificar_bit(bitmap,true,numero_bloque);
	actualizar_archivo_bitmap(bitmap);
}

void setear_bloque_libre(int numero_bloque){
	pthread_mutex_lock(&mutex_bitmap);
	t_bitarray*bitmap=leer_bitmap();
	modificar_bit(bitmap,false,numero_bloque);
	actualizar_archivo_bitmap(bitmap);
	pthread_mutex_unlock(&mutex_bitmap);

}

int bloque_disponible_en_bitmap(){
	int indice=-1;
	bool bit=true;

	t_bitarray*bitmap=leer_bitmap();
	int max_bit=bitarray_get_max_bit(bitmap);
	while ((bit==true)&&(indice<max_bit)){
		indice++;
		bit=bitarray_test_bit(bitmap, indice);
	}
	actualizar_archivo_bitmap(bitmap);

	if(bit==true)
		return -1;

	return indice;
}
