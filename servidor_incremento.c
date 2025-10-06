// Servidor TCP en C que recibe enteros en texto y responde con entero+1
// Uso: ./servidor_incremento <puerto>
// Protocolo simple: cliente envía una línea con un entero (terminada en '\n'),
// el servidor responde con otra línea con (n+1). Si recibe 0, responde 1 y
// cierra la conexión.

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
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *port = argv[1];

    // Preparar dirección de escucha, socket TCP
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    int s = getaddrinfo(NULL, port, &hints, &res);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return EXIT_FAILURE;
    }

    int srv = -1;
    int yes = 1;
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        srv = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (srv == -1) continue;
        setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
        if (bind(srv, rp->ai_addr, rp->ai_addrlen) == 0) break;
        close(srv); srv = -1;
    }
    freeaddrinfo(res);
    if (srv == -1) { perror("bind/listen socket"); return EXIT_FAILURE; }

    if (listen(srv, 10) != 0) { perror("listen"); close(srv); return EXIT_FAILURE; }
    fprintf(stdout, "[Servidor] Escuchando en puerto %s...\n", port);

    // Atender clientes secuencialmente
    for (;;) {
        struct sockaddr_storage caddr; socklen_t clen = sizeof caddr;
        int cli = accept(srv, (struct sockaddr*)&caddr, &clen);
        if (cli < 0) { perror("accept"); continue; }

        // Bucle por conexión: procesar múltiples enteros hasta recibir 0 o cierre
        for (;;) {
            // Leer una línea con un entero
            char buf[BUFSZ]; size_t pos = 0;
            while (pos < BUFSZ - 1) {
                char c; ssize_t r = recv(cli, &c, 1, 0);
                if (r == 0) { // cerrado por cliente
                    goto fin_conexion;
                }
                if (r < 0) { perror("recv"); goto fin_conexion; }
                buf[pos++] = c;
                if (c == '\n') break;
            }
            buf[pos] = '\0';

            // Convertir a entero (en base 10)
            long n = strtol(buf, NULL, 10);
            long inc = n + 1;

            // Responder con (n+1) seguido de \n
            char out[64];
            int wlen = snprintf(out, sizeof out, "%ld\n", inc);
            if (wlen > 0) send(cli, out, (size_t)wlen, 0);

            // Si el número fue 0, cerrar la conexión (protocolo de fin)
            if (n == 0) {
                fprintf(stdout, "[Servidor] Se recibió 0. Cerrando conexión con cliente.\n");
                break;
            }
        }
fin_conexion:
        close(cli);
    }

    close(srv);
    return EXIT_SUCCESS;
}


