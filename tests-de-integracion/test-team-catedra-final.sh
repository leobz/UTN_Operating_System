#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Team Catedra Final  **\n\n\n"


echo "Ejecutando 'sh test-team-catedra-final.sh'"

rm *.log
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.final team.config
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nEjecutar 'test-team-catedra-final-2.sh'?"
read "rta"
sh test-team-catedra-final-2.sh
sleep $(expr $sleep_time \* 35)


echo "\nComprobacion: Cumplio objetivo Global:\n"
tail team.log


echo "\nEjecutar test?\n"
read "rta"
byexample -l shell team-catedra-final-fifo.md

# Restaurando configuracion inicial
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar

1>/dev/null 2>/dev/nul         sh mataProcesos.sh