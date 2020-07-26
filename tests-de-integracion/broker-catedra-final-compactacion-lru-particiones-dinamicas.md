## Prueba Final Broker Compactación LRU Particiones Dinamicas

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la compactación de particiones dinámicas del proceso broker independiente de los demás. Para esto, será necesario la ejecución del proceso broker y de scripts proporcionados por la cátedra.

## Actividades:

1) Bajar sistema, cambiar algoritmo de reemplazo a LRU y volverlo a levantar ejecutando el script.
2) Al ejecutar la suscripción se actualizan los tiempos de LRU de los primeros dos mensajes por lo que se deben borrar el mensaje Catch Pikachu. Al compactar se guarda el mensaje nuevo en la posición 44.



## Prueba LRU

1) Validar el funcionamiento de la compactación para el algoritmo de reemplazo LRU.


```bash
$ cat broker.log | grep "PARTICIONES DINAMICAS"
<...>Eliminación de partición con posición de inicio: 8<...>
<...>Compactación de particiones<...>
```

```bash
$ cat broker.log | grep "Almacenando mensaje"
<...>Almacenando mensaje con ID 6 en cache en posicion 44