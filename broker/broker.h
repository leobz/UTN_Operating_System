/*
 * broker.h
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */

#ifndef BROKER_H_
#define BROKER_H_

void inicializar_broker(t_socket_config **broker_config, t_log **logger);
void finalizar_broker(t_socket_config* broker_config, t_log* logger);


#endif /* BROKER_H_ */
