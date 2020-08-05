#!/bin/sh

sleep_time=2

echo "  ** Test de Broker **  \n\n"
sh broker-base-tamanio-minimo-4.sh
sh broker-base-tamanio-minimo-16.sh
sh broker-buddy-system-fifo.sh
sh broker-buddy-system-lru.sh
sh broker-particiones-dinamicas-compactacion-fifo.sh
sh broker-particiones-dinamicas-compactacion-lru.sh
sh broker-particiones-dinamicas-consolidacion-fifo.sh
sh broker-particiones-dinamicas-consolidacion-lru.sh

