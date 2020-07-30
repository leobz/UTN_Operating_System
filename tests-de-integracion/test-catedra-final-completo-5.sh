echo
echo Ejecutando test de byexample
byexample -l shell catedra-final-completo.md

echo
echo Restaurando configuracion inicial
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar
rm team.config; cp team.config.auxiliar team.config; rm team.config.auxiliar
rm *.log

echo
echo Cerrando procesos anteriores..
sh mataProcesos.sh 2> /dev/null

