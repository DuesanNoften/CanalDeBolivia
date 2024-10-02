#include <stdio.h>
#include <stdlib.h>
#include "Scheduling/scheduling.h"

int main() {
    // Create some ships
    // ID, type, priority, time, real_time_max
    Ship ship1 = {1, 0, 3, 10, 20};  
    Ship ship2 = {2, 2, 1, 5, 10};
    Ship ship3 = {3, 1, 2, 8, 15};

    // Create a linked list of ships (threads)
    Node *head = (Node *)malloc(sizeof(Node));
    head->ship = ship1;
    head->next = (Node *)malloc(sizeof(Node));
    head->next->ship = ship2;
    head->next->next = (Node *)malloc(sizeof(Node));
    head->next->next->ship = ship3;
    head->next->next->next = NULL;

    // Run scheduling algorithms
    printf("Running Round Robin:\n");
    round_robin(head);

    printf("Running Priority:\n");
    priority(head);

    printf("Running SJF:\n");
    sjf(head);

    printf("Running FCFS:\n");
    fcfs(head);

    printf("Running Real-Time:\n");
    real_time(head);

    // Free allocated memory
    free(head->next->next);
    free(head->next);
    free(head);

    return 0;
}
