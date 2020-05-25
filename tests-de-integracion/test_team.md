# Test Team

## Inicializacion

Eliminamos Logs antiguos y levantamos Team en segundo plano.

```bash
$ rm team.log; ../team/Debug/team &
<...>[<job-id>] <pid>
```

## Tests

En el **Objetivo Global** hay: **3 Pikachus** y **2 Squirtles**

### APPEARED_POKEMON

- - -

#### Pokemon Pertenece a Objetivo Global -> Se agrega al Mapa

Envío Pikachu en posicion (1,1).  
Como pertenece al Objetivo global lo agrego al mapa.

```bash
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 1 1; sleep 3 # byexample: +timeout=4 
[AGREGADO]: Pikachu 1 1 [TOTAL]: 1
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 1 1
```

Envio otro Pikachu (2,2).
Como ya habia uno, la cantidad aumenta a dos.

```bash
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 2 2; sleep 3 # byexample: +timeout=4 
[AGREGADO]: Pikachu 2 2 [TOTAL]: 2
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 2 2
```

#### Pokemon No Pertenece a Objetivo Global -> No se agrega

Envío Charmander.  
Como no pertenece al Objetivo Global no se agrega.

```bash
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Charmander 1 1; sleep 3 # byexample: +timeout=4 
```

```bash
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Charmander 1 1
```

## Finalizacion

Cerramos el proceso Team (de otra manera el puerto quedara en uso)

```bash
$ kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```
