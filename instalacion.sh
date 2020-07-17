echo Actualizando Makefiles
python3 actualizar_makefile.py

echo
echo Compilando Utils
make clean -C utils/Debug 2>> error.log  1>> instalacion.log
make -C utils/Debug 2>> error.log  1>> instalacion.log

echo
echo Compilando Gameboy
make clean -C gameboy/Debug 2>> error.log  1>> instalacion.log
make -C gameboy/Debug 2>> error.log  1>> instalacion.log

echo
echo Compilando Broker
make clean -C broker/Debug 2>> error.log  1>> instalacion.log
make -C broker/Debug 2>> error.log  1>> instalacion.log

echo
echo Compilando Team
make clean -C team/Debug 2>> error.log  1>> instalacion.log
make -C team/Debug 2>> error.log  1>> instalacion.log

echo
echo Compilando Gamecard
make clean -C gamecard/Debug 2>> error.log  1>> instalacion.log
make -C gamecard/Debug 2>> error.log  1>> instalacion.log