#!/bin/sh

echo "\n**  Test Final Prueba completa	**\n" 

echo "Eliminando archivos anteriores"
rm -r ../tall_grass/Files/*     2>/dev/null
rm ../tall_grass/Blocks/*       2>/dev/null
rm *.log                        2>/dev/null



echo "\n**  Parte 1: Gamecard-Broker  **\n" 

echo "Cargando configuracion Broker de la Cátedra"
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.final.completo broker.config
echo "Levanto Broker en segundo plano"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep 0.5

echo "Cargando configuracion Gamecard de la Cátedra"
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.final.completo gamecard.config
echo "Levanto Gamecard en segundo plano"
1>/dev/null 2>/dev/null     ../gamecard/Debug/gamecard &
sleep 0.5


echo "\nEjecutando script new_pokemon_antes_team.sh (NEW_POKEMON -> Broker)"
../gameboy/Debug/gameboy BROKER NEW_POKEMON Pikachu 9 9 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Squirtle 5 2 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Jolteon 2 2 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Flareon 4 6 1
sleep 0.5

echo "\nComprobacion: Se crearon los 4 pokemon en el FileSystem:"
echo
cat ../tall_grass/Files/Pikachu/Metadata.bin
echo
cat ../tall_grass/Files/Squirtle/Metadata.bin
echo
cat ../tall_grass/Files/Jolteon/Metadata.bin
echo
cat ../tall_grass/Files/Flareon/Metadata.bin
echo



echo "\n**  Parte 2: 2 instancias de Team  **\n" 

echo "Cargando configuracion Team 1 de la Cátedra"
cp team.config team.config.auxiliar; rm team.config; cp team.1.config.final.completo team.config
echo "Levanto Team 1 en segundo plano"
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nCargando configuracion Team 2 de la Cátedra"
rm ../team/Debug/team.config; cp team.2.config.final.completo ../team/Debug/team.config
echo "Levanto Team 2 en segundo plano"
cd ../team/Debug/
1>/dev/null 2>/dev/null     ./team & 

sleep 0.5