## Prueba Final Team

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la planificación y el proceso Team en particular, sin la intervención de los demás módulos. La misma se encargará de verificar los distintos algoritmos de planificación, la detección y resolución de deadlocks.

## Actividades:

1) Iniciar el proceso con el algoritmo FIFO, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
2) Iniciar el proceso con el algoritmo RR con Quantum 2, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
3) Iniciar el proceso con el algoritmo SJF, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
4) Iniciar el proceso con el algoritmo SJF con Desalojo, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice

## Prueba RR

2) El funcionamiento del algoritmo RR debería detectar un deadlock triple entre los procesos
3) Como C finaliza antes que la primer captura de A, el mismo se dirige a atrapar a Gengar generando así un deadlock triple

```bash
$ cat team.log | grep "DEADLOCK"
<...>Se detectó espera circular entre  3 TCBs.<...>
```

```bash
$ cat team.log | grep "Gengar"
<...>TID:2 (Unblocked->Ready)<...>Motivo:CAPTURA de Gengar<...>
```

```bash
$ cat team.log | grep "FIN"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!
```
