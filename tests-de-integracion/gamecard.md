# Test de Gamecard

## Inicializacion

Cargo configuración del test

```shell
$ cat byexample.config                                        # byexample: +fail-fast
sleep-time=<sleep-time>
```

Levanto el Gamecard en segundo plano

```bash
$ ../gamecard/Debug/gamecard &
[<job-gamecard-id>] <gamecard-pid>
```

- - - - - - - - - - - - -

## Finalización

Cerramos el proceso Gamecard (de otra manera el puerto quedara en uso)

```bash
$ rm *.log; kill %% ; wait                    # byexample: +timeout=4 +norm-ws +paste -skip
[<job-id>]+ Term<...>
```
