#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Scheduling/scheduling.h"
#include "CEthreads/CEthread.h"
#include "canal.h"

// Prototipo de la función de procesamiento del barco
int process_ship_thread(void *arg) {
    Ship *ship = (Ship *)arg;

    printf("El hilo barcoID: %d comenzo\n", ship->id);

    CEmutex_lock(&ship->mutex);
    sleep(ship->time);  // Simula el tiempo que toma procesar el barco
    printf("El hilo barcoID: %d termino.\n", ship->id);
    CEmutex_unlock(&ship->mutex);

    return 0;
}


// Función para crear un barco y asignarle valores
Ship create_ship(int id, int priority, int time) {
    Ship ship;
    ship.id = id;
    ship.priority = priority;
    ship.time = time;
    ship.remaining_time = time; // Inicialmente, el tiempo restante es igual al tiempo de procesamiento
    ship.real_time_max = time + 5; // Asumir un tiempo máximo para pasar por el canal

    // Inicializar el mutex del barco
    if (CEmutex_init(&ship.mutex) != 0) {
        printf("Error al inicializar el mutex para el barco con ID: %d\n", ship.id);
        exit(EXIT_FAILURE);
    }

    // Configurar el hilo del barco
    ship.thread.start_routine = process_ship_thread;
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

void insert_ship(Node **head, Ship ship) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->ship = ship;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int main() {
    // Configuración del canal
    CanalConfig canal_config;
    canal_config.flow_control_method = 0; // 0: Equidad, 1: Letrero, 2: Tico
    canal_config.canal_length = 100;       // Longitud del canal
    canal_config.ship_speed = 5;            // Velocidad del barco
    canal_config.num_ships = 10;            // Cantidad total de barcos
    canal_config.time_to_switch = 3;        // Tiempo para cambiar el letrero
    canal_config.W = 3;                      // Parámetro W para el control de flujo

    // Crear listas de barcos
    Node *left_ships = NULL;
    Node *right_ships = NULL;

    // Crear algunos barcos y añadirlos a las listas
    for (int i = 0; i < 5; i++) {
        //Ship ship = create_ship(i + 1, 1, rand() % 5 + 1); // ID, Prioridad, Tiempo (1-5)
        Ship ship = create_ship(i + 1, 1, 5); 
        insert_ship(&left_ships, ship);
    }

    for (int i = 0; i < 5; i++) {
        //Ship ship = create_ship(i + 6, 1, rand() % 5 + 1); // ID, Prioridad, Tiempo (1-5)
        Ship ship = create_ship(i + 6, 1, 5); 
        insert_ship(&right_ships, ship);
    }

    // Iniciar el paso de barcos por el canal
    start_canal(&canal_config, &left_ships, &right_ships);

    // Liberar memoria de las listas de barcos
    Node *current;
    while (left_ships) {
        current = left_ships;
        left_ships = left_ships->next;
        free(current);
    }
    while (right_ships) {
        current = right_ships;
        right_ships = right_ships->next;
        free(current);
    }

    return 0;
}
