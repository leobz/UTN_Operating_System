## Inicializacion
Borrar logs anteriores
```
$ rm broker.log
<...>
```

Levantar broker y pasarlo a segundo plano
```
$ ../broker/Debug/broker &
[<job-id>] <pid>
```

## Envio de mensajes

NEW: pikachu en posicion (1,1) cantidad: 1

```
$ ../gameboy/Debug/gameboy BROKER NEW pikachu 1 1 1

```

Comprobamos que el CODIDO_DE_OPERACION es NEW = 3

```
$ cat broker.log 
<...>Mensaje recibido con codigo_de_mensaje:  3
```


## Finalizacion
Cierro broker (De otro modo el puerto queda sin poder usarse)

```
$ kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```

