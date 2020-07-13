# Test Team con Round Robin

1) Se envian 2 Pokemons a Team
2) Se planifican dos entrenadores, que pasan a la cola de Ready
3) Se ejecuta el primero, finaliza por quantum y entra el segundo
4) Se ejecuta el segundo, como la rafaga es menor al quantum, finaliza.
5) Se vuelve a ejecutar el primero y finaliza

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                                  # byexample: +fail-fast
sleep-time=<sleep-time>
```

Modifico Archivo de configuracion para que el algoritmo sea Round Robin y no FIFO

```bash
$ sed -i 's/FIFO/RR/g' team.config
```

Modifico Archivo de configuracion para que el retardo del cpu no sea 0, de otro modo, el primer entrenador realizaria todas sus instrucciones antes del envio del 2do pokemon

```bash
$ sed -i 's/RETARDO_CICLO_CPU=0/RETARDO_CICLO_CPU=1/g' team.config
```

Levanto Team en segundo plano.

```bash
$ ../team/Debug/team &
[<job-id>] <pid>
```

## Precondiciones

En el **Objetivo Global** hay: **3 Pikachus**

Posición entrenadores: TID 0:(1,1) TID 1:(3,3) TID 2:(5,5)
- - -

### Envio pokemones a Team

Envío Pikachu en posicion (6,6), el mas cercano es el TCB 2

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 6 6 ; # byexample: +timeout=4 +paste +fail-fast
<...>
```

Luego envio otro Pikachu (1,1). El TID 0 se encuentra en la misma posicion por lo que va a capturarlo.

```bash
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 1 1  # byexample: +timeout=4 +fail-fast
<...>
```

### Test de Round Robin

Comprobamos que llegaron los mensajes y los entrenadores pasan a ready:

```bash
$ sleep <sleep-time> ; cat team.log  # byexample: +timeout=4 +fail-fast +paste
<...>[CAMBIO DE COLA] TID:2 (New->Ready) (5, 5) Motivo:CAPTURA<...>
<...>
<...>[CAMBIO DE COLA] TID:0 (New->Ready) (1, 1) Motivo:CAPTURA<...>
<...>
```

a) El entrenador mas cercano al primer pokemon (6,6) es el que tiene TID 2, asi que se ejecuta  
b) Cómo la ráfaga es del entrenador con TID 2 es de 3 instrucciones y el Quantum es de 2, el tcb no termina de ejecutar toda su ráfaga, y pasará a Ready con una instrucción pendiente.  
c) El entrenador con TID 0 pasa a ejecutar. Va a atrapar al pokemon (1,1), y como ya esta en la posicion, ejecuta la instruccion Catch, y finaliza  
d) El entrenador con TID 2 vuelve a ejecutarse, realiza su catch y finaliza

```bash
$ sleep 4; cat team.log  # byexample: +timeout=20 +fail-fast
<...>
<...>[INSTRUCCION] TID:2, MOVIMIENTO Posición:(6, 5)<...>
<...>[INSTRUCCION] TID:2, MOVIMIENTO Posición:(6, 6)<...>
<...>[CAMBIO DE COLA] TID:2 (Exec->Ready) (6, 6) Motivo:QUANTUM
<...>[INSTRUCCION] TID:0, CATCH Pikachu 1 1<...>
<...>[INSTRUCCION] TID:2, CATCH Pikachu 6 6<...>
```

## Finalizacion

Cerramos el proceso Team (de otra manera el puerto quedara en uso)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>]+ Term<...>
```

Restauro Archivo de configuración a su estado original

```bash
$ sed -i 's/RR/FIFO/g' team.config           # byexample: +timeout=4 +norm-ws +paste -skip
```

```bash
$ sed -i 's/RETARDO_CICLO_CPU=1/RETARDO_CICLO_CPU=0/g' team.config  # byexample: +timeout=4 +norm-ws +paste -skip
```
