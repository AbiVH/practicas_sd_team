// Cliente TCP simple que habla con el servidor Java (ServidorSaludo.java)
// Uso: ./cliente_saludo <ip_o_host> <puerto> [mensaje]
// Ejemplo: ./cliente_saludo 192.168.1.73 5000 "Hola"
// Este cliente abre una conexión TCP, envía un mensaje terminado en '\n' y
// espera una línea de respuesta del servidor para imprimirla en pantalla.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUFSZ 1024

int main(int argc, char *argv[]) {
    // Valido parámetros de ejecución: host obligatorio, puerto obligatorio, mensaje opcional
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <host> <puerto> [mensaje]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *host = argv[1];
    const char *port = argv[2];
    const char *mensaje = (argc >= 4) ? argv[3] : "Hola";

    // Preparo la información de dirección
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Resuelve el host y puerto a una o más direcciones
    int s = getaddrinfo(host, port, &hints, &res);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return EXIT_FAILURE;
    }

    int sock = -1;
    // Intenta conectarse probando cada resultado hasta lograrlo
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == -1) continue;

        if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
            break; // conectado
        }
        close(sock);
        sock = -1;
    }
    freeaddrinfo(res);

    if (sock == -1) {
        perror("No se pudo conectar");
        return EXIT_FAILURE;
    }

    // Envío del mensaje como UNA línea (el servidor Java usa readLine())
    // Añadimos '\n' al final para que el servidor sepa dónde termina la línea
    size_t len = strlen(mensaje);
    char *linea = malloc(len + 2);
    if (!linea) { perror("malloc"); close(sock); return EXIT_FAILURE; }
    memcpy(linea, mensaje, len);
    linea[len] = '\n';
    linea[len+1] = '\0';

    ssize_t n = send(sock, linea, len + 1, 0);
    free(linea);
    if (n == -1) {
        perror("send");
        close(sock);
        return EXIT_FAILURE;
    }

    // Recibo la respuesta del servidor leyendo hasta encontrar '\n' o cerrar
    char buf[BUFSZ];
    size_t pos = 0;
    while (pos < BUFSZ - 1) {
        char c;
        ssize_t r = recv(sock, &c, 1, 0);
        if (r == 0) break;       // el servidor cerró la conexión
        if (r < 0) { perror("recv"); close(sock); return EXIT_FAILURE; }
        buf[pos++] = c;
        if (c == '\n') break;    // fin de línea recibido
    }
    buf[pos] = '\0';

    // Imprimo tal cual la línea que respondió el servidor
    printf("Respuesta del servidor: %s", buf); // ya trae '\n' si el server lo envió
    close(sock);
    return EXIT_SUCCESS;
}
