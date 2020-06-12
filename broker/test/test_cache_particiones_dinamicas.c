#include "test_cache_particiones_dinamicas.h"

void exampleTest(){

}
void agregar_tests_particiones_dinamicas(){
	CU_pSuite suite_configuracion = CU_add_suite("Memoria Cache:Particiones Dinamicas", NULL, NULL);

	CU_add_test(suite_configuracion, "Example", exampleTest);
}
