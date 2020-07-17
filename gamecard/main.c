#include "main.h"

int main(){
	inicializar_gamecard();
	inicializar_directorios();
	pthread_create(&suscripcion_broker, NULL, (void*)suscribirme_al_broker, NULL);


//  Funciones de muestra (Parte de tener un Files/Pikachu/Metadata.bin ya creado)

//	t_config* config_pikachu = leer_config_pokemon("Pikachu");
//	char* valor = config_get_string_value(config_pikachu, "1-2");

//	printf("Valor: %s\n", valor);
//	config_set_value(config_pikachu, "1-2", "ElNumeroMalo");
//	guardar_config_en_archivo_pokemon(config_pikachu, "Pikachu");


	// Creo la conexion para escuchar
	char* ip = gamecard_config->ip_gamecard;
	char* puerto = gamecard_config->puerto_gamecard;
	int socket_servidor = iniciar_servidor(ip, puerto);

	while (1){
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}

	pthread_detach(suscripcion_broker);

	finalizar_gamecard();
}
