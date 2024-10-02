#include <stdio.h>
#include "scheduling.h"

void sjf(Node *head) {
    Node *current = head;
    Node *shortest_job = current;

    while (current != NULL) {
        if (current->ship.time < shortest_job->ship.time) {
            shortest_job = current;
        }
        current = current->next;
    }
    printf("Running ship with the shortest time ID %d\n", shortest_job->ship.id);
}
