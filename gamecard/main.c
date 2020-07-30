#include "main.h"

int main(){

	inicializar_gamecard();
	inicializar_directorios();
	inicializar_diccionarios();
	inicializar_semaforos();
	pthread_create(&suscripcion_broker, NULL, (void*)suscribirme_al_broker, NULL);

/*

	int bytes=0;
	char*pokemon="Picachu";
	int cantidad_de_posiciones=3;
	t_posiciones posi[3];
	posi[0].posx=2;
	posi[0].posy=4;
	posi[1].posx=8;
	posi[1].posy=9;
	posi[2].posx=7;
	posi[2].posy=6;


	void* loca=serializar_localized_pokemon(&bytes,pokemon,cantidad_de_posiciones,posi,0, 2);
	t_mensaje_localized*loc=deserializar_paquete_localized_pokemon(loca);

	//printf("Mensaje locacalized: %s\n",loc->pokemon);

	printf("Mensaje locacalized: %s\n",loc->pokemon);

	printf("Mensaje localized: %s %d %d %d %d %d %d %d\n",loc->pokemon,loc->cantidad_posiciones,
			loc->pos[0].posx,loc->pos[0].posy,loc->pos[1].posx,loc->pos[1].posy,loc->pos[2].posx,loc->pos[2].posy);

*/

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
