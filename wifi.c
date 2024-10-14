#include "wifi.h"

int sock;

void handle_exit(int sig) {
    printf("\nCerrando cliente...\n");
    close(sock);
    exit(0);
}

int initialize_socket() {
    struct sockaddr_in server_addr;

    // Capturar señal para cierre limpio
    signal(SIGINT, handle_exit);

    // Crear el socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Error al crear el socket");
        return -1;
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Dirección inválida o no soportada");
        close(sock);
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Conexión fallida");
        close(sock);
        return -1;
    }

    return 0;
}

int send_message(const char *message) {
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Error al enviar datos");
        return -1;
    }

    printf("Mensaje enviado: %s\n", message);
    return 0;
}

