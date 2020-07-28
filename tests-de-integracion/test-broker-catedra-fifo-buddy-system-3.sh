echo
echo Ejecutando test de byexample
byexample -l shell broker-catedra-final-fifo-buddy-system.md

echo
echo Restaurando configuracion inicial
rm broker.config; cp broker.config.auxiliar broker.config; rm broker.config.auxiliar
rm *.log

echo
echo Cerrando procesos anteriores..
sh mataProcesos.sh 2> /dev/null

