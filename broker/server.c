/*
 * server.c
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#include"colas.h"

void loggear_nueva_conexion(t_log* logger, t_paquete_socket* paquete) {

	log_info(logger, "[CONEXION] COD_OP:%s",op_code_to_string(paquete->codigo_operacion));
}

void procesar_mensaje_recibido(t_paquete_socket* paquete) {

	if ((paquete->codigo_operacion >= 0) && (paquete->codigo_operacion <= 5)){

		if(paquete->id_correlativo==0){
			ingresar_en_cola_y_cache(paquete);
			liberar_paquete_socket(paquete);
		}
		else{
			if(esta_en_diccionario(mensajes_iguales,paquete->id_correlativo)){
				enviar_confirmacion(++id_mensaje,CONFIRMACION,paquete->socket_cliente);
				liberar_paquete_socket(paquete);
			}
			else{
				ingresar_en_cola_y_cache(paquete);

				char*id_men=pasar_a_char(paquete->id_correlativo);
				meter_en_diccionario(mensajes_iguales,id_men,paquete->id_proceso);
				free(id_men);

				liberar_paquete_socket(paquete);
			}
		}
	}
	else {

		switch (paquete->codigo_operacion) {

		case SUSCRIPCION:{


			log_info(logger, "[SUSCRIPCION] Cola:%s ID_Proceso:%d", op_code_to_string(paquete->cola),paquete->id_proceso);

			t_proceso* proceso;

			if(esta_en_diccionario(dic_suscriptores[paquete->cola],paquete->id_proceso)){ //si el proceso ya estaba registrado

				bool buscar_suscriptor(t_proceso* proceso){
					return proceso->id_proceso==paquete->id_proceso;
				}
				t_proceso* proceso_encontrado=list_find(suscriptores[paquete->cola], &buscar_suscriptor);

				proceso_encontrado->socket=paquete->socket_cliente;//le cambio el socket al suscriptor y actualizo los diccionarios
				proceso=proceso_encontrado;
			}

			else{			//si no existia ese proceso lo creo y lo meto en la lista y en los diccionarios
				proceso=malloc(sizeof(t_proceso));
				proceso->id_proceso=paquete->id_proceso;
				proceso->socket=paquete->socket_cliente;
				list_add(suscriptores[paquete->cola], proceso);
				meter_en_diccionario(dic_suscriptores[paquete->cola],paquete->id_proceso,proceso);
				meter_en_diccionario(subscribers,paquete->id_proceso,proceso);
			}

			cola_paquete=paquete->cola;

			pthread_t thread_subscribers;
			pthread_create(&thread_subscribers,NULL,&verificar_cache,proceso);
			pthread_detach(thread_subscribers);

			break;}

		case CONFIRMACION:{

			log_info(logger, "[MSG_RECIBIDO] CON ID_MENSAJE: %d", paquete->id_mensaje);

			t_adm_mensaje* administrador_confirmado = obtener_de_diccionario(administracion_por_id, paquete->id_mensaje);
			t_proceso* proceso_confirmado = obtener_de_diccionario(subscribers,paquete->id_proceso);

			list_add(administrador_confirmado->suscriptores_confirmados, proceso_confirmado); //Lo agrego a la lista deconfirmados de ese mensaje

			//printf("Recibida confirmacion de proceso: %d\n",paquete->id_proceso);

			break;}

		case OP_ERROR:

			printf("Error al recibir mensaje del socket: %d\n",
					paquete->socket_cliente);
			pthread_exit(NULL);
			break;

		default:
			pthread_exit(NULL);
			break;
		}

		free(paquete);
	}
}


void verificar_cache(t_proceso* proceso){
	//printf("Entro a hilo verificar cache\n");
	int id_suscriptor=proceso->id_proceso;
	int socket=proceso->socket;
	int num_cola=cola_paquete;

	//leer primero iteracion ****

	void enviar_mensajes_cacheados(t_adm_mensaje* actual_administrator){

		bool confirmo_el_mensaje(t_proceso* proceso_a_comparar){
			return proceso_a_comparar->id_proceso == id_suscriptor;
		}

		if(!list_any_satisfy(actual_administrator->suscriptores_confirmados, &confirmo_el_mensaje)){
			//si ese proceso no se encuentra entre los procesos que habian confirmado el mensaje

			int bytes=0;
			void* mensaje_para_enviar = generar_mensaje(actual_administrator,&bytes);

			int validez = enviar_mensaje_con_retorno(socket,mensaje_para_enviar,bytes);
			if(validez!=1){
				//log_info(logger,"Mensaje %s enviado a suscriptor con id: %d y socket: %d",num_cola, proceso->id_proceso, proceso->socket);
				list_add(actual_administrator->suscriptores_enviados,proceso);
			}
			free(mensaje_para_enviar);
		}

	}

//***** primero itera
	list_iterate(administradores[num_cola],&enviar_mensajes_cacheados); //checkeo por cada estructura de administracion si se le habia enviado ese mensaje

}

t_mensaje* preparar_mensaje(t_paquete_socket* paquete) {
	t_mensaje* mensaje_a_preparar = malloc(sizeof(t_mensaje));

	pthread_mutex_lock(&global); //id_mensaje es una variable compartida

	int identificador=++id_mensaje;
	enviar_confirmacion(identificador,CONFIRMACION,paquete->socket_cliente);

	mensaje_a_preparar->id_mensaje = identificador;

	pthread_mutex_unlock(&global);

	mensaje_a_preparar->codigo_operacion = paquete->codigo_operacion;
	mensaje_a_preparar->id_correlativo = paquete->id_correlativo;
	mensaje_a_preparar->payload_size = paquete->buffer->size;
	mensaje_a_preparar->payload = malloc(paquete->buffer->size);
	memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
	mensaje_a_preparar->siguiente = NULL;

	return mensaje_a_preparar;
}

void ingresar_en_cola_y_cache(t_paquete_socket* paquete){

	t_mensaje* mensaje_a_encolar;
	mensaje_a_encolar = preparar_mensaje(paquete);

	pthread_mutex_lock(&mutex[paquete->codigo_operacion]);
		insertar_mensaje(mensaje_a_encolar, paquete->codigo_operacion);
	pthread_mutex_unlock(&mutex[paquete->codigo_operacion]);

	loggear_nueva_conexion(logger, paquete);

	sem_post(&cola_vacia[paquete->codigo_operacion]);

}


void enviar_confirmacion(int id,op_code confirmacion,int socket){
	int offset=0;
	int id_falso=0;

	void*enviar=malloc(sizeof(int)*3);
	memcpy(enviar,&confirmacion,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id,sizeof(int));
	offset+=sizeof(int);
	memcpy(enviar+offset,&id_falso,sizeof(int)); //valor nulo pq no es un id_proceso

	enviar_mensaje(socket,enviar,sizeof(int)*3);
	//le devuelve al proceso emisor el id del mensaje
}
