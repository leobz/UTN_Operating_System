#!/bin/sh

echo **                      	**
echo **  Test Broker Base	**
echo **                      	**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.catedra.base broker.config

echo
echo Actualizando configuracion a Tamanio minimo de particion 16
sed -i 's/TAMANO_MINIMO_PARTICION=4/TAMANO_MINIMO_PARTICION=16/g' broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &
