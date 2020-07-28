#include "test_localized.h"



void test_transformar_localized_en_appeareds() {
//	t_posiciones pos[2];
//
//	pos[0].posx = 0;
//	pos[0].posy = 0;
//	pos[1].posx = 1;
//	pos[1].posy = 1;
//
//	t_buffer* buffer_localized = buffer_localized_pokemon("Pikachu", 2, pos);
//	t_mensaje_localized* mensaje_localized = deserializar_mensaje_localized_pokemon(buffer_localized);
//
//	CU_ASSERT_STRING_EQUAL(mensaje_localized->pokemon, "Pikachu");
//
//	//t_list* appeareds = transformar_localized_en_appeareds(mensaje_localized);
//
//	t_mensaje_appeared* ap_1 = list_get(appeareds, 0);
//	t_mensaje_appeared* ap_2 = list_get(appeareds, 1);
//
//	CU_ASSERT_EQUAL(list_size(appeareds), 2);
//
//	CU_ASSERT_STRING_EQUAL(ap_1->pokemon, "Pikachu");
//	CU_ASSERT_STRING_EQUAL(ap_2->pokemon, "Pikachu");
//
//	CU_ASSERT_EQUAL(ap_1->posx, 0);
//	CU_ASSERT_EQUAL(ap_1->posy, 0);
//
//	CU_ASSERT_EQUAL(ap_2->posx, 1);
//	CU_ASSERT_EQUAL(ap_2->posy, 1);

}

void agregar_tests_localized(){
	CU_pSuite suite_configuracion = CU_add_suite("Acciones de LOCALIZED", NULL, NULL);

	CU_add_test(suite_configuracion,
				"Un mensaje localized se transforma en appeareds",
				test_transformar_localized_en_appeareds);

}

