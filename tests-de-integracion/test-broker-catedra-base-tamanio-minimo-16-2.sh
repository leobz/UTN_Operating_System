#!/bin/sh

echo
echo Enviando mensajes CATCH_POKEMON, CAUGHT_POKEMON y GET_POKEMON a Broker:
echo

../gameboy/Debug/gameboy BROKER CATCH_POKEMON Pikachu 9 3
../gameboy/Debug/gameboy BROKER CATCH_POKEMON Squirtle 9 3

../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 10 OK
../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 11 FAIL

../gameboy/Debug/gameboy BROKER CATCH_POKEMON Bulbasaur 1 7
../gameboy/Debug/gameboy BROKER CATCH_POKEMON Charmander 1 7

../gameboy/Debug/gameboy BROKER GET_POKEMON Pichu
../gameboy/Debug/gameboy BROKER GET_POKEMON Raichu
