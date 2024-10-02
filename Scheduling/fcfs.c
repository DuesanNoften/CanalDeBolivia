#include <stdio.h>
#include "scheduling.h"

void fcfs(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("Ejecutando barco ID %d en FCFS\n", current->ship.id);
        current = current->next;
    }
}
