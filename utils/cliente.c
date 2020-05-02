#include "cliente.h"

int crear_conexion(char *ip, char* puerto) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
			server_info->ai_socktype, server_info->ai_protocol);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)
			== -1)
		printf("Error al ejecutar la llamada al método connect()\n");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	t_buffer* buffer = malloc(sizeof(t_buffer));
	void* a_enviar;
	int mensaje_length = strlen(mensaje) + 1;

	buffer->size = sizeof(int) + mensaje_length;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &mensaje_length, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, mensaje, mensaje_length);

	buffer->stream = stream;

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = buffer;
	int bytes = buffer->size + (sizeof(int) * 2);

	a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

char* recibir_mensaje(int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	char* mensaje_recibido;
	int size_buffer = 0;

	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(int), 0);
	recv(socket_cliente, &(size_buffer), sizeof(int), 0);
	paquete->buffer->size = size_buffer;
	paquete->buffer->stream = malloc(size_buffer);
	recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, 0);

	switch (paquete->codigo_operacion) {
	case MENSAJE:
		mensaje_recibido = deserializar_buffer_de_un_string(paquete->buffer);
		break;
	default:
		break;
	}

	eliminar_paquete(paquete);

	return mensaje_recibido;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}
