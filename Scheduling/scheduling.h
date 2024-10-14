#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "../CEthreads/CEthread.h"

// Estructura que representa un barco
typedef struct {
    int id;
    int priority;
    int time;  
    int remaining_time;  
    int real_time_max;
    int type; // 0 = normal, 1 = pesquero, 2 = patrulla
    int direction; // 0 = izquierda-derecha, 1 = derecha-izquierda
    CEthreads_t thread;  
    CEmutex_t mutex;  
} Ship;

typedef struct Node {
    Ship ship;
    struct Node *next;
} Node;

void round_robin(Node **head, int quantum);
void priority_scheduling(Node **head);
void sjf_scheduling(Node **head);
void fcfs_scheduling(Node **head);
void real_time_scheduling(Node **head);

#endif
