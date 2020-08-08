#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: FIFO Particiones Dinamicas  **\n\n\n"


echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/broker.config.compactacion.particiones.dinamicas broker.config

echo "Levantando Broker"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1

echo "\nEjecutando 'compactacion_basico.sh'"
sh delibird-pruebas/compactacion_basico.sh

echo "\nEjecutando test"
byexample -l shell byexample-pruebas/broker-catedra-final-compactacion-fifo-particiones-dinamicas.md

# Restaurando configuracion inicial
rm broker.config;

# Bajando broker
1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`