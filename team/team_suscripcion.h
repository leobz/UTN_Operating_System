#ifndef TEAM_SUSCRIPCION_H_
#define TEAM_SUSCRIPCION_H_

#include "../utils/config.h"
#include "../utils/log.h"

#include "../utils/servidor.h"
#include "../utils/cliente.h"
#include "team.h"


t_mensaje_new* mensaje_new;
t_mensaje_get* mensaje_get;
t_mensaje_catch* mensaje_catch;
t_mensaje_appeared* mensaje_appeared;
t_mensaje_localized* mensaje_localized;
t_mensaje_caught* mensaje_caught;

void recibir_mensaje_get(int conexion);
#endif /* TEAM_SUSCRIPCION_H_ */
