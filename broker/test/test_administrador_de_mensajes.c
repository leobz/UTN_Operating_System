#include "test_administrador_de_mensajes.h"

void agregar_tests_administrador_de_mensaje() {
	CU_pSuite suite_configuracion = CU_add_suite("Administrador de mensaje", NULL, NULL);

	CU_add_test(suite_configuracion,
			"Adminstrador guarda mensaje en cache",
			test_guardar_mensaje_en_cache_dinamica);

}


t_mensaje* mensaje_generico() {
	char const *texto = "someString";
	int texto_size = strlen(texto) + 1;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	mensaje->payload = texto;
	mensaje->payload_size = texto_size;

	return mensaje;
}

void test_guardar_mensaje_en_cache_dinamica() {
	inicializar_test_con_particion_dinamica();

	t_adm_mensaje* administrator = iniciar_administracion(mensaje_generico());

}
