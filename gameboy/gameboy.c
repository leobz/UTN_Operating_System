#include "gameboy.h"
#include "test/testing.h"


int main(int argc, char **argv) {
	inicializar_gameboy(&gameboy_config, &logger,&logger_debug);
	// ./gameboy test
	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();
	}

	// ./gameboy SUSCRIPCION [COLA_DE_MENSAJES] [TIEMPO]
	if (strcmp(argv[1], "SUSCRIPTOR") == 0) {
		if (argc != 4) {
			printf("ERROR: Comando invalido, pruebe: ./gameboy SUSCRIPCION [COLA_DE_MENSAJES] [TIEMPO]\n");
			log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy SUSCRIPCION [COLA_DE_MENSAJES] [TIEMPO]");
			exit(-1);
		}

		int cola = string_to_op_code(argv[2]);
		int tiempo = atoi(argv[3]);


		int conexion = crear_conexion(gameboy_config->ip_broker,gameboy_config->puerto_broker);

		if (conexion == -1)
		{
			log_error(logger,"ERROR: Conexion con [Broker] no establecida");
			//exit(-1);
		}
		// TODO: funcion de alarm
		// TODO: buscar como terminar un hilo por cierto tiempo


		pthread_t hilo_gameboy;
		pthread_create(&hilo_gameboy,NULL,(void*)servidor_gameboy,conexion);
		pthread_detach(hilo_gameboy);

		t_suscripcion *suscripcion = malloc(sizeof(t_suscripcion));
		suscripcion->cod_operacion = SUSCRIPCION;
		suscripcion->cola_a_suscribir = cola;
		suscripcion->id_proceso=gameboy_config->id_proceso_gameboy;


		void *a_enviar = empaquetar_suscripcion(suscripcion);

		log_info(logger, "%ld Conexion establecida con [Broker]", (long)getpid());
		enviar_mensaje(conexion, a_enviar, sizeof(int) * 3);
		log_info(logger,"%ld Mensaje enviado a [Broker]: SUSCRIPCION id_proceso %d cola %d por %d segundos", (long)getpid(), gameboy_config->id_proceso_gameboy, cola, tiempo);

		sleep(tiempo);

		liberar_conexion(conexion);
	}

	if (strcmp(argv[1], "BROKER") == 0) {

		// ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
		if (strcmp(argv[2], "NEW_POKEMON") == 0) {
			if (argc != 7) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ../gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int cantidad = atoi(argv[6]);
			int id_mensaje=0;
			int id_correlativo = 0;

			int conexion = crear_conexion(gameboy_config->ip_broker,
					gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;
			void *a_enviar = serializar_new_pokemon(&bytes, pokemon, pos_x,pos_y, cantidad,id_mensaje, id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);

			liberar_conexion(conexion);
		}

		// ./gameboy BROKER GET_POKEMON [POKEMON]
		if (strcmp(argv[2], "GET_POKEMON") == 0) {

			if (argc != 4) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER GET_POKEMON [POKEMON]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy BROKER GET_POKEMON [POKEMON]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int id_mensaje=0;
			int id_correlativo = 0;

			int conexion = crear_conexion(gameboy_config->ip_broker,
					gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;
			void *a_enviar = serializar_get_pokemon(&bytes, pokemon, id_mensaje,
					id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);

			liberar_conexion(conexion);
		}

		// ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]
		if (strcmp(argv[2], "CATCH_POKEMON") == 0) {

			if (argc != 6) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int id_mensaje=0;
			int id_correlativo = 0;

			int conexion = crear_conexion(gameboy_config->ip_broker,
					gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes=0;
			void *a_enviar = serializar_catch_pokemon(&bytes, pokemon, pos_x,pos_y,id_mensaje, id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);


			liberar_conexion(conexion);
		}

		// ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]
		if (strcmp(argv[2], "APPEARED_POKEMON") == 0) {

			if (argc != 7) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int id_mensaje=0;
			int id_correlativo = atoi(argv[6]);

			int conexion = crear_conexion(gameboy_config->ip_broker,
					gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;
			void *a_enviar = serializar_appeared_pokemon(&bytes, pokemon, pos_x,
					pos_y,id_mensaje, id_correlativo);
			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);

			liberar_conexion(conexion);
		}

		// ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]
		if (strcmp(argv[2], "CAUGHT_POKEMON") == 0) {

			if (argc != 5) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ../gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]");
				exit(-1);
			}

			int id_correlativo = atoi(argv[3]);
			int estado = 0;
			int id_mensaje=0;

			if (strcmp(argv[4], "OK") == 0) {
				estado = OK;
			} else if (strcmp(argv[4], "FAIL") == 0) {
				estado = FAIL;
			}

			int conexion = crear_conexion(gameboy_config->ip_broker,
					gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;

			void *a_enviar = serializar_caught_pokemon(&bytes, estado,id_mensaje,id_correlativo);
			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);

			liberar_conexion(conexion);

		}

		// ./gameboy BROKER LOCALIZED_POKEMON [POKEMON] [CANT_PARES_POSICIONES] [POSX] [POSY].. [ID_MENSAJE_CORRELATIVO]
		if (strcmp(argv[2], "LOCALIZED_POKEMON") == 0) {
			if (argc < 7 ) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy BROKER LOCALIZED_POKEMON [POKEMON] [CANT_PARES_POSICIONES] [POSX] [POSY].. [ID_MENSAJE_CORRELATIVO]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy BROKER LOCALIZED_POKEMON [POKEMON] [CANT_PARES_POSICIONES] [POSX] [POSY].. [ID_MENSAJE_CORRELATIVO]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int cantidad_de_posiciones = atoi(argv[4]);
//			t_posiciones posi[cantidad_de_posiciones];
			t_list* posiciones = list_create();

			int posicion_argumentos = 5;
			for (int indice_par = 0; indice_par < cantidad_de_posiciones; indice_par++) {
				t_posiciones* posicion = malloc(sizeof(t_posiciones));

				posicion->posx = atoi(argv[posicion_argumentos]);
				posicion_argumentos++;
				posicion->posy = atoi(argv[posicion_argumentos]);
				posicion_argumentos++;

				list_add(posiciones, posicion);
			}

			int id_correlativo = atoi(argv[posicion_argumentos]);

			int conexion = crear_conexion(gameboy_config->ip_broker, gameboy_config->puerto_broker);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Broker] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;
			void* a_enviar = serializar_localized_pokemon(&bytes,pokemon,posiciones,0, id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);
			recibir_id_correlativo(conexion);

			liberar_conexion(conexion);
		}

	}

	if (strcmp(argv[1], "TEAM") == 0) {

		// ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]
		if (strcmp(argv[2], "APPEARED_POKEMON") == 0) {
			if (argc < 6) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]");
				exit(-1);
			}
			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int id_mensaje=0;
			int id_correlativo = 0;

			int conexion = crear_conexion(gameboy_config->ip_team,
					gameboy_config->puerto_team);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [Team] no estable1cida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [Team]");

			int bytes;
			void *a_enviar = serializar_appeared_pokemon(&bytes, pokemon, pos_x,
					pos_y,id_mensaje, id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);

			recibir_id_correlativo(conexion);
			liberar_conexion(conexion);
		}
	}

	if (strcmp(argv[1], "GAMECARD") == 0) {

		// ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE]
		if (strcmp(argv[2], "NEW_POKEMON") == 0) {

			if (argc != 8) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int cantidad = atoi(argv[6]);
			int id_mensaje = atoi(argv[7]);
			int id_correlativo=0;

			int conexion = crear_conexion(gameboy_config->ip_gamecard,gameboy_config->puerto_gamecard);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [GAMECARD] no establecida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [GAMECARD]");

			int bytes=0;
			void *a_enviar = serializar_new_pokemon(&bytes, pokemon, pos_x,
					pos_y, cantidad, id_mensaje,id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);

			liberar_conexion(conexion);
		}

		// ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
		if (strcmp(argv[2], "CATCH_POKEMON") == 0) {

			if (argc != 7) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int pos_x = atoi(argv[4]);
			int pos_y = atoi(argv[5]);
			int id_mensaje = atoi(argv[6]);
			int id_correlativo=0;

			int conexion = crear_conexion(gameboy_config->ip_gamecard,gameboy_config->puerto_gamecard);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [GAMECARD] no estable1cida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [GAMECARD]");

			int bytes;
			void *a_enviar = serializar_catch_pokemon(&bytes, pokemon, pos_x,
					pos_y, id_mensaje,id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);


			liberar_conexion(conexion);
		}

		// ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE]
		if (strcmp(argv[2], "GET_POKEMON") == 0) {

			if (argc != 5) {
				printf("ERROR: Comando invalido, pruebe: ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE]\n");
				log_error(logger, "ERROR: Comando invalido, pruebe: ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE]");
				exit(-1);
			}

			char *pokemon = argv[3];
			int id_mensaje = atoi(argv[4]);
			int id_correlativo=0;

			int conexion = crear_conexion(gameboy_config->ip_gamecard,gameboy_config->puerto_gamecard);

			if (conexion == -1) {
				log_error(logger,"ERROR: Conexion con [GAMECARD] no estable1cida");
				exit(-1);
			}

			log_info(logger, "Conexion establecida con [GAMECARD]");

			int bytes;
			void *a_enviar = serializar_get_pokemon(&bytes, pokemon,id_mensaje,id_correlativo);

			enviar_mensaje(conexion, a_enviar, bytes);

			liberar_conexion(conexion);
		}
	}

	finalizar_gameboy(gameboy_config, logger);
	return 0;
}

