echo Test Team: Final RR

echo
echo Eliminando logs..
rm *.log

echo
echo Cargando configuracion de la Cátedra
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.final team.config

echo
echo Actualizando configuracion a RR
sed -i 's/FIFO/RR/g' team.config

echo
echo Levanto Team en segundo plano:
../team/Debug/team &
