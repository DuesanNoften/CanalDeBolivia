#include <stdio.h>
#include "scheduling.h"

void real_time(Node *head) {
    Node *current = head;
    Node *urgent = current;

    while (current != NULL) {
        if (current->ship.real_time_max < urgent->ship.real_time_max) {
            urgent = current;
        }
        current = current->next;
    }
    printf("Running real-time ship ID %d\n", urgent->ship.id);
}
