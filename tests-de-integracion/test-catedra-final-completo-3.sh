#!/bin/sh

echo **                      		**
echo **  Test Final Prueba completa	**
echo **                      		**
echo

echo
echo Cargando configuracion Team 1 de la Cátedra
cp team.config team.config.auxiliar; rm team.config; cp team.1.config.final.completo team.config

echo
echo Levanto Team 1 en segundo plano:
../team/Debug/team &

sleep 0.5

echo
echo Cargando configuracion Team 2 de la Cátedra
rm team.config; cp team.2.config.final.completo team.config

echo
echo Levanto Team 2 en segundo plano:
../team/Debug/team &

