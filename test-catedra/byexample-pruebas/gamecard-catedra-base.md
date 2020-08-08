## Prueba Base Gamecard

## Disclaimer:

El objetivo de la prueba es verificar el correcto funcionamiento del File System individualmente. Para esto se requerirá la ejecución de una instancia del Proceso GameCard y la ejecución de scripts que provee la cátedra.

## Configuración del sistema:

Metadata FileSystem 

```
    BLOCK_SIZE=64
    BLOCKS=4096
    MAGIC_NUMBER=TALL_GRASS
```

## Resultados Esperados

El tamaño del archivo Pikachu se haya actualizado a 13 bytes.

```bash
$ cat ../tall-grass/Files/Pikachu/Metadata.bin
OPEN=N
DIRECTORY=N
BLOCKS=[0]
SIZE=13
```

------------------------------------------------------------------------------------------

Verificar que el archivo Charmander ahora indique que posee solo un bloque y su tamaño es 61 bytes.

```bash
$ cat ../tall-grass/Files/Charmander/Metadata.bin
OPEN=N
DIRECTORY=N
BLOCKS=[1]
SIZE=61
```

------------------------------------------------------------------------------------------

Dentro del file system de linux y el punto de montaje, validar:

1) Verificar que ejecutando el script pikachu (una única sentencia), el file system cree  el archivo y asigne correctamente los bloques y bytes.
2) Verificar que al realizar múltiples NEW sobre un mismo pokémon, el archivo crezca en tamaño de bloques.
3) La asignación de bloques se realice correctamente.
4) Verificar que al realizar catch se libere espacio en el archivo.
5) Verificar que al liberar espacio suficiente, se liberen los bloques innecesarios.

```bash
$ ls -l ../tall-grass/Blocks/
total 12
<...> 13 <...> 0.bin
<...> 61 <...> 1.bin
<...>  0 <...> 2.bin
<...> 12 <...> Metadata.bin
```
