# Como mandar la señal SIGUSR1 al broker

## 1) Comando kill

```
kill -SIGUSR1 <PID>
```

## 2) Usando htop

```
* Desde la consola, escribir el comando htop.
* Buscar el proceso broker en base al PID.
* Seleccionar ese proceso.
* Apretar F9.
* Elegir la señal SIGUSR1.
* Presionar Enter.
* Salir con F10.
```

**Una vez mandada la señal, se crea el archivo dump_cache.txt con la información solicitada.**

*NOTA: Al iniciar el broker, se guarda el PID del mismo en un archivo llamado pid.txt*