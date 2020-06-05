#include "serializacion.h"

t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer,
		int id_correlativo) {
	t_paquete *paquete = (t_paquete*) malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	paquete->id_correlativo = id_correlativo;
	paquete->buffer = buffer;

	return paquete;
}


// ELIMINAR
void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_paquete_socket(t_paquete_socket* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared) {
	free(mensaje_appeared->pokemon);
	free(mensaje_appeared);
}

void eliminar_mensaje_catch(t_mensaje_catch* mensaje_catch) {
	free(mensaje_catch->pokemon);
	free(mensaje_catch);
}

void eliminar_mensaje_new(t_mensaje_new* mensaje_new){
	free(mensaje_new->pokemon);
	free(mensaje_new);
}

void eliminar_mensaje_get(t_mensaje_get* mensaje_get){
	free(mensaje_get->pokemon);
	free(mensaje_get);
}


// UTILIDADES
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

// NEW_POKEMON
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

t_mensaje_new* deserializar_mensaje_new_pokemon(t_buffer* buffer) {
	t_mensaje_new* mensaje_new = (t_mensaje_new*) malloc(sizeof(t_mensaje_new));

	int offset = 0;
	memcpy(&(mensaje_new->length_pokemon), buffer->stream + offset,
			sizeof(int));
	mensaje_new->pokemon = malloc(mensaje_new->length_pokemon);
	offset += sizeof(int);
	memcpy(mensaje_new->pokemon, buffer->stream + offset,
			mensaje_new->length_pokemon);
	offset += mensaje_new->length_pokemon;
	memcpy(&(mensaje_new->posx), buffer->stream + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(mensaje_new->posy), buffer->stream + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(mensaje_new->cantidad), buffer->stream + offset, sizeof(int));

	return mensaje_new;
}

// GET_POKEMON
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

void* serializar_get_pokemon(int* bytes, char* nombre_pokemon,
		int id_correlativo) {

	t_buffer* buffer = buffer_get_pokemon(nombre_pokemon);
	t_paquete *paquete = crear_paquete(GET_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_get* deserializar_mensaje_get_pokemon(t_buffer* buffer) {
	t_mensaje_get* mensaje_get = (t_mensaje_get*) malloc(sizeof(t_mensaje_get));

	int offset = 0;
	memcpy(&(mensaje_get->length_pokemon), buffer->stream + offset,
			sizeof(int));
	mensaje_get->pokemon = malloc(mensaje_get->length_pokemon);
	offset += sizeof(int);
	memcpy(mensaje_get->pokemon, buffer->stream + offset,
			mensaje_get->length_pokemon);

	return mensaje_get;
}

// CATCH_POKEMON
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

void* serializar_catch_pokemon(int* bytes, char* nombre_pokemon, int pos_x,
		int pos_y, int id_correlativo) {
	t_buffer* buffer = buffer_catch_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(CATCH_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_catch* deserializar_mensaje_catch_pokemon(t_buffer* buffer) {
	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(
			sizeof(t_mensaje_catch));

	int offset = 0;
	memcpy(&(mensaje_catch->length_pokemon), buffer->stream + offset,
			sizeof(int));

	mensaje_catch->pokemon = malloc(mensaje_catch->length_pokemon);
	offset += sizeof(int);
	memcpy(mensaje_catch->pokemon, buffer->stream + offset,
			mensaje_catch->length_pokemon);

	offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), buffer->stream + offset, sizeof(int));

	offset += sizeof(int);
	memcpy(&(mensaje_catch->posy), buffer->stream + offset, sizeof(int));

	return mensaje_catch;
}

t_mensaje_catch* deserializar_paquete_catch_pokemon(void* package) {

	int offset = 0;
	int op = 0;
	int cod = 0;
	int size = 0;
	memcpy(&(op), package + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(cod), package + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(size), package + offset, sizeof(int));
	offset += sizeof(int);

	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(
			sizeof(t_mensaje_catch));

	memcpy(&(mensaje_catch->length_pokemon), package + offset, sizeof(int));

	mensaje_catch->pokemon = malloc(mensaje_catch->length_pokemon);
	offset += sizeof(int);
	memcpy(mensaje_catch->pokemon, package + offset,
			mensaje_catch->length_pokemon);

	offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), package + offset, sizeof(int));

	offset += sizeof(int);
	memcpy(&(mensaje_catch->posy), package + offset, sizeof(int));

	return mensaje_catch;
}


// APPEARED_POKEMON
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
		int pos_y, int id_correlativo) {

	t_buffer* buffer = buffer_appeared_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(APPEARED_POKEMON, buffer,
			id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

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

// CAUGHT_POKEMON
t_buffer* buffer_caught_pokemon(int estado) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	buffer->size = sizeof(int);
	buffer->stream = malloc(sizeof(int));

	memcpy(buffer->stream, &estado, sizeof(int));

	return buffer;
}

void* serializar_caught_pokemon(int* bytes, int estado, int id_correlativo) {
	t_buffer* buffer = buffer_caught_pokemon(estado);
	t_paquete *paquete = crear_paquete(CAUGHT_POKEMON, buffer, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(int) * 3;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

estado* deserializar_mensaje_caught_pokemon(t_buffer* buffer) {
	estado* mensaje_caught = (estado*) malloc(sizeof(estado));

	int offset = 0;
	memcpy(&(mensaje_caught), buffer->stream + offset,
			sizeof(int));
	return mensaje_caught;
}

/*-------------------------------------------------------------------------------------*/
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

int string_to_op_code(char* enum_cola) {

	if (strcmp(enum_cola, "NEW_POKEMON") == 0)
		return NEW_POKEMON;

	else if (strcmp(enum_cola, "GET_POKEMON") == 0)
		return GET_POKEMON;

	else if (strcmp(enum_cola, "CATCH_POKEMON") == 0)
		return CATCH_POKEMON;

	else if (strcmp(enum_cola, "APPEARED_POKEMON") == 0)
		return APPEARED_POKEMON;

	else if (strcmp(enum_cola, "LOCALIZED_POKEMON") == 0)
		return LOCALIZED_POKEMON;

	else if (strcmp(enum_cola, "CAUGHT_POKEMON") == 0)
		return CAUGHT_POKEMON;

	else if (strcmp(enum_cola, "SUSCRIPCION") == 0)
		return SUSCRIPCION;

	else {
		return OP_ERROR;
	}
}

