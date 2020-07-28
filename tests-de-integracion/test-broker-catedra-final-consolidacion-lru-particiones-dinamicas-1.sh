#!/bin/sh

echo **                      				**
echo **  Test Consolidacion LRU Particiones Dinamicas  	**
echo **                      				**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.consolidacion.particiones.dinamicas broker.config

echo Actualizando configuracion a LRU
sed -i 's/FIFO/LRU/g' broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &