#include "main.h"

t_datos_suscripcion* crear_datos_suscripcion(int id_proceso, int cola) {
	t_datos_suscripcion* datos_suscripcion = malloc(sizeof(t_datos_suscripcion));
	datos_suscripcion->conexion = crear_conexion(gamecard_config->ip_broker, gamecard_config->puerto_broker);
	datos_suscripcion->cola = cola;
	datos_suscripcion->id_proceso = id_proceso * 10 + datos_suscripcion->cola;
	return datos_suscripcion;
}

int main(){

	inicializar_gamecard();
	inicializar_directorios();
	inicializar_diccionarios();
	inicializar_semaforos();


	//CAMBIAR POR ARCHIVO DE CONFIGURACION

	int id_proceso = (int)getpid();

	char* puerto_broker = gamecard_config->puerto_broker;
	char* ip_broker = gamecard_config->ip_broker;


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
