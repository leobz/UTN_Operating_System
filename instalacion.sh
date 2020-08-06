echo
echo Descargando so-commons-library...
git clone https://github.com/sisoputnfrba/so-commons-library.git

echo
echo Instalando so-commons-library...
sudo make install -C so-commons-library 2>> error.log  1>> instalacion.log

echo
echo Descargando delibird-pruebas...
cd test-catedra/
git clone https://github.com/sisoputnfrba/delibird-pruebas.git

echo
echo Actualizando path delibird-pruebas dentro de test-catedra/ ...
cd delibird-pruebas/
git checkout .
sed -i 's#./gameboy#../gameboy/Debug/gameboy#g' *.sh
cd ../..

echo
echo Actualizando Makefiles
sh restaurar_makefile_desde_base.sh
python3 actualizar_makefile.py

echo
echo Compilando Utils
make clean -C utils/Debug 2>> error.log  1>> instalacion.log
make -C utils/Debug 2>> error.log  1>> instalacion.log

echo Compilando Gameboy
make clean -C gameboy/Debug 2>> error.log  1>> instalacion.log
make -C gameboy/Debug 2>> error.log  1>> instalacion.log

echo Compilando Broker
make clean -C broker/Debug 2>> error.log  1>> instalacion.log
make -C broker/Debug 2>> error.log  1>> instalacion.log

echo Compilando Team
make clean -C team/Debug 2>> error.log  1>> instalacion.log
make -C team/Debug 2>> error.log  1>> instalacion.log

echo Compilando Gamecard
make clean -C gamecard/Debug 2>> error.log  1>> instalacion.log
make -C gamecard/Debug 2>> error.log  1>> instalacion.log

echo
echo Actualizando LIBRARY_PATH
root_tp=$(pwd)
utils_path="${root_tp}/utils/Debug"
export_command="export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:${utils_path}"
echo $export_command >> $HOME/.bashrc

echo
echo Actualizando ruta tall-grass
mkdir tall-grass
root_tp=$(pwd)
tall_grass_path="${root_tp}/tall-grass"
cd test-catedra/configuraciones
sed -i "/PUNTO_MONTAJE/c PUNTO_MONTAJE_TALLGRASS=$tall_grass_path" gamecard.config.*

echo
echo Fin de la instalacion