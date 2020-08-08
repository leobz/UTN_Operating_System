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
			perror("socket");
        	continue;
        }

		int activado=1;

		if (setsockopt(socket_servidor,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
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

    if (socket_cliente < 1) {
                perror("accept");
                exit(socket_cliente);
    }

	t_paquete_socket* paquete = recibir_mensajes(socket_cliente);

	pthread_t thread;
	pthread_create(&thread,NULL,(void*)procesar_mensaje_recibido,paquete);
	pthread_detach(thread);
}

t_paquete_socket* recibir_mensajes(int socket_cliente) {
	t_paquete_socket* paquete = (t_paquete_socket*) malloc(sizeof(t_paquete_socket));
	int size_buffer = 0;

	paquete->socket_cliente = socket_cliente;

	int recv_result = recv(socket_cliente, &(paquete->codigo_operacion), sizeof(int), MSG_WAITALL);

	if(recv_result == 0 || recv_result == -1){
		paquete->codigo_operacion= OP_ERROR;
	}

	if(paquete->codigo_operacion==SUSCRIPCION){
		recv(socket_cliente, &(paquete->cola), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(paquete->id_proceso), sizeof(int), MSG_WAITALL);
	}

	if(paquete->codigo_operacion==CONFIRMACION){
		recv(socket_cliente, &(paquete->id_mensaje), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(paquete->id_proceso), sizeof(int), MSG_WAITALL);
	}


	if ((paquete->codigo_operacion != OP_ERROR)&&(paquete->codigo_operacion !=SUSCRIPCION)&&(paquete->codigo_operacion !=CONFIRMACION)) {
		// si lo que recivo es un mensaje
		recv(socket_cliente, &(paquete->id_mensaje), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(paquete->id_correlativo), sizeof(int), MSG_WAITALL);
		recv(socket_cliente, &(size_buffer), sizeof(int), MSG_WAITALL);

		paquete->buffer = (t_buffer*)malloc(sizeof(t_buffer));
		paquete->buffer->size = size_buffer;
		paquete->buffer->stream = malloc(size_buffer);

		recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
	}

	return paquete;
}


