echo
echo Ejecutando test de byexample
# byexample -l shell gamecard-catedra-final-2.md

echo
echo Restaurando configuracion inicial
rm gamecard.config; cp gamecard.config.auxiliar gamecard.config; rm gamecard.config.auxiliar

echo
echo Cerrando procesos anteriores..
sh mataProcesos.sh
