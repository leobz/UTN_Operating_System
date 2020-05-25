# Test de Broker

## Inicializacion

Borrar Logs anteriores y levanto el Broker en segundo plano

```bash
$ rm broker.log ; ../broker/Debug/broker &
<...>[<job-id>] <pid>
```

- - - - - - - - - - - - -

## NEW_POKEMON

Envio el mensaje

```bash
$ sleep 3; ../gameboy/Debug/gameboy BROKER NEW_POKEMON pikachu 1 1 1; sleep 3 # byexample: +timeout=8
```

Compruebo recepción del mensaje con Código de Operación e ID

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:NEW_POKEMON ID:0
```

## SUSCRIPCIÓN

Suscripción a NEW_POKEMON

(Corregir: debe ser "SUSCRIPTOR" y no SUSCRIPCION -  Se pone NEW_POKEMON no cero)

```bash
$ ../gameboy/Debug/gameboy SUSCRIPCION 0 10; sleep 3 # byexample: +timeout=4 
```

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:SUSCRIPCION ID:0
<...>[SUSCRIPCION] Cola:NEW_POKEMON
```

## Finalización

Cierro broker (De otro modo el puerto queda sin poder usarse)

```bash
$ kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```
