#!/bin/sh

echo
echo Enviando mensajes CAUGHT_POKEMON, NEW_POKEMON, CATCH_POKEMON y SUSCRIPCION a Broker:
echo


../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 1 OK
../gameboy/Debug/gameboy BROKER CAUGHT_POKEMON 2 FAIL
../gameboy/Debug/gameboy BROKER NEW_POKEMON Pikachu 2 3 1
../gameboy/Debug/gameboy BROKER CATCH_POKEMON Muk 4 5
../gameboy/Debug/gameboy SUSCRIPCION NEW_POKEMON 10 1
../gameboy/Debug/gameboy BROKER CATCH_POKEMON Charmander 4 5
