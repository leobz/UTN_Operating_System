#!/bin/sh

sleep_time=2


echo "\n\n\n  **  Test Team Espera circular  **\n\n\n"

echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/team.config.catedra.espera.circular team.config

echo "Levantando Team"
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nEjecutar 'espera_circular_basico.sh'?"
read "rta"
sh delibird-pruebas/espera_circular_basico.sh
sleep $(expr $sleep_time \* 3)

echo "\nComprobacion: Detecto deadlock entre 5 TCBs:\n"
cat team.log | grep "Se detectó espera circular entre  5 TCBs"

echo "\nComprobacion: Cumplio objetivo Global:\n"
tail team.log

echo "\nCorrer test?\n"
read "rta"
byexample -l shell byexample-pruebas/team-catedra-espera-circular.md

# Restaurando configuracion inicial
rm team.config;

