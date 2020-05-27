#include "serializacion.h"

t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer,
		int id_correlativo) {
	t_paquete *paquete = (t_paquete*) malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	paquete->id_correlativo = id_correlativo;
	paquete->buffer = buffer;

	return paquete;
}

void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared) {
	free(mensaje_appeared->pokemon);
	free(mensaje_appeared);
}

void* serializar_paquete(t_paquete* paquete, int bytes) {
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->id_correlativo), sizeof(int));
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

t_buffer* buffer_new_pokemon(char* nombre_pokemon, int pos_x, int pos_y,
		int cantidad) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	int pokemon_length = strlen(pokemon) + 1;

	buffer->size = sizeof(int) * 4 + pokemon_length;
	buffer->stream = malloc(buffer->size);

	int offset = 0;
	memcpy(buffer->stream + offset, &pokemon_length, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(buffer->stream + offset, &pos_x, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, &pos_y, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, &cantidad, sizeof(int));

	free(pokemon);

	return buffer;
}

t_buffer* buffer_get_pokemon(char* nombre_pokemon) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	int pokemon_length = strlen(pokemon) + 1;

	buffer->size = sizeof(int) + pokemon_length;
	buffer->stream = malloc(buffer->size);

	int offset = 0;
	memcpy(buffer->stream + offset, &pokemon_length, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer->stream + offset, pokemon, pokemon_length);

	free(pokemon);

	return buffer;
}

t_buffer* buffer_catch_pokemon(char* nombre_pokemon, int pos_x, int pos_y) {
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

t_buffer* buffer_caught_pokemon(int estado) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	buffer->size = sizeof(int);
	buffer->stream = malloc(sizeof(int));

	memcpy(buffer->stream, &estado, sizeof(int));

	return buffer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* serializar_new_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int cantidad, int id_correlativo) {

	t_buffer* buffer = buffer_new_pokemon(nombre_pokemon, pos_x, pos_y,
			cantidad);
	t_paquete *paquete = crear_paquete(NEW_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

void* serializar_get_pokemon(int* bytes, char* nombre_pokemon,
		int id_correlativo) {

	t_buffer* buffer = buffer_get_pokemon(nombre_pokemon);
	t_paquete *paquete = crear_paquete(GET_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

void* serializar_catch_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int id_correlativo) {
	t_buffer* buffer = buffer_catch_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(CATCH_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

void* serializar_appeared_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int id_correlativo) {

	t_buffer* buffer = buffer_appeared_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(APPEARED_POKEMON, buffer,
			id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

void* serializar_caught_pokemon(int* bytes, int estado, int id_correlativo) {
	t_buffer* buffer = buffer_caught_pokemon(estado);
	t_paquete *paquete = crear_paquete(CAUGHT_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

t_mensaje_appeared* get_mensaje_appeared_by_buffer(t_buffer* buffer) {
	t_mensaje_appeared* mensaje_appeared = (t_mensaje_appeared*) malloc(
			sizeof(t_mensaje_appeared));

	void* stream = buffer->stream;

	memcpy(&(mensaje_appeared->length_pokemon), stream, sizeof(int));
	stream += sizeof(int);

	mensaje_appeared->pokemon = malloc(mensaje_appeared->length_pokemon);
	memcpy(mensaje_appeared->pokemon, stream, mensaje_appeared->length_pokemon);
	stream += mensaje_appeared->length_pokemon;

	memcpy(&(mensaje_appeared->posx), stream, sizeof(int));
	stream += sizeof(int);

	memcpy(&(mensaje_appeared->posy), stream, sizeof(int));
	stream += sizeof(int);

	return mensaje_appeared;
}

char* op_code_to_string(int enum_value) {
	switch (enum_value) {
	case OP_ERROR:
		return "OP_ERROR";
	case NEW_POKEMON:
		return "NEW_POKEMON";
	case GET_POKEMON:
		return "GET_POKEMON";
	case CATCH_POKEMON:
		return "CATCH_POKEMON";
	case APPEARED_POKEMON:
		return "APPEARED_POKEMON";
	case LOCALIZED_POKEMON:
		return "LOCALIZED_POKEMON";
	case CAUGHT_POKEMON:
		return "CAUGHT_POKEMON";
	case SUSCRIPCION:
		return "SUSCRIPCION";
	default:
		return "NULL";
	}
}

