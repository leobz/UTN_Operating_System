## Prueba Base Broker Tamanio minimo particion de 16

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento del proceso broker independiente de los demás enfocándonos en el sistema de particiones dinámicas. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

1) Reiniciar Broker cambiando el tamaño de partición mínima a 16 y ejecutar el script cargar_memoria_complejo.sh

## Prueba Tamanio minimo de particion 16

1) Se verifica que se creen las particiones adecuadamente administrando los datos como indica el Anexo. Verificar por medio del log las posiciones deben quedar de la siguiente manera:
Tamaño mínimo de partición 16: Posiciones 0, 19, 39, 55, 71, 92, 114, 130

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 1 en cache en posicion 0<...>
<...>Almacenando mensaje con ID 2 en cache en posicion 19<...>
<...>Almacenando mensaje con ID 3 en cache en posicion 39<...>
<...>Almacenando mensaje con ID 4 en cache en posicion 55<...>
<...>Almacenando mensaje con ID 5 en cache en posicion 71<...>
<...>Almacenando mensaje con ID 6 en cache en posicion 92<...>
<...>Almacenando mensaje con ID 7 en cache en posicion 114<...>
<...>Almacenando mensaje con ID 8 en cache en posicion 130
```