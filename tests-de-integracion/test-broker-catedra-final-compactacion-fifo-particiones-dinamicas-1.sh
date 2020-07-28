#!/bin/sh

echo **                      				**
echo **  Test Compactación FIFO Particiones Dinamicas  	**
echo **                      				**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.compactacion.particiones.dinamicas broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &
