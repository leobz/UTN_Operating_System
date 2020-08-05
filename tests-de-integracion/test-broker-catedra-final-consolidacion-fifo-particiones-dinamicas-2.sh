#!/bin/sh

# echo
# echo Enviando mensajes CAUGHT_POKEMON, CATCH_POKEMON y SUSCRIPCION a Broker:
# echo

../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 1 OK
../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 2 FAIL

../gameboy/Debug/gameboy BROKER CATCH_POKEMON Pikachu 2 3
../gameboy/Debug/gameboy BROKER CATCH_POKEMON Squirtle 5 2

../gameboy/Debug/gameboy BROKER CATCH_POKEMON Onyx 4 5

../gameboy/Debug/gameboy SUSCRIPTOR CAUGHT_POKEMON 10

../gameboy/Debug/gameboy BROKER CATCH_POKEMON Charmander 4 5
