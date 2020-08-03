#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Team Catedra Final SJF Con Desalojo  **\n\n\n"


echo "Ejecutando 'sh test-team-catedra-final-rr.sh'"

rm *.log
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.final team.config

echo "Actualizando configuracion a SJF CD"
sed -i 's/FIFO/SJF-CD/g' team.config

1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nEjecutar 'test-team-catedra-final-sjf-cd-2'?"
read "rta"
sh test-team-catedra-final-sjf-cd-2.sh
sleep $(expr $sleep_time \* 35)


echo "\nComprobacion: Cumplio objetivo Global:\n"
tail team.log


echo "\nEjecutar test?\n"
read "rta"
byexample -l shell team-catedra-final-sjf-cd.md

# Restaurando configuracion inicial
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar
