python3 actualizar_makefile.py

echo
echo Compilo Utils
make clean -C utils/Debug 2>> error.log  1>> error.log
make -C utils/Debug 2>> error.log  1>> error.log

echo
echo Compilo Gameboy
make clean -C gameboy/Debug 2>> error.log  1>> error.log
make -C gameboy/Debug 2>> error.log  1>> error.log

echo
echo Compilo Broker
make clean -C broker/Debug 2>> error.log  1>> error.log
make -C broker/Debug 2>> error.log  1>> error.log

echo
echo Compilo Team
make -C team/Debug 2>> error.log  1>> error.log
make -C team/Debug 2>> error.log  1>> error.log