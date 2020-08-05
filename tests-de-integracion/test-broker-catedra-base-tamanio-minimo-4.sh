#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: Base Tamanio Minimo 4  **\n\n\n"


echo "Ejecutando 'sh test-broker-catedra-base-tamanio-minimo-4.sh'"

rm *.log
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.catedra.base broker.config

1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1


echo "\nEjecutando 'test-broker-catedra-base-tamanio-minimo-4-2.sh'"
sh test-broker-catedra-base-tamanio-minimo-4-2.sh

echo "\nEjecutando 'test-broker-catedra-base-tamanio-minimo-4-3.sh'"
sh test-broker-catedra-base-tamanio-minimo-4-3.sh


echo "\nEjecutando test:"
byexample -l shell broker-catedra-base-tamanio-minimo-4.md

# Restaurando configuracion inicial
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar

1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`