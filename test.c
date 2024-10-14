#include "wifi.h"
#include "CEthreads/CEthread.h"
#include "Scheduling/scheduling.h"

int exisro() {

    return 1;
}

Ship create_ship(int id, int priority, int time, int type, int direction) {
    Ship ship;
    ship.id = id;
    ship.priority = priority;
    ship.time = time;
    ship.type = type;
    ship.direction = direction;
    ship.remaining_time = time; // Inicialmente, el tiempo restante es igual al tiempo de procesamiento
    ship.real_time_max = time + 5; // Asumir un tiempo máximo para pasar por el canal

    // Inicializar el mutex del barco
    if (CEmutex_init(&ship.mutex) != 0) {
        printf("Error al inicializar el mutex para el barco con ID: %d\n", ship.id);
        exit(EXIT_FAILURE);
    }

    // Configurar el hilo del barco
    ship.thread.start_routine = exisro;
    ship.thread.arg = &ship;  // Pasar el puntero del barco al hilo
    ship.thread.stack_size = 1024 * 1024; // Asignar tamaño de pila (1 MB)

    // Crear el hilo para el barco
    if (CEthread_create(&ship.thread) == -1) {
        printf("Error al crear el hilo para el barco con ID: %d\n", ship.id);
        exit(EXIT_FAILURE);
    }

    printf("BarcoID: %d creado con hilo.\n", ship.id);

    return ship;
}




int main() {

    Ship ship1 = create_ship(1, 1, 10, 0, 0);
    // Example message to send
    int ship_data[6] = {ship1.time, ship1.type, ship1.direction, 1, 0, 1};
    char messages[100];  // Buffer to hold the string representation of the array
    int size = sizeof(ship_data) / sizeof(ship_data[0]);  // Get the size of the array
    strcpy(messages, "[");  // Start of array
    for (int i = 0; i < size; i++) {
        char buffer[10];  // Buffer for each number
        sprintf(buffer, "%d", ship_data[i]);  // Convert integer to string
        strcat(messages, buffer);  // Append number to message

        // Append a comma if it's not the last element
        if (i < size - 1) {
            strcat(messages, ", ");
        }
        else {
            strcat(messages, "]");  // End of array
        }
    }
    const char *message = messages;
    CEthreads_t thread;
    thread.start_routine = wifi_thread_function;
    thread.arg = (void *)message;
    thread.stack_size = 1024 * 1024; // Asignar tamaño de pila (1 MB)
    CEthreads_t thread2;
    thread2.start_routine = wifi_thread_function;
    thread2.arg = (void *)message;
    thread2.stack_size = 1024 * 1024; // Asignar tamaño de pila (1 MB)
    // Create a WiFi thread and send the message
    if (CEthread_create(&thread) == -1) {
        printf("Error al crear el hilo de WiFi\n");
        return 1;
    }
    if (CEthread_create(&thread2) == -1) {
        printf("Error al crear el hilo de WiFi\n");
        return 1;
    }
    // Main thread can perform other tasks here
    while (1) {
        // Simulate doing other work in the main thread
        printf("Main thread is running...\n");
        sleep(5);
    }

    return 0;
}
