#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Team Catedra Final: SJF Sin Desalojo  **\n\n\n"


echo "Cargando archivos de configuracion"
rm *.log
cp configuraciones/team.config.catedra.final team.config

echo "Actualizando ALGORITMO_PLANIFICACION=SJF-SD"
sed -i 's/FIFO/SJF-SD/g' team.config

echo "Levantando Team"
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nEjecutar 'prueba_final_team.sh'?"
read "rta"
sh delibird-pruebas/prueba_final_team.sh

sleep $(expr $sleep_time \* 40)


echo "\nComprobacion: Cumplio objetivo Global:\n"
tail team.log

echo "\nEjecutar test?\n"
read "rta"
byexample -l shell byexample-pruebas/team-catedra-final-sjf.md

# Restaurando configuracion inicial
rm team.config;
