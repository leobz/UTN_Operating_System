#include "main.h"


int main(){
	inicializar_gamecard();
	inicializar_directorios();
	pokemones_gamecard = dictionary_create();
	pthread_create(&suscripcion_broker, NULL, (void*)suscribirme_al_broker, NULL);


	t_archivo* archivo = leer_archivo("Files/Pikachu/Metadata.bin");

	char* buffer_archivo_pikachu = buffer_archivo(archivo);
	printf("Archivo unido:\n%s\n", buffer_archivo_pikachu);


	char* buffer_leido = archivo_a_string("bloque.bin");
	t_config* un_bloque = cargar_bloque_desde_buffer(buffer_leido);



	printf("MAGIC NUMBER LEIDO DESDE BUFFER : %s \n", config_get_string_value(un_bloque,"MAGIC_NUMBER"));



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
