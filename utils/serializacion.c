#include "serializacion.h"

t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer) {
	t_paquete *paquete = (t_paquete*) malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	paquete->buffer = buffer;

	return paquete;
}

void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void* serializar_paquete(t_paquete* paquete, int bytes) {
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

void* deserializar_buffer_de_un_string(t_buffer* buffer) {
	// SOLO SIRVE PARA DESERIALIZAR PAQUETES QUE CONTENGAN SOLO UN STRING
	// NO SIRVE PARA DESERIALIZAR OTRO TIPO DE BUFFERS
	char* msj;
	int length = 0;

	void* stream = buffer->stream;

	memcpy(&length, stream, sizeof(int));
	stream += sizeof(int);

	msj = malloc(length);
	memcpy(msj, stream, length);

	return msj;
}

t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, int pos_x, int pos_y) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	int pokemon_lenght = strlen(pokemon) + 1;

	buffer->size = sizeof(int) * 3 + pokemon_lenght;
	buffer->stream = malloc(buffer->size);

	int offset = 0;
	memcpy(buffer->stream + offset, &pokemon_lenght, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, pokemon, pokemon_lenght);
	offset += pokemon_lenght;
	memcpy(buffer->stream + offset, &pos_x, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, &pos_y, sizeof(int));

	free(pokemon);

	return buffer;
}



void* serializar_appeared_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y) {

	t_buffer* buffer = buffer_appeared_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(APPEARED_POKEMON, buffer);

	*bytes = paquete->buffer->size + sizeof(int) * 2;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}
