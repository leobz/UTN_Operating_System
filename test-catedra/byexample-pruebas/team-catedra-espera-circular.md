## Prueba Team - Espera Circular

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la espera circular bajo el algoritmo RR. Para la misma, se requiere la ejecución de un proceso Team.

## Actividades:

1) Ejecutar el Módulo Team con la configuración especificada.
2) Ejecutar el script espera_circular_basico.sh
3) Esperar que los 5 entrenadores se muevan y atrapen un pokemon.
4) Se debe detectar un deadlock con espera circular entre los 5 y resolverse.

5) Reiniciar el aplicativo cambiando el algoritmo a Quantum 2 y verificar que funcione igual.
6) Reiniciar el aplicativo cambiando el algoritmo a FIFO y verificar que funcione igual.

## Resultados Esperados:

1) El entrenador A se mueve a atrapar a Jolteon en posición 2-2.
2) El entrenador B se mueve a atrapar a Flareon en posición 4-6.
3) El entrenador C se mueve a atrapar a Umbreon en posición 10-6.
4) El entrenador D se mueve a atrapar a Espeon en posición 7-1.
5) El entrenador E se mueve a atrapar a Vaporeon en posición 4-10.
6) Se detecta un deadlock con espera circular entre los 5 entrenadores.
7) Se resuelve el deadlock y finalizan los distintos entrenadores.
8) Finaliza el proceso Team.

# Test

## Movimiento de Captura

1) El entrenador A (0) se mueve a atrapar a Jolteon en posición 2-2.
2) El entrenador B (1) se mueve a atrapar a Flareon en posición 4-6.
3) El entrenador C (2) se mueve a atrapar a Umbreon en posición 10-6.
4) El entrenador D (3) se mueve a atrapar a Espeon en posición 7-1.
5) El entrenador E (4) se mueve a atrapar a Vaporeon en posición 4-10.


```bash
$ cat team.log | grep "Motivo:CAPTURA"
<...>TID:0 <...> Motivo:CAPTURA de Jolteon (2,2)<...>
<...>TID:1 <...> Motivo:CAPTURA de Flareon (4,6)<...>
<...>TID:2 <...> Motivo:CAPTURA de Umbreon (10,6)<...>
<...>TID:3 <...> Motivo:CAPTURA de Espeon (7,1)<...>
<...>TID:4 <...> Motivo:CAPTURA de Vaporeon (4,10)<...>
```


## Detección de Deadlocks
6) Se detecta un deadlock con espera circular entre los 5 entrenadores.
7) Se resuelve el deadlock y finalizan los distintos entrenadores.

```bash
$ cat team.log | grep "DEADLOCK"
<...>Se detectó espera circular entre  5 TCBs<...>
```

## Finalizacion por cumplir objetivo

8) Finaliza el proceso Team.

```bash
$ cat team.log | grep "FIN"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!
```
