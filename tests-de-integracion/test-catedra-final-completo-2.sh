#!/bin/sh

echo
echo Enviando mensajes NEW_POKEMON a Broker:
echo

../gameboy/Debug/gameboy BROKER NEW_POKEMON Pikachu 9 9 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Squirtle 5 2 1

../gameboy/Debug/gameboy BROKER NEW_POKEMON Jolteon 2 2 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Flareon 4 6 1
