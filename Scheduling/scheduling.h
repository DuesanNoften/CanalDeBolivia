#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <pthread.h>

// Estructura que representa un barco
typedef struct {
    int id;
    int priority;
    int time;  // Tiempo de procesamiento (simulado)
    int remaining_time;  // Tiempo restante para procesarse
    int real_time_max;  // Tiempo máximo para pasar por el canal (para tiempo real)
    pthread_t thread;
    pthread_mutex_t mutex;  // Mutex para pausar/reanudar el hilo
    pthread_cond_t cond;    // Condición para señalizar la reanudación
} Ship;

// Estructura nodo para la lista enlazada de barcos
typedef struct Node {
    Ship ship;
    struct Node *next;
} Node;

// Función para insertar un barco en la lista enlazada
void insert_ship(Node **head, Ship ship);

// Prototipos de algoritmos de calendarización
void round_robin(Node **head, int quantum);
void priority_scheduling(Node **head);
void sjf_scheduling(Node **head);
void fcfs_scheduling(Node **head);
void real_time_scheduling(Node **head);

#endif
