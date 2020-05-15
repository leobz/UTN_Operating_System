#include "main.h"
t_log* logger;
t_team_config* team_config;

void imprimir_diccionario(t_dictionary* diccionario) {
	void imprimir_clave_valor(char* key, int value) {
		printf("	%s -> %d\n", key, value);
	}

	dictionary_iterator(diccionario, imprimir_clave_valor);
}

void imprimir_posicion(t_posicion* posicion){
	printf("	(%d, %d)\n", posicion->x, posicion->y);

}

void imprimir_posiciones(t_list* posiciones) {
	t_posicion* posicion = list_get(posiciones, 0);
	//TODO: sustiturir por ASSERTS
	imprimir_posicion(posicion);
}


void imprimir_objetivo_global() {

//	void imprimirObjetivoGlobal(char* pokemon, int cantidad) {
//		printf("%s: %d\n", pokemon, cantidad);
//	}

}

void mostrar_lista_entrenadores(t_team_config* team_config) {
	for (int i = 0; i < team_config->cantidad_entrenadores; i++) {
		t_tcb_entrenador* entrenador = list_get(entrenadores, i);

		printf("\nEntrenador %d\n", i);
		printf("  Posicion:\n");
		imprimir_posicion(entrenador->posicion);

		printf("  Objetivos:\n");
		imprimir_diccionario(entrenador->objetivos);

		printf("  Capturados:\n");
		imprimir_diccionario(entrenador->pokemones_capturados);

	}

}
//void mostrar(void *elemento) {
//	printf("El elemento: %s\n", (char *) elemento);
//}


int main() {
	logger = iniciar_logger("team.log", "team", LOG_LEVEL_INFO);

	t_team_config *team_config = cargar_team_config("team.config");

	char* puerto = team_config->puerto_broker;
	char* ip = team_config->ip_broker;

	char* puerto_gameboy = "4444";

	cargar_objetivo_global(team_config);
	imprimir_objetivo_global();

	crear_tcb_entrenadores(team_config);
	crear_pokemon_requeridos();

	mostrar_lista_entrenadores(team_config);

	int socket_servidor = iniciar_servidor(ip, puerto_gameboy);

    while(1)
    	esperar_cliente(socket_servidor, &procesar_mensaje_recibido);

	destruir_objetivo_global();
	destruir_pokemon_requeridos();
	destruir_team_config(team_config);
	return 0;
}