void inicializar_gameboy(t_gameboy_config **gameboy_config, t_log **logger,t_log **logger_debug) {
	*gameboy_config = cargar_gameboy_config("gameboy.config");
	*logger = iniciar_logger("gameboy.log", "gameboy", LOG_LEVEL_INFO);
	*logger_debug = iniciar_logger("gameboy_debug.log", "gameboy", LOG_LEVEL_INFO);
}

void finalizar_gameboy(t_gameboy_config *gameboy_config, t_log *logger) {
	destruir_gameboy_config(gameboy_config);
	destruir_logger(logger);
	destruir_logger(logger_debug);
}

void parsear_gameboy_config(t_gameboy_config *gameboy_config, t_config *config) {
	gameboy_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));
	gameboy_config->ip_gamecard = strdup(
			config_get_string_value(config, "IP_GAMECARD"));
	gameboy_config->ip_team = strdup(
			config_get_string_value(config, "IP_TEAM"));
	gameboy_config->ip_gameboy = strdup(
			config_get_string_value(config, "IP_GAMEBOY"));
	gameboy_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));
	gameboy_config->puerto_gamecard = strdup(
			config_get_string_value(config, "PUERTO_GAMECARD"));
	gameboy_config->puerto_team = strdup(
			config_get_string_value(config, "PUERTO_TEAM"));
	gameboy_config->puerto_gameboy = strdup(
			config_get_string_value(config, "PUERTO_GAMEBOY"));
	gameboy_config->id_proceso_gameboy=config_get_int_value(config,"ID_PROCESO");
}

t_gameboy_config *cargar_gameboy_config(char *path_archivo) {
	t_config *config;
	t_gameboy_config *gameboy_config;

	config = leer_config(path_archivo);
	gameboy_config = malloc(sizeof(t_gameboy_config));

	parsear_gameboy_config(gameboy_config, config);
	destruir_config(config);
	return gameboy_config;
}

void destruir_gameboy_config(t_gameboy_config *gameboy_config) {
	free(gameboy_config->ip_broker);
	free(gameboy_config->ip_gamecard);
	free(gameboy_config->ip_team);
	free(gameboy_config->ip_gameboy);
	free(gameboy_config->puerto_broker);
	free(gameboy_config->puerto_gamecard);
	free(gameboy_config->puerto_team);
	free(gameboy_config->puerto_gameboy);
	free(gameboy_config);
}
