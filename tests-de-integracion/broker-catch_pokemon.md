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

## SUSCRIPCIÓN

Suscripción a CATCH_POKEMON

```  bash
$ ../gameboy/Debug/gameboy SUSCRIPCION CATCH_POKEMON 200 & # byexample: +timeout=4 +paste
[<job-id-gameboy>] <pid-gameboy>
```


```bash
$ sleep <sleep-time>; cat broker.log    # byexample: +timeout=20 +paste
<...>[SUSCRIPCION] Cola:CATCH_POKEMON
<...>
```

## CATCH_POKEMON

Envio el mensaje

```bash
$ sleep <sleep-time>; ../gameboy/Debug/gameboy BROKER CATCH_POKEMON Pikachu 6 6 10; sleep <sleep-time> # byexample: +timeout=20 +paste
Insertando mensaje Catch
Extrayendo mensaje CATCH_POKEMON
Estoy por desencolar proceso CATCH_POKEMON
Empaquetando mensaje CATCH_POKEMON a enviar
```

Compruebo recepción del mensaje con Código de Operación e ID

```bash
$ cat broker.log
<...>[CONEXION] COD_OP:CATCH_POKEMON ID:10
<...>
```

## Finalización

Cierro broker (De otro modo el puerto queda sin poder usarse)

```bash
$ rm *.log; kill <pid-gameboy> ; kill <pid> ; sleep <sleep-time>     # byexample: +timeout=10 +norm-ws +paste -skip
<...>Term<...>broker<...>
<...>Term<...>gameboy<...>
```
