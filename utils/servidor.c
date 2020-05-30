#include "servidor.h"

int iniciar_servidor(char* ip, char* puerto)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
        	printf("Error al momento de crear el socket del servidor\n");
        	continue;
        }

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
        	printf("Error al momento de asociar el socket del servidor al puerto: %s\n", puerto);
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    return socket_servidor;
}

void esperar_cliente(int socket_servidor, void(*procesar_mensaje_recibido)(t_paquete_socket*))
{
	struct sockaddr_in dir_cliente;

	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	t_paquete_socket* paquete = recibir_mensaje_servidor(socket_cliente);
	/* cambie t_paquete por t_paquete_socket pq necesito q el paquete tenga el socket
	 * del cliente y opcionalmente un idntififcador de colas en caso de ser suscripcion
	 * pq a procesar_mensaje_recibido solo se le puede pasar un argumento
	 */

	pthread_create(&thread,NULL,(void*)procesar_mensaje_recibido,paquete);
	pthread_detach(thread);
}

t_paquete_socket* recibir_mensaje_servidor(int socket_cliente) {
	t_paquete_socket* paquete = (t_paquete_socket*) malloc(sizeof(t_paquete_socket));
	int size_buffer = 0;

	paquete->socket_cliente = socket_cliente;

	if(recv(socket_cliente, &(paquete->codigo_operacion), sizeof(int), MSG_WAITALL) == -1){
		paquete->codigo_operacion= OP_ERROR;
		}

	if(paquete->codigo_operacion==SUSCRIPCION){
		recv(socket_cliente, &(paquete->cola), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(paquete->tiempo), sizeof(int), MSG_WAITALL);
	}


	if ((paquete->codigo_operacion != OP_ERROR)&&(paquete->codigo_operacion !=SUSCRIPCION)) {
		// si lo que recivo es un mensaje
		recv(socket_cliente, &(paquete->id_correlativo), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(size_buffer), sizeof(int), MSG_WAITALL);

		paquete->buffer = (t_buffer*)malloc(sizeof(t_buffer));
		paquete->buffer->size = size_buffer;
		paquete->buffer->stream = malloc(size_buffer);

		recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
	}

	return paquete;
}


