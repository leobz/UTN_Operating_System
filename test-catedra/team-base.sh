#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Team Catedra Base  **\n\n\n"


echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/team.config.catedra.base team.config

echo "Levantando Team"
1>/dev/null 2>/dev/null     ../team/Debug/team &
 
echo "\nEjecutar 'catch_pokemon_escenario_1_s1.sh'?"
read "rta"
sh delibird-pruebas/catch_pokemon_escenario_1_s1.sh
sleep $sleep_time

echo "Comprobacion: TID:1 paso a Unblocked:\n"
cat team.log | grep "Exec->Unblocked"

echo "\n\nEjecutar 'catch_pokemon_escenario_1_s2.sh'?"
read "rta"
sh delibird-pruebas/catch_pokemon_escenario_1_s2.sh
sleep $(expr $sleep_time \* 2)

echo "Comprobacion: Cumplio objetivo Global:\n"
tail team.log

echo "\n\nEjecutar test?\n"
read "rta"
byexample -l shell byexample-pruebas/team-catedra-base.md

# Restaurando configuracion inicial
rm team.config;

