# Simulador de Carrera de Autos

Este es un simulador de carrera de autos multihilo en C++. Cada auto se mueve en un orden aleatorio en cada turno,
y el programa muestra el orden de llegada de los autos al final de la carrera.

## Requisitos

- Un compilador de C++ compatible con C++11 o superior (por ejemplo, `g++`).
- Un sistema operativo compatible con POSIX (por ejemplo, Linux).

## Instrucciones de Compilación

Para compilar el programa, sigue estos pasos:

1. Abre una terminal y navega al directorio donde se encuentra el archivo `main.cpp`.
2. Ejecuta el siguiente comando para compilar el programa:

    make

## Instrucciones de Ejecución

Para ejecutar el programa, sigue estos pasos:

1. Abre una terminal y navega al directorio donde se encuentra el ejecutable `main`.
2. Ejecuta el programa con los argumentos necesarios:

    ./main <Distancia total> <Numero de autos>

    Por ejemplo, para ejecutar una carrera de 100 metros con 3 autos, usa el siguiente comando:

    ./main 100 3

## Ejemplo de Salida

Al ejecutar el programa, verás una salida similar a la siguiente:

Distancia total carrera: 100 metros
Numero de autos: 3
_______________________________________
Auto0 avanza 5 metros. (Total: 5 metros)
Auto1 avanza 7 metros. (Total: 7 metros)
Auto2 avanza 3 metros. (Total: 3 metros)
...
Auto0 avanza 10 metros y termina la carrera en el lugar 1
Auto1 avanza 8 metros y termina la carrera en el lugar 2
Auto2 avanza 6 metros y termina la carrera en el lugar 3
Fin de la Carrera 🏁

Lugar | Auto
-------------
1       auto0
2       auto1
3       auto2