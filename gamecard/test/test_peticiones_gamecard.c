/*
 * test_peticiones_gamecard.c
 *
 *  Created on: Jul 18, 2020
 *      Author: diego
 */

#include "test_peticiones_gamecard.h"
///ESTOS SON TESTS A REALIZAR



//TEST PETICION NEW
/*		t_mensaje_new*menssaje_new=malloc(sizeof(t_mensaje_new));

		menssaje_new->pokemon="Pikachu";
		menssaje_new->cantidad=10;
		menssaje_new->length_pokemon=strlen(menssaje_new->pokemon);
		menssaje_new->posx=36;
		menssaje_new->posy=4;

		crear_archivo_pokemon(menssaje_new);

		agregar_posicion(menssaje_new);

		menssaje_new->posy=7;
		menssaje_new->cantidad=9;
		agregar_posicion(menssaje_new);

		menssaje_new->posx=5;
		menssaje_new->posy=18;
		menssaje_new->cantidad=10;
		agregar_posicion(menssaje_new);

		menssaje_new->posx=5;
		menssaje_new->posy=18;
		menssaje_new->cantidad=12;
		agregar_posicion(menssaje_new);

		menssaje_new->posx=15;
		menssaje_new->posy=12;
		menssaje_new->cantidad=10;
		agregar_posicion(menssaje_new);

		menssaje_new->posy=25;
		menssaje_new->posx=12;
		menssaje_new->cantidad=10;
		agregar_posicion(menssaje_new);

		menssaje_new->posx=3;
		menssaje_new->posy=45;
		menssaje_new->cantidad=4;
		agregar_posicion(menssaje_new);

	//PERMITE VER EL FUNCIONAMIENTO DEL BITMAP HASTA ENCONTRAR EL PRIMER BLOQUE LIBRE
	int indice=-1;
	bool bit=true;

	t_bitarray*bitmap=leer_bitmap();
	int max_bit=bitarray_get_max_bit(bitmap);
	while ((bit==true)&&(indice<max_bit)){
		indice++;
		bit=bitarray_test_bit(bitmap, indice);
		printf("Bit numero: %d, valor: %d\n",indice,bit);
	}
	actualizar_archivo_bitmap(bitmap);

	setear_bloque_libre(1);

//TEST PETICION GET

/*	crear_archivo_metadata(crear_pokemon_metadata("Pikachu"),1);
	t_config* config_pikachu = leer_config_pokemon("Pikachu");
	config_set_value(config_pikachu, "1-4", "100");
	config_set_value(config_pikachu, "2-9", "23");
	config_set_value(config_pikachu, "1-3", "65");
	config_set_value(config_pikachu, "5-6", "2");
	guardar_config_en_archivo_pokemon(config_pikachu,"Pikachu");

	dictionary_put(cantidad_posiciones_pokemon,"Pikachu",4);

	t_posiciones* pos=obtener_posiciones_pokemon("Pikachu");
	printf("Llega2\n");
	printf("Posicion 2: %d\n",pos[1].posx);*/

/*	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon("Pikachu"));
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);

	char**str_vector=string_n_split(buffer_pokemon, 3, "\n");
	printf("Buffer: %s\n",buffer_pokemon);
	printf("2da posicion: %s\n",str_vector[1]);*/
	//config_set_value(config_pikachu, "2-2","Pokaho");
	//guardar_config_en_archivo_pokemon(config_pikachu, "Pikachu");
