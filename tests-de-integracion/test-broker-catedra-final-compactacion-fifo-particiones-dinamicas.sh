#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Broker Catedra: FIFO Particiones Dinamicas  **\n\n\n"


echo "Ejecutando 'sh test-broker-catedra-final-compactacion-fifo-particiones-dinamicas.sh'"

rm *.log
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.compactacion.particiones.dinamicas broker.config

1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 1


echo "\nEjecutando 'test-broker-catedra-final-compactacion-fifo-particiones-dinamicas-2.sh'"
sh test-broker-catedra-final-compactacion-fifo-particiones-dinamicas-2.sh

echo "\nEjecutando test:"
byexample -l shell broker-catedra-final-compactacion-fifo-particiones-dinamicas.md

# Restaurando configuracion inicial
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar

1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`