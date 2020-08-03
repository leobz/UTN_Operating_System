#!/bin/sh

# echo
# echo Enviando mensajes NEW_POKEMON a Team:
# echo

../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Jolteon 2 2
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Flareon 4 6
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Umbreon 10 6
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Espeon 7 1
../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Vaporeon 4 10

#echo Restaurando configuracion inicial
cp team.config team.config.catedra.espera.circular; rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar

