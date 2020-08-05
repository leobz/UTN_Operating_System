#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: Base Tamanio Minimo 16  **\n\n\n"


echo "Ejecutando 'sh test-broker-catedra-base-tamanio-minimo-16.sh'"

rm *.log
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.catedra.base broker.config
echo Actualizando configuracion a Tamanio minimo de particion 16
sed -i 's/TAMANO_MINIMO_PARTICION=4/TAMANO_MINIMO_PARTICION=16/g' broker.config

1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1

echo "\nEjecutar 'test-broker-catedra-base-tamanio-minimo-16-2.sh'?"
#read "rta"
sh test-broker-catedra-base-tamanio-minimo-16-2.sh
#sleep $(expr $sleep_time \* 3)

echo "\nEjecutando test:"
#read "rta"
byexample -l shell broker-catedra-base-tamanio-minimo-16.md

# Restaurando configuracion inicial
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar

1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`