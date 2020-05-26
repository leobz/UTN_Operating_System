## Instalar byexample

1) Instalar Pip
```
$ sudo apt-get install python3-pip
```

2) Instalar Byexample
```
$ pip3 install byexample
```

3) Comprobar version
```
$ byexample -V
```


## Correr los tests
Para ejecutar el test de team ejecutar
```
$ byexample -l shell team.md
```

En caso de querer ejecutar otro test, solo cambiar el nombre del archivo test.

## Como funciona byexample
En general se **crean** los test de la siguiente manera:

(Este seria el interior de un archivo test_ejemplo.md)
```
 Comentarios del ejemplo 1 (Que comportamiento quiero)

 ```
 > Codigo del ejemplo 1
 Resultado Esperado del ejemplo 1
 ```


 Comentarios del ejemplo 2 (Que comportamiento quiero)

 ```
 > Codigo del Ejemplo 2
 Resultado Esperado del ejemplo 2
 ```
```

Y se ejecutan de esta:

```
$ byexample -l <lenguaje> <nombre-del-test>.md
```

## Crear un test de Shell


Para ver un ejemplo mas completo ver el siguiente [enlace](https://kilobiter.wordpress.com/2020/05/13/crear-tests-de-consolashell-con-byexample/)

```
$ cat test_shell.md

 Testeo que echo imprima en consola #Comento el comportamiento

 ``` 				# Empiezo el ejemplo (```)
 $ echo HolaMundo!  # En shell se usa ($) para poner comandos
 HolaMundo!			# El resultado esperado se pone abajo.
 ```				# Finalizo el ejemplo (```)
```

**Correr el test:**  

```
$ byexample -l shell test_shell.md

 [PASS] Pass: 1 Fail: 0 Skip: 0

```
Verificar que el estado de la izquierda siempre sea [PASS], si no lo es, buscar cual es el problema o contactar con algun integrante del equipo que nos de una mano.

## Crear un test de Python


```
$ cat test_python.md

 Testeo que la funcion f() retorne 10

 ```
 >>> def f():
 ...    return 10 
 >>> f()
 10
 ```

```

**Correr el test:**  

```
$ byexample -l python test_python.md

 [PASS] Pass: 1 Fail: 0 Skip: 0
```

### En resumen
Byexample es ideal para las personas que desan testear y documentar. Para compartir conocimiento y para asegurarse siempre, que el comportamiento que queremos transmitir funciona.


Links que explican mas en profundidad el funcionamiento:


[Como hacer Test en Shell con Byexample](https://kilobiter.wordpress.com/2020/05/13/crear-tests-de-consolashell-con-byexample/)

[Pagina oficial de Byexample (con documentacion)](https://byexamples.github.io/byexample/)  

