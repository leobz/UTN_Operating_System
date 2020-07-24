echo Test Team: Final SJF CD

echo "  Eliminando logs.."
rm *.log

echo "  Cargando configuracion de la Cátedra"
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.final team.config

echo "  Actualizando configuracion a SJF CD"
sed -i 's/FIFO/SJF-CD/g' team.config

echo
echo "  Levanto Team en segundo plano"
../team/Debug/team &
