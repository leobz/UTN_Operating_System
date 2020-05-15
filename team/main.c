#include "team.h"

void imprimir_lista_de_pokemones(t_list* lista_pokemones) {
	t_dictionary* dic1 = list_get(lista_pokemones, 0);
	int cant_pikachu = dictionary_get(dic1, "Pikachu");
	int cant_pidgey = dictionary_get(dic1, "Pidgey");

	//TODO: sustituir por ASSERTS
	printf("%d\n", cant_pikachu);
	printf("%d\n", cant_pidgey);
}

void imprimir_posiciones(t_list* posiciones){
	t_posicion* posicion = list_get(posiciones, 0);
	//TODO: sustiturir por ASSERTS
	printf("(%d, %d)\n", posicion->x, posicion->y);
}

void imprimir_objetivo_global(){
	int cantidad_pikachu = dictionary_get(objetivo_global, "Pikachu");
	int cantidad_charmander = dictionary_get(objetivo_global, "Charmander");
	int cantidad_pidgey = dictionary_get(objetivo_global, "Pidgey");

	//TODO: sustituir por ASSERTS
	printf("Cantidad de Pikachus:%d\n", cantidad_pikachu);
	printf("Cantidad de Charmander:%d\n", cantidad_charmander);
	printf("Cantidad de Pidgey:%d\n", cantidad_pidgey);

}

int main() {

	t_team_config *team_config = cargar_team_config("team.config");

	imprimir_lista_de_pokemones(team_config->pokemon_entrenadores);
	imprimir_lista_de_pokemones(team_config->objetivos_entrenadores);
	imprimir_posiciones(team_config->posiciones_entrenadores);

	char* puerto = team_config->puerto_broker;
	char* ip = team_config->ip_broker;

	cargar_objetivo_global(team_config);
	imprimir_objetivo_global();
	//crear_tcb_entrenadores(t_team_config);

	//crear_pokemon_requeridos();

//	int socket_servidor = iniciar_servidor(ip, puerto);
//
//    while(1)
//    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

	destruir_objetivo_global();
	destruir_pokemon_requeridos();
	destruir_team_config(team_config);
	return 0;
}
