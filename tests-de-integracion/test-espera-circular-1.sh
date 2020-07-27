#!/bin/sh

echo **                      **
echo ** Test Espera Circular **
echo **                      **
echo

echo
echo Cargando configuracion de la Cátedra
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.espera.circular team.config

echo
echo Levanto Team en segundo plano:
../team/Debug/team &

