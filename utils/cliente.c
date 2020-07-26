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

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)== -1) {

		printf("WARNING: Error al conectar con IP:%s Puerto:%s\n", ip, puerto);
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(int socket_cliente, void* a_enviar, int bytes) {
	send(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL);
	free(a_enviar);
}
void enviar_mensaje_nofree(int socket_cliente, void* a_enviar, int bytes) {
	send(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL);
}
int enviar_mensaje_con_retorno(int socket_cliente, void* a_enviar, int bytes){
	int verificacion=send(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL);
	return verificacion;
}

int recibir_mensaje(int socket_cliente, t_buffer *buffer_recibido) {
	//TODO: Testear este metodo
	int size_buffer = 0;
	int codigo_operacion;

	recv(socket_cliente, &(codigo_operacion), sizeof(int), 0);
	recv(socket_cliente, &(size_buffer), sizeof(int), 0);

	buffer_recibido->size = size_buffer;
	buffer_recibido->stream = malloc(size_buffer);
	recv(socket_cliente, buffer_recibido->stream, buffer_recibido->size, 0);

	return codigo_operacion;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}
