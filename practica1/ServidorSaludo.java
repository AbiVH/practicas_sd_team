// Importaciones necesarias para el manejo de sockets y entrada/salida
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

/**
 * Servidor de saludo simple que maneja múltiples clientes 
 * Cada cliente se conecta, envía un mensaje y recibe "Hola que tal" como respuesta
 */
public class ServidorSaludo {

    public static void main(String[] args) {
        // Puerto por defecto, pero puede ser cambiado desde línea de comandos
        int port = 5000;
        if (args.length >= 1) {
            port = Integer.parseInt(args[0]);
        }

        System.out.println("[Servidor] Iniciando en el puerto " + port + " ...");
        
        // Se crea un ServerSocket que escuchará las conexiones entrantes
        try (ServerSocket server = new ServerSocket(port)) {
            // Bucle infinito para aceptar clientes uno por uno (secuencial)
            while (true) {
                // Se espera a que llegue una conexión (esto bloquea hasta que alguien se conecte)
                Socket client = server.accept();

                // Manejo del cliente
                String remote = client.getRemoteSocketAddress().toString();
                System.out.println("[Servidor] Conexión de " + remote);

                // Uso try-with-resources para manejar el cierre de streams
                try (
                    // BufferedReader para leer texto del cliente (con codificación UTF-8)
                    BufferedReader in = new BufferedReader(
                        new InputStreamReader(client.getInputStream(), StandardCharsets.UTF_8)
                    );
                    // PrintWriter para enviar texto al cliente (con autoFlush activado)
                    PrintWriter out = new PrintWriter(
                        new OutputStreamWriter(client.getOutputStream(), StandardCharsets.UTF_8),
                        true // autoFlush - envía inmediatamente sin necesidad de flush()
                    )
                ) {
                    // Lee UNA línea del cliente (terminada en '\n')
                    // readLine() bloquea hasta que reciba una línea completa
                    String linea = in.readLine();
                    System.out.println("[Servidor] Recibí: " + linea);

                    // Responde SIEMPRE con "Hola que tal"
                    // println() añade automáticamente el salto de línea
                    out.println("Hola que tal");

                    // La conexión se cierra automáticamente al salir del try-with-resources
                } catch (IOException e) {
                    System.err.println("[Servidor] Error con " + remote + ": " + e.getMessage());
                } finally {
                    try { client.close(); } catch (IOException ignored) {}
                }
            }
        } catch (IOException e) {
            System.err.println("[Servidor] Error: " + e.getMessage());
        }
    }
}
