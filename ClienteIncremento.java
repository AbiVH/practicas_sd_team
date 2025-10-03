import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

// Cliente Java que envía enteros al servidor C y muestra (n+1)
// Uso: java ClienteIncremento <host> <puerto>
public class ClienteIncremento {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("Uso: java ClienteIncremento <host> <puerto>");
            return;
        }
        String host = args[0];
        int port = Integer.parseInt(args[1]);

        try (Socket socket = new Socket(host, port);
             BufferedReader in = new BufferedReader(
                 new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8)
             );
             PrintWriter out = new PrintWriter(
                 new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8),
                 true
             );
             Scanner scanner = new Scanner(System.in)) {

            System.out.println("Conectado a " + host + ":" + port);
            while (true) {
                System.out.print("Ingresa un entero (0 para terminar): ");
                String linea = scanner.nextLine().trim();
                // Validación básica: vacío -> seguir pidiendo
                if (linea.isEmpty()) continue;

                // Enviar tal cual como línea (servidor C espera línea con entero)
                out.println(linea);

                // Si es 0, el servidor cerrará tras responder 1; luego terminamos
                try {
                    int n = Integer.parseInt(linea);
                    if (n == 0) {
                        String resp = in.readLine();
                        System.out.println("Respuesta del servidor: " + resp);
                        break;
                    }
                } catch (NumberFormatException ignored) {}

                // Leer respuesta y mostrar
                String resp = in.readLine();
                if (resp == null) {
                    System.out.println("Conexión cerrada por el servidor.");
                    break;
                }
                System.out.println("Respuesta del servidor: " + resp);
            }
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}


