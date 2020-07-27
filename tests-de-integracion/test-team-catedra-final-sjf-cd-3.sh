echo
echo Ejecutando test de byexample
byexample -l shell team-catedra-final-sjf-cd.md

echo
echo Restaurando configuracion inicial
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar
rm *.log

echo
echo Cerrando procesos anteriores..
sh mataProcesos.sh 2> /dev/null
