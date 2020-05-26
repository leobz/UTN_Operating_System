# Test de Broker

## Inicializacion

Compilo Broker

```shell
$ make clean -C ../broker/Debug; make -C ../broker/Debug      # byexample: +timeout=10
<...>Finished building target: broker<...>
```

Borro Logs anteriores y levanto el Broker en segundo plano

```bash
$ rm *.log ; ../broker/Debug/broker &
<...>[<job-id>] <pid>
```

- - - - - - - - - - - - -

## NEW_POKEMON

Envio el mensaje

```bash
$ ../gameboy/Debug/gameboy BROKER NEW_POKEMON pikachu 1 1 1; sleep 0.1 # byexample: +timeout=8
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
$ ../gameboy/Debug/gameboy SUSCRIPCION 0 10; sleep 0.1 # byexample: +timeout=4 
```

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:SUSCRIPCION ID:0
<...>[SUSCRIPCION] Cola:NEW_POKEMON
```

## Finalización

Cierro broker (De otro modo el puerto queda sin poder usarse)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```
