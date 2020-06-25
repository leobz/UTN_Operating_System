#include "test_administrador_de_mensajes.h"

char const *payload_string = "someString";
int payload_string_size;

void agregar_tests_administrador_de_mensaje() {
	CU_pSuite suite_configuracion = CU_add_suite("Administrador de mensaje", NULL, NULL);

	CU_add_test(suite_configuracion,
			"Adminstrador guarda mensaje en cache",
			test_guardar_mensaje_en_cache_dinamica);

}

void test_guardar_mensaje_en_cache_dinamica() {
	logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);
	inicializar_test_con_particion_dinamica();
	inicializar_test_con_administracion_de_mensajes();

	t_mensaje* mensaje = mensaje_generico();
	t_adm_mensaje* administrator = iniciar_administracion(mensaje);

	agregar_mensaje_memoria_cache(administrator, mensaje);

	CU_ASSERT_EQUAL(administrator->particion_dinamica->tamanio_particion, payload_string_size);
}

void inicializar_test_con_administracion_de_mensajes(){
	inicializar_diccionarios();
	inicalizar_lista_de_todos_las_colas();

	payload_string_size = strlen(payload_string) + 1;
}

t_mensaje* mensaje_generico() {
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->codigo_operacion = NEW_POKEMON;
	mensaje->payload = payload_string;
	mensaje->payload_size = payload_string_size;

	return mensaje;
}
