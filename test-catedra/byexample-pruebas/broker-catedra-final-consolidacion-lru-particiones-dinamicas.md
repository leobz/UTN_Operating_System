## Prueba Final Broker Consolidacion LRU Particiones Dinamicas

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la consolidación de particiones dinámicas del proceso broker independiente de los demás. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

4) Bajar sistema, cambiar algoritmo de reemplazo a LRU y volverlo a levantar ejecutando el script.
5) Al ejecutar la suscripción se actualizan los tiempos de LRU de los primeros dos mensajes por lo que se deben borrar los dos siguientes (Catch Pikachu y Catch Squirtle) guardándose posteriormente el nuevo mensaje en la posición 8.

## Prueba LRU

2) Validar el funcionamiento de la consolidación para el algoritmo de reemplazo LRU.


```bash
$ cat broker.log | grep "PARTICIONES DINAMICAS"
<...>Eliminación de partición con posición de inicio: 8<...>
<...>Eliminación de partición con posición de inicio: 27<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 6 en cache en posicion 8
```
