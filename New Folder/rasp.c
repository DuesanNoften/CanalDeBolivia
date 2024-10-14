#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>  // Para configurar el puerto serial

int main() {
    const char *port = "/dev/ttyACM0";  // Ajusta si es necesario
    int serial = open(port, O_WRONLY | O_NOCTTY);

    if (serial == -1) {
        perror("Error abriendo el puerto serial");
        return 1;
    }

    // Configurar el puerto serial
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serial, &tty) != 0) {
        perror("Error obteniendo configuración del puerto");
        close(serial);
        return 1;
    }

    // Configuración de UART: 9600 baudrate, 8N1 (8 bits, sin paridad, 1 bit de parada)
    cfsetospeed(&tty, B9600);  // Velocidad de transmisión
    cfsetispeed(&tty, B9600);  // Velocidad de recepción

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;  // 8 bits
    tty.c_cflag &= ~PARENB;  // Sin paridad
    tty.c_cflag &= ~CSTOPB;  // 1 bit de parada
    tty.c_cflag &= ~CRTSCTS; // Sin control de flujo por hardware
    tty.c_cflag |= CREAD | CLOCAL;  // Habilitar lectura y sin control del puerto

    // Desactivar modos canonicos y control de eco
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Aplicar la configuración
    if (tcsetattr(serial, TCSANOW, &tty) != 0) {
        perror("Error configurando el puerto serial");
        close(serial);
        return 1;
    }

    // Enviar mensaje
    const char *mensaje = "Hola, Pico W!\n";
    write(serial, mensaje, strlen(mensaje));
    printf("Mensaje enviado: %s", mensaje);

    close(serial);
    return 0;
}
