#!/bin/sh

ciclo_cpu_time=0.2
sleep_time=0.5


echo "\n**  Test Final Prueba completa	**\n" 


echo "Eliminando archivos anteriores"
rm -r ../tall_grass/Files/*     2>/dev/null
rm ../tall_grass/Blocks/*       2>/dev/null
rm *.log                        2>/dev/null
rm ../team/Debug/*.log          2>/dev/null






echo "\n\n**  Parte 1: Particiones, FIFO y RR  **\n\n" 

echo "Cargando configuracion Broker de la Cátedra"
cp configuraciones/broker.config.final.completo broker.config

echo "Cargando configuracion Gamecard de la Cátedra"
cp configuraciones/gamecard.config.final.completo gamecard.config

echo "Cargando configuracion Team 1 y Team 2 de la Cátedra"
cp configuraciones/team.1.config.final.completo team.config
cp configuraciones/team.2.config.final.completo ../team/Debug/team.config

echo "\nLevantando Broker"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep $sleep_time

echo "Levantando Gamecard"
1>/dev/null 2>/dev/null     ../gamecard/Debug/gamecard &
sleep $sleep_time

echo "\nEjecutando 'new_pokemon_antes_team.sh'"
sh delibird-pruebas/new_pokemon_antes_team.sh

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

sh test-catedra-final-completo-parte-team.sh


echo "\n\nBajando broker"
1>/dev/null 2>/dev/null   kill `ps -ef|grep -v grep |grep "../broker/Debug/broker"| awk '{print $2}'`

echo "Eliminando logs de broker y teams"
rm broker.log;
rm team.log;
rm ../team/Debug/team.log;





echo "\n\n*  Parte 2: Particiones, FIFO y RR  **\n\n" 


echo "Actualizando configuracion Broker ALGORITMO_MEMORIA=BS"
sed -i 's/ALGORITMO_MEMORIA=PARTICIONES/ALGORITMO_MEMORIA=BS/g' broker.config 


echo "Actualizando configuracion Team 1 ALGORITMO_PLANIFICACION=SJF-SD"
sed -i 's/ALGORITMO_PLANIFICACION=FIFO/ALGORITMO_PLANIFICACION=SJF-SD/g' team.config 

echo "Actualizando configuracion Team 2 ALGORITMO_PLANIFICACION=SJF-CD"
sed -i 's/ALGORITMO_PLANIFICACION=RR/ALGORITMO_PLANIFICACION=SJF-CD/g' ../team/Debug/team.config 


echo "Levantando Broker"
1>/dev/null 2>/dev/null     ../broker/Debug/broker &
sleep $sleep_time


echo "\nEjecutando 'new_pokemon_antes_team.sh'"
sh delibird-pruebas/new_pokemon_antes_team.sh

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


sh test-catedra-final-completo-parte-team.sh











echo "Restaurando configuracion inicial"
rm broker.config;
rm gamecard.config;
rm team.config;
rm ../team/Debug/team.config;


echo "Cerrando procesos anteriores.."
sh mataProcesos.sh 2> /dev/null


