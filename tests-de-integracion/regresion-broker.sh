#!/bin/sh

sleep_time=2

echo "  ** Test de Broker **  \n\n"
sh test-broker-catedra-base-tamanio-minimo-4.sh
sh test-broker-catedra-base-tamanio-minimo-16.sh
sh test-broker-catedra-fifo-buddy-system.sh
sh test-broker-catedra-final-compactacion-fifo-particiones-dinamicas.sh
sh test-broker-catedra-final-compactacion-lru-particiones-dinamicas.sh
sh test-broker-catedra-final-consolidacion-fifo-particiones-dinamicas.sh
sh test-broker-catedra-final-consolidacion-lru-particiones-dinamicas.sh
sh test-broker-catedra-lru-buddy-system.sh
