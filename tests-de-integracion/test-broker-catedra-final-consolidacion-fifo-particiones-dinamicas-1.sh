#!/bin/sh

echo **                      				**
echo **  Test Consolidacion FIFO Particiones Dinamicas  **
echo **                      				**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.consolidacion.particiones.dinamicas broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &
