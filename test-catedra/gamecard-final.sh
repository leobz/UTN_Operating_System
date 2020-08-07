sleep_time=1

echo  "\n  ** Gamecard Catedra: Final  **\n"

echo "Cargando configuracion de la Cátedra"
cp configuraciones/gamecard.config.catedra.base gamecard.config

echo "Eliminando archivos existentes del FileSystem"
rm -r ../tall-grass/Files/* 
rm ../tall-grass/Blocks/*
rm *.log

# echo "  Actualizando Metadata.bin"
# echo -e "BLOCK_SIZE=64\nBLOCKS=4096\nMAGIC_NUMBER=TALL_GRASS" > ../tall-grass/Metadata/Metadata.bin

echo "Levantando gamecard"
1>/dev/null 2>/dev/null ../gamecard/Debug/gamecard &
sleep 0.1



echo "\nEjecutando 'new_pikachu.sh'"
sh delibird-pruebas/new_pikachu.sh
sleep $sleep_time

echo "Comprobacion: Se creo la carpeta Pikachu y su metadata indica que el tamaño es 7 bytes.\n"
cat ../tall-grass/Files/Pikachu/Metadata.bin



echo "\nEjecutando 'new_pokemons_varios.sh'"
sh delibird-pruebas/new_pokemons_varios.sh
sleep $(expr $sleep_time \* 4)

echo "Comprobacion: El tamaño del archivo Pikachu se haya actualizado a 13 bytes.\n"
cat ../tall-grass/Files/Pikachu/Metadata.bin

echo "\nComprobacion: Se creo la carpeta Charmander y su metadata indique
 que posee dos bloques y su tamaño es 70 bytes\n"
cat ../tall-grass/Files/Charmander/Metadata.bin



echo "\nEjecutando 'catch_charmander.sh'"
sh delibird-pruebas/catch_charmander.sh
sleep $sleep_time

echo "Comprobacion: Verificar que el archivo Charmander ahora indique que posee
solo un bloque y su tamaño es 61 bytes.\n"
cat ../tall-grass/Files/Charmander/Metadata.bin



#COMPROBACION A NIVEL DE LINUX

echo "\n\n  Comprobaciones a nivel FileSystem de Linux\n\n"
echo "Verificar que ejecutando el script pikachu (una única sentencia), el file system cree  el archivo y asigne correctamente los bloques y bytes.
Verificar que al realizar múltiples NEW sobre un mismo pokémon, el archivo crezca en tamaño de bloques.
La asignación de bloques se realice correctamente.
Verificar que al realizar catch se libere espacio en el archivo.
Verificar que al liberar espacio suficiente, se liberen los bloques innecesarios\n"

ls -l ../tall-grass/Blocks/



# TEST BYEXAMPLE
byexample -l shell byexample-pruebas/gamecard-catedra-base.md


echo "\nFinalizando Gamecard"
1>/dev/null 2>/dev/null sh mataProcesos.sh

