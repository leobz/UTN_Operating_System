# Tests de Integración

## Instalar entorno

1) Instalar Pip

```bash
$ sudo apt-get install python3-pip
```

2) Instalar Byexample

```bash
$ pip3 install byexample
```

3) Comprobar version

```bash
$ byexample -V
```

## Configurar tiempo de espera (Importante)

A raiz de que cada computadora es distinta en cuanto a velocidad de procesamiento,
algunas necesitan mas tiempo para procesar los distintos modulos.
Por esto se debera crear un archivo "byexample.config" que tenga el siguiente contenido:

$ cat byexample.config

```
sleep-time=0.05
```

0.05 es un valor de ejemplo, es el valor que el motor de test espera entre cada comando a ejecutar, en las maquinas virtuales puede llegar a ser de hasta 3 segundos,
usted puede ir probando valores para ver cual es su tiempo de espera ideal.

## Diferencia entre archivos .sh y .md

Los .sh son scripts que compilan los módulos y además corren uno o más tests de byexample. Piense en ellos como si fuesen un wrapper que automatiza todo el proceso de testeo.

Los .md son archivos de test de byexample, en ellos puede **agregar o modificar los tests** que correran los scripts .sh.

## Correr Regresion

Este script compila y corre los tests de todos los módulos

```bash
$ sh regresion.sh
```

## Correr un Test Particular

Puede compilar y correr un módulo en especial (Broker, Team, etc).

Por defecto, Utils y Gameboy siempre se van a compilar sea cual sea el test que se elija.

Ejemplo:

```bash
$ sh team.sh
```

## Correr los tests desde byexample

En caso de que quiera testear algo y no necesite o quiera compilarlo desde el script .sh, puede realizar la compilacion a mano y luego correr el test por separado.

Si desea ejecutar el test de byexample, de por ejemplo team, debe ejecutar:

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

