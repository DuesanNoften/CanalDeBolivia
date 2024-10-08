#ifndef CANAL_H
#define CANAL_H

#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdatomic.h>
#include "CEthreads/CEthread.h"

typedef struct {
    int length; // Length of the canal
    int control_mode; // 0 = Fairness, 1 = Sign, 2 = Tico
    int W; // For Fairness mode
    int sign_time; // For Sign mode
    int current_side; // 0 = left, 1 = right, controls which side is moving
    CEmutex_t canal_lock; // Mutex to control access to the canal
} Canal;

typedef struct Node {
    Ship *ship;          // Pointer to the ship (thread) struct
    struct Node *next;   // Pointer to the next node in the queue
} Node;

typedef struct Queue {
    Node *front;  // Front of the queue (first to be processed)
    Node *rear;   // Rear of the queue (last to be processed)
    int size;     // Number of elements in the queue
} Queue;

Queue* init_queue();

void enqueue(Queue *q, Ship *ship);

Ship* dequeue(Queue *q);

Ship* peek(Queue *q);

int queue_is_empty(Queue *q);

void destroy_queue(Queue *q);


#endif CANAL_H