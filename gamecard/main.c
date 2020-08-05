#include "main.h"

int main(){

	inicializar_gamecard();
	inicializar_directorios();
	inicializar_diccionarios();
	inicializar_semaforos();

	int id_proceso = gamecard_config->id_proceso;

	t_datos_suscripcion* datos_suscripcion_get = crear_datos_suscripcion(id_proceso, GET_POKEMON);
	t_datos_suscripcion* datos_suscripcion_new = crear_datos_suscripcion(id_proceso, NEW_POKEMON);
	t_datos_suscripcion* datos_suscripcion_catch = crear_datos_suscripcion(id_proceso, CATCH_POKEMON);

	iniciar_hilo_suscripcion(datos_suscripcion_get);
	iniciar_hilo_suscripcion(datos_suscripcion_new);
	iniciar_hilo_suscripcion(datos_suscripcion_catch);



	// Creo la conexion para escuchar
	char* ip = gamecard_config->ip_gamecard;
	char* puerto = gamecard_config->puerto_gamecard;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1){
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido_cliente);
	}


	finalizar_gamecard();
}
