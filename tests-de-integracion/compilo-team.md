Cargo configuración del test

```shell
$ cat byexample.config             # byexample: +fail-fast
sleep-time=<sleep-time>
```

Limpio compilación anterior

```shell
$ make clean -C ../team/Debug     # byexample: +timeout=10 +fail-fast
<...>
```

Compilo Broker

```bash
$ make -C ../team/Debug           # byexample: +timeout=10 +fail-fast
<...>Finished building target: team<...>
```
