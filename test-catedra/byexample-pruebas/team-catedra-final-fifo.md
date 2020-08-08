## Prueba Final Team

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la planificación y el proceso Team en particular, sin la intervención de los demás módulos. La misma se encargará de verificar los distintos algoritmos de planificación, la detección y resolución de deadlocks.

## Actividades:

1) Iniciar el proceso con el algoritmo FIFO, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
2) Iniciar el proceso con el algoritmo RR con Quantum 2, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
3) Iniciar el proceso con el algoritmo SJF, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice
4) Iniciar el proceso con el algoritmo SJF con Desalojo, ejecutar el script prueba_final_team.sh y verificar que el proceso finalice

## Prueba FIFO

1) El funcionamiento del algoritmo FIFO debería detectar un deadlock entre el proceso A y C

```bash
$ cat team.log | grep "DEADLOCK"
<...>Se detectó espera circular entre  2 TCBs.<...>
<...>El entrenador TID: 2<...>TID: 0 para intercambiar<...>
```

```bash
$ cat team.log | grep "FIN"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!
```

2) El funcionamiento del algoritmo RR debería detectar un deadlock triple entre los procesos
3) Como C finaliza antes que la primer captura de A, el mismo se dirige a atrapar a Gengar generando así un deadlock triple
4) El funcionamiento del algoritmo SJF debería ser similar al algoritmo FIFO pero deberían dar distintas métricas.
5) El funcionamiento del algoritmo SJF con Desalojo debería ser similar al SJF pero con distintas  métricas
6) Comparar métricas obtenidas de los distintos algoritmos
## Resolución de Deadlocks


## Finalizacion por cumplir objetivo

