## Prueba Base Team

## Disclaimer:

Esta prueba tiene como objetivo verificar el funcionamiento de la planificación bajo el algoritmo FIFO. Para la misma, se requiere la ejecución de un proceso Team.

## Actividades:

1) Ejecutar el Módulo Team con la configuración especificada.
2) Ejecutar el script catch_pokemon_escenario_1_s1.sh
3) Esperar a que los tres entrenadores se muevan y atrapen los respectivo pokemon.
4) Ejecutar el script catch_pokemon_escenario_1_s2.sh
5) Esperar a que el entrenador B y entrenador C se muevan a atrapar los pokemon.
6) Verificar que se detecten dos deadlocks diferentes y que se resuelvan.

## Resultados Esperados:

1) El entrenador A se mueve a atrapar a Pikachu en posición 1-1.
2) El entrenador C se mueve a atrapar a Squirtle en posición 9-7.
3) El entrenador B se mueve a atrapar a Onix en posición 2-2.
4) El entrenador B se mueve a atrapar a Squirtle en posición 3-5.
5) El entrenador C se mueve a atrapar a Gengar en posición 7-5.
6) Se detecta deadlock entre A-B.
7) Se detecta deadlock entre B-C.
8) Se mueve A o B a la posición del otro e intercambian Pikachu-Squirtle. Finaliza el Entrenador A.
9) Se mueve B o C a la posición del otro e intercambian Onix-Gengar. Finaliza el Entrenador C.
10) Una vez resuelto ambos deadlock, finaliza el entrenador B.
11) Finaliza el proceso Team.

Todos los resultados se validan por medio del archivo log.


# Test

## Movimiento de Captura

1) El entrenador A (0) se mueve a atrapar a Pikachu en posición 1-1.
2) El entrenador C (2) se mueve a atrapar a Squirtle en posición 9-7.
3) El entrenador B (1) se mueve a atrapar a Onix en posición 2-2.
4) El entrenador B (1) se mueve a atrapar a Squirtle en posición 3-5.
5) El entrenador C (2) se mueve a atrapar a Gengar en posición 7-5.

```bash
$ cat team.log | grep "Motivo:CAPTURA"
<...>TID:0 <...> Motivo:CAPTURA de Pikachu (1,1)<...>
<...>TID:2 <...> Motivo:CAPTURA de Squirtle (9,7)<...>
<...>TID:1 <...> Motivo:CAPTURA de Onix (2,2)<...>
<...>TID:1 <...> Motivo:CAPTURA de Squirtle (3,5)<...>
<...>TID:2 <...> Motivo:CAPTURA de Gengar (7,5)<...>
```


## Detección de Deadlocks

6) Se detecta deadlock entre A-B (0-1).
7) Se detecta deadlock entre B-C (1-2).

```bash
$ cat team.log | grep "DEADLOCK"
<...>TID: 1 se moverá a la posición <...> del entrenador TID: 0<...>
<...>TID: 2 se moverá a la posición <...> del entrenador TID: 1<...>
```


## Resolución de Deadlocks

8) Se mueve A o B (0 o 1) a la posición del otro e intercambian Pikachu-Squirtle. 
9) Se mueve B o C (1 o 2) a la posición del otro e intercambian Onix-Gengar. s

```bash
$ cat team.log | grep "Motivo:Entrenador va a intercambiar"
<...>TID:1 (Deadlock->Ready) <...> Motivo:Entrenador va a intercambiar<...>
<...>TID:2 (Deadlock->Ready) <...> Motivo:Entrenador va a intercambiar<...>
```

8) ... Finaliza el Entrenador A (0).
9) ... Finaliza el Entrenador C (2).
10) Una vez resuelto ambos deadlock, finaliza el entrenador B (1) o C(2), ya que se resuelven los dos simultaneamente
    
TODO: Ver porque TID:2 pasa De Ready->Exit y no de Exec->Exit
```bash
$ cat team.log | grep "Exit"
<...>TID:0 (Deadlock->Exit) (1, 1) Motivo:Cumplió Objetivo
<...>TID:1 (Deadlock->Exit) (1, 1) Motivo:Cumplió Objetivo
<...>TID:2 (<...>->Exit) (1, 1) Motivo:Cumplió Objetivo
```

## Finalizacion por cumplir objetivo

11) Finaliza el proceso Team.

```bash
$ cat team.log | grep "FIN"
<...>[FIN DEL PROCESO] ¡Team cumplió objetivo!
```
