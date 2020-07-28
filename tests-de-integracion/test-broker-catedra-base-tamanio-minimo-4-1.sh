#!/bin/sh

echo **                      	**
echo **  Test Broker Base	**
echo **                      	**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.catedra.base broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &
