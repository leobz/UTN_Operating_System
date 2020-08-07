## Prueba Base Broker Tamanio minimo particion de 4

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento del proceso broker independiente de los demás enfocándonos en el sistema de particiones dinámicas. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

1) Ejecutar script cargar_memoria_simple.sh
2) Verificar a partir del log que se haya creado los mensaje dentro de la memoria en la posiciones 0 y 11.
3) Ejecutar script cargar_memoria_complejo.sh
4) Verificar que la memoria quede en el estado adecuado.


## Prueba Tamanio minimo de particion 4

1) Se verifica que se creen las particiones adecuadamente administrando los datos como indica el Anexo. Verificar por medio del log las posiciones deben quedar de la siguiente manera:
Tamaño mínimo de partición 4: Posiciones 0, 11, 25, 44, 64, 68, 72, 93, 115, 124

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 1 en cache en posicion 0<...>
<...>Almacenando mensaje con ID 2 en cache en posicion 11<...>
<...>Almacenando mensaje con ID 3 en cache en posicion 25<...>
<...>Almacenando mensaje con ID 4 en cache en posicion 44<...>
<...>Almacenando mensaje con ID 5 en cache en posicion 64<...>
<...>Almacenando mensaje con ID 6 en cache en posicion 68<...>
<...>Almacenando mensaje con ID 7 en cache en posicion 72<...>
<...>Almacenando mensaje con ID 8 en cache en posicion 93<...>
<...>Almacenando mensaje con ID 9 en cache en posicion 115<...>
<...>Almacenando mensaje con ID 10 en cache en posicion 124
```