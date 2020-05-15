
## Inicializacion
Eliminamos Logs antiguos
```
$ rm team.log

```

Inicializamos el modulo Team  
Lo enviamos a segundo plano  
Guardamos su PID
```
$ ../team/Debug/team & 
[<job-id>] <pid>
```

En el objetivo Global hay:
  3 Pikachus
  2 Squirtles

## Tests
Envio de mensajes a TEAM

### APPEARED_POKEMON

### Pokemon Pertenece a Objetivo Global se agrega al Mapa
Enviamos un Pikachu en la Posicion (1,1).
Como Pikachu pertenece al Objetivo global se agregara al mapa. 

```
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 1 1
[AGREGADO]: Pikachu 1 1 [TOTAL]: 1
```

```
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 1 1
```

### Pokemon Agregado aumenta el total
Enviamos otro Pikachu en la Posicion (2,2).
Como Pikachu pertenece al Objetivo global se agregara al mapa  
y su cantidad aumenta.
```
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Pikachu 2 2
[AGREGADO]: Pikachu 2 2 [TOTAL]: 2
```

```
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Pikachu 2 2
```

### Pokemon que no esta en el Objetivo Global no se agrega
Enviamos un Charmander en la Posicion (1,1).
Como Charmander no pertenece al Objetivo global no se agrega  
ni se muestra.
```
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON Charmander 1 1

```

```
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: Charmander 1 1
```

## Finalizacion
Cerramos el proceso Team (de otra manera el puerto quedara en uso)
```
$ kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```

