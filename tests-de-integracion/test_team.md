
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

TODO: Por defecto hay 2 pikachus hardCodeados en el objetivo global.
Borrarlos cuando se puedan cargar por config

## Tests
Envio de mensajes a TEAM

### APPEARED_POKEMON

Enviamos un Pikachu en la Posicion (1,1).
Como Pikachu pertenece al Objetivo global se agregara al mapa. 

```
$ ../gameboy/Debug/gameboy TEAM APPEARED_POKEMON pikachu 1 1
[AGREGADO]: pikachu 1 1 [TOTAL]: 1
```

```
$ cat team.log
<...>[MSG_RECIBIDO] APPEARED_POKEMON: pikachu 1 1
```

## Finalizacion
Cerramos el proceso Team (de otra manera el puerto quedara en uso)
```
$ kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```

