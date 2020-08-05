#!/bin/sh

sleep_time=2


echo "\n\n\n  **  Test Team Espera circular  **\n\n\n"

echo Cargando configuracion de la Cátedra
rm *.log
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.espera.circular team.config

echo Levanto Team en segundo plano:
1>/dev/null 2>/dev/null     ../team/Debug/team &


echo "\nEjecutar 'test-espera-circular-2.sh'?"
read "rta"
sh test-espera-circular-2.sh
sleep $(expr $sleep_time \* 3)

echo "\nComprobacion: Detecto deadlock entre 5 TCBs:\n"
cat team.log | grep "Se detectó espera circular entre  5 TCBs"

echo "\nComprobacion: Cumplio objetivo Global:\n"
tail team.log


echo "\nCorrer test?\n"
read "rta"
byexample -l shell team-catedra-espera-circular.md
