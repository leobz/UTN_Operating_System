#!/bin/sh

echo **                      		**
echo **  Test Final Prueba completa	**
echo **                      		**
echo

echo
echo Cargando configuracion Broker de la Cátedra
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.final.completo broker.config

echo
echo Levanto Broker en segundo plano:
../broker/Debug/broker &

sleep 0.5

echo
echo Cargando configuracion Gamecard de la Cátedra
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.final.completo gamecard.config

echo
echo Levanto Gamecard en segundo plano:
../gamecard/Debug/gamecard &
