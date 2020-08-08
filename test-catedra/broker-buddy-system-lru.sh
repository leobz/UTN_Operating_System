#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: LRU BS  **\n\n\n"


echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/broker.config.buddy.system broker.config

echo "Actualizando ALGORITMO_REEMPLAZO=LRU"
sed -i 's/FIFO/LRU/g' broker.config

echo "Levantando Broker"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1

echo "\nEjecutando 'buddy_basico.sh'"
sh delibird-pruebas/buddy_basico.sh

echo "\nEjecutando test"
byexample -l shell byexample-pruebas/broker-catedra-final-lru-buddy-system.md

# Restaurando configuracion inicial
rm broker.config;

# Bajando broker
1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`
