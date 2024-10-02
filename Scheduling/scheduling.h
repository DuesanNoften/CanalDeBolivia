#ifndef SCHEDULING_H
#define SCHEDULING_H

typedef struct {
    int id;
    int type;
    int priority;
    int time;
    int real_time_max;
} Ship;

typedef struct Node {
    Ship ship;
    struct Node *next;
} Node;

// Function prototypes for scheduling algorithms
void round_robin(Node *head);
void priority(Node *head);
void sjf(Node *head);
void fcfs(Node *head);
void real_time(Node *head);

#endif
