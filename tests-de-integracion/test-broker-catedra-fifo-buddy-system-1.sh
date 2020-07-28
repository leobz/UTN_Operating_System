#!/bin/sh

echo **                      	**
echo **  Test Buddy System FIFO	**
echo **                      	**
echo

echo
echo Cargando configuracion de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.buddy.system broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &
