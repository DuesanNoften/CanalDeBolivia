#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "../CEthreads/CEthread.h"

// Estructura que representa un barco
typedef struct {
    int id;
    int priority;
    int time; 
    int type; 
    int remaining_time;  
    int real_time_max;  
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
