#include "main.h"

int main(){
	inicializar_gamecard();

	//NEW POKEMON
	//TODO: Listado de pokemons, recorrer el objetivo general para ver los distintos pokemons que hay que tener
	/*TODO:Si el pokemon no existe:
	 1) crear directorio metada.bin (ver txt.c)
	 2) Buscar bloque libre en bitmap-> recorrer buscando un 0, ese es el bloque libre --> mirar bitarray.c
	 3) completar el metadata.bin
	 */
	/*TODO: Si el pokemon existe:
	if(esta abierto) --> chequear Y/N
		Busco el tiempo en archivo de configuracion
	else
		fopen archivo y unirlos todos
	*/

	/*TODO: If(Existe la posicion){cantidad+lo que sumo}
	  	  	else{agrego una linea al final del archivo}
	  	  	 sleep()
	 	 	 fclose()
	 */
	/*TODO:
	 	 Conecto Gamecard con broker
	 	 recibo la respuesta
	 	 en caso que hubo respuesta mando caught
	 	 si no hubo respuesta logueo el error
	 */

	finalizar_gamecard();
}
