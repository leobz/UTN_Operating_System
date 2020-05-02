#include"servidor.h"

void iniciar_servidor(char* ip, char* puerto)
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

    while(1)
    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)procesar_mensaje_recibido,&socket_cliente);
	pthread_detach(thread);
}

void procesar_mensaje_recibido(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;

//	int size;
//	void* msg;
//		switch (cod_op) {
//		case MENSAJE:
//			msg = recibir_mensaje(cliente_fd, &size);
//			devolver_mensaje(msg, size, cliente_fd);
//			free(msg);
//			break;
//		case 0:
//			pthread_exit(NULL);
//		case -1:
//			pthread_exit(NULL);
//		}
}

//void* recibir_mensaje(int socket_cliente, int* size)
//{
//	void * buffer;
//
//	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
//	buffer = malloc(*size);
//	recv(socket_cliente, buffer, *size, MSG_WAITALL);
//
//	return buffer;
//}
//
//void* serializar_paquete(t_paquete* paquete, int bytes)
//{
//	void * magic = malloc(bytes);
//	int desplazamiento = 0;
//
//	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
//	desplazamiento+= paquete->buffer->size;
//
//	return magic;
//}
//
//void devolver_mensaje(void* payload, int size, int socket_cliente)
//{
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//
//	paquete->codigo_operacion = MENSAJE;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = size;
//	paquete->buffer->stream = malloc(paquete->buffer->size);
//	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);
//
//	int bytes = paquete->buffer->size + 2*sizeof(int);
//
//	void* a_enviar = serializar_paquete(paquete, bytes);
//
//	send(socket_cliente, a_enviar, bytes, 0);
//
//	free(a_enviar);
//	free(paquete->buffer->stream);
//	free(paquete->buffer);
//	free(paquete);
//}
