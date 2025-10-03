## Práctica 1 - Sistemas Distribuidos

Ejemplos cliente-servidor usando sockets en C y Java.

### Contenido

- Ejercicio 1 (saludo por línea):
  - `ServidorSaludo.java`: servidor que recibe una línea y responde "Hola que tal".
  - `cliente_saludo.c`: cliente en C que envía una línea terminada en `\n` y muestra la respuesta.

- Ejercicio 2 (incremento de enteros):
  - `servidor_incremento.c`: servidor en C que recibe enteros (uno por línea) y responde con `n+1`.
  - `ClienteIncremento.java`: cliente en Java que envía enteros por consola y muestra `n+1`. Finaliza al enviar `0`.

### Requisitos

- Compilador de C (clang o gcc)
- JDK (javac y java)
- macOS o Linux

### Compilación y ejecución

#### 1) Ejercicio de saludo

Servidor (Java):
```bash
javac ServidorSaludo.java
java ServidorSaludo 5000
```

Cliente (C):
```bash
cc -Wall -Wextra -O2 -o cliente_saludo cliente_saludo.c
./cliente_saludo 127.0.0.1 5000 "Hola"
```

Notas:
- El servidor atiende un cliente a la vez y cierra la conexión tras responder.
- El cliente envía una línea terminada en `\n` (requerido por `readLine()` en Java).

#### 2) Ejercicio de incremento de enteros

Servidor (C):
```bash
cc -Wall -Wextra -O2 -o servidor_incremento servidor_incremento.c
./servidor_incremento 6000
```

Cliente (Java):
```bash
javac ClienteIncremento.java
java ClienteIncremento 127.0.0.1 6000
```

Flujo de uso (cliente):
- Ingresa un entero y presiona Enter.
- El servidor responde con `n+1`.
- Repite las veces que quieras.
- Al ingresar `0`, el servidor responde y luego cierra la conexión; el cliente termina.
