#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: Base Tamanio Minimo 16  **\n\n\n"


echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/broker.config.catedra.base broker.config

echo "Actualizando TAMANO_MINIMO_PARTICION=16"
sed -i 's/TAMANO_MINIMO_PARTICION=4/TAMANO_MINIMO_PARTICION=16/g' broker.config

echo "Levantando Broker"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1

echo "\nEjecutando 'cargar_memoria_complejo.sh'"
sh delibird-pruebas/cargar_memoria_complejo.sh

echo "\nEjecutando test"
byexample -l shell byexample-pruebas/broker-catedra-base-tamanio-minimo-16.md

# Restaurando configuracion inicial
rm broker.config

# Bajando broker
1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`