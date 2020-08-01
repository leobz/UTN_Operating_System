#!/bin/sh

ciclo_cpu_time=1
sleep_time=1

echo "\n**  Test Final Prueba completa	**\n" 

echo "Eliminando archivos anteriores"
rm -r ../tall_grass/Files/*     2>/dev/null
rm ../tall_grass/Blocks/*       2>/dev/null
rm *.log                        2>/dev/null
rm ../team/Debug/*.log          2>/dev/null



echo "\n**  Parte 1: Gamecard-Broker  **\n" 

echo "Cargando configuracion Broker de la Cátedra"
cp broker.config broker.config.auxiliar; rm broker.config; cp broker.config.final.completo broker.config
echo "Levanto Broker en segundo plano"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep $sleep_time

echo "Cargando configuracion Gamecard de la Cátedra"
cp gamecard.config gamecard.config.auxiliar; rm gamecard.config; cp gamecard.config.final.completo gamecard.config
echo "Levanto Gamecard en segundo plano"
1>/dev/null 2>/dev/null     ../gamecard/Debug/gamecard &
sleep $sleep_time


echo "\nEjecutando script new_pokemon_antes_team.sh (NEW_POKEMON -> Broker)"
../gameboy/Debug/gameboy BROKER NEW_POKEMON Pikachu 9 9 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Squirtle 5 2 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Jolteon 2 2 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Flareon 4 6 1
sleep $(calc $sleep_time \* 4)

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

sleep $sleep_time

echo "\n**  Parte 2: 2 instancias de Team  **\n" 

echo "Cargando configuracion Team 1 de la Cátedra"
cp team.config team.config.auxiliar; rm team.config; cp team.1.config.final.completo team.config
echo "Levanto Team 1 en segundo plano"
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nCargando configuracion Team 2 de la Cátedra"
cp ../team/Debug/team.config ../team/Debug/team.config.auxiliar; rm ../team/Debug/team.config; cp team.2.config.final.completo ../team/Debug/team.config
echo "Levanto Team 2 en segundo plano"
cd ../team/Debug/
1>/dev/null 2>/dev/null     ./team & 
cd ../../tests-de-integracion/
sleep $sleep_time


echo "\nComprobacion: Se hicieron 4 Catch => Se eliminan bloques"
sleep $(calc $ciclo_cpu_time \* 8)

echo
cat ../tall_grass/Files/Pikachu/Metadata.bin
echo
cat ../tall_grass/Files/Squirtle/Metadata.bin
echo
cat ../tall_grass/Files/Jolteon/Metadata.bin
echo
cat ../tall_grass/Files/Flareon/Metadata.bin
echo


sleep $(calc $ciclo_cpu_time \* 20)
echo "\n**  Parte 3: Ejecutando script 'new_pokemon_post_team.sh'  **\n"

../gameboy/Debug/gameboy BROKER NEW_POKEMON Onix 2 8 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Gengar 6 6 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Squirtle 5 5 1

../gameboy/Debug/gameboy BROKER NEW_POKEMON Umbreon 10 6 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Espeon 7 1 1
../gameboy/Debug/gameboy BROKER NEW_POKEMON Vaporeon 4 10 1

sleep $(calc $ciclo_cpu_time \* 40)


echo "\nComprobacion: Ambos team cumplieron objetivo:\n"

tail -10 team.log
echo
tail -10 ../team/Debug/team.log


echo "\n\nEjecutando test de byexample\n"
byexample -l shell catedra-final-completo.md


echo "Restaurando configuracion inicial"
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar
rm ../team/Debug/team.config; cp ../team/Debug/team.config.auxiliar ../team/Debug/team.config; rm ../team/Debug/team.config.auxiliar
#rm *.log

echo "Cerrando procesos anteriores.."
sh mataProcesos.sh 2> /dev/null
