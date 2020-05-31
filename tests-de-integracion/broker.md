# Test de Broker

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                        # byexample: +fail-fast
sleep-time=<sleep-time>
```

Compilo Broker

```shell
$ make clean -C ../broker/Debug; make -C ../broker/Debug      # byexample: +timeout=10 +fail-fast
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
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER NEW_POKEMON pikachu 1 1 1; sleep <sleep-time> # byexample: +timeout=8 +paste
```

Compruebo recepción del mensaje con Código de Operación e ID

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:NEW_POKEMON ID:0
```

## SUSCRIPCIÓN

Suscripción a NEW_POKEMON

(Corregir: debe ser "SUSCRIPTOR" y no SUSCRIPCION)

```bash
$ ../gameboy/Debug/gameboy SUSCRIPCION NEW_POKEMON 10; sleep <sleep-time> # byexample: +timeout=4 +paste
```

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:SUSCRIPCION ID:<...>
<...>[SUSCRIPCION] Cola:NEW_POKEMON
```

## Finalización

Cierro broker (De otro modo el puerto queda sin poder usarse)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste
[<job-id>]+ Term<...>
```