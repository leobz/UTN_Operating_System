#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<stdio.h>
#include "team.h"

void cargar_objetivo_global (void){

	t_config *config = config_create("./team.config");
	char **read_array = config_get_array_value(config,"POKEMON_ENTRENADORES");
	t_list *objetivo_global_desordenado = list_create();

	void a_la_lista(char *pokemon_requerido){
		if(pokemon_requerido != NULL){
			list_add(objetivo_global_desordenado,pokemon_requerido);
		}
	}

	void imprimir(char *string){
		if(string != NULL){
			char ** objetivos_entrenadores = string_split(string,"|");
			string_iterate_lines(objetivos_entrenadores,a_la_lista);
			printf("Read: %s \n", string);
		} else{
			printf("Got NULL \n");
		}
	}

	string_iterate_lines(read_array, imprimir);

	void mostrar(void *elemento){
		printf("El elemento: %s\n",(char *)elemento);
	}
	list_iterate(objetivo_global_desordenado,mostrar);

	t_list *objetivo_global_final = list_create();

	//for(int i=0;objetivo_global_final[i]!=NULL;i++){


	//}
	free(config);
	free(read_array);
	free(objetivo_global_desordenado);
}
void parsear_team_config(t_team_config *team_config, t_config *config){
		team_config->posiciones_entrenadores=config_get_array_value(config,"POSICIONES_ENTRENADORES");
		team_config->pokemon_entrenadores=config_get_array_value(config,"POKEMON_ENTRENADORES");
		team_config->objetivos_entrenadores=config_get_array_value(config,"OBJETIVOS_ENTRENADORES");
		team_config->tiempoDeReconexion=config_get_int_value(config ,"TIEMPO_RECONEXION");
		team_config->retardo_ciclo_cpu=config_get_int_value(config,"RETARDO_CICLO_CPU");
		team_config->algoritmo_de_planificacion=config_get_string_value(config,"ALGORITMO_PLANIFICACION");
		team_config->quantum=config_get_int_value(config,"QUANTUM");
		team_config->estimacion_inicial=config_get_int_value(config,"IP_BROKER");
		team_config->ip_broker=config_get_int_value(config,"ESTIMACION_INICIAL");
		team_config->puerto_broker=config_get_int_value(config,"PUERTO_BROKER");
		team_config->log_file=config_get_string_value(config,"LOG_FILE");
	}
t_team_config *cargar_team_config(char *path_archivo){
	t_config *config;
	t_team_config *team_config;

	config=leer_config(path_archivo);
	team_config=malloc(sizeof(t_team_config));

	parsear_team_config(team_config,config);
	destruir_config(config);

	return team_config;
}
void destruir_team_config(t_team_config *team_config){
	free(team_config->posiciones_entrenadores);
	free(team_config->pokemon_entrenadores);
	free(team_config->objetivos_entrenadores);
	free(team_config->algoritmo_de_planificacion);
	free(team_config->log_file);
	free(team_config);
}
int cargar_configuracion(void){

	t_team_config *team_config = cargar_team_config("team.config");
	destruir_team_config(team_config);
	printf("funciono");
	return 0;
}
/*crear_tcb_entrenadores(t_team_config){

}*/
int main(){
	 cargar_objetivo_global();
	 cargar_configuracion();
	 //crear_tcb_entrenadores(t_team_config);
	return 0;
}
//int i=0,j,ptr=0,flag;
/*char strlist[NUMBER_OF_STRING][LENGTH_OF_STRING];
int count[NUMBER_OF_STRING];
while (name[i++]!=NULL){
    j=flag=0;
    while (j++<ptr){
        if (strcmp(name[i],strlist[j])){
            flag=1;
            count[j]++;
            break;
        }
    }
    if (!flag){
        strcpy(strlist[ptr],name[i]);
        count[ptr]=1;
        ptr++;
    }
}

*/
