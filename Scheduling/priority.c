#include <stdio.h>
#include "scheduling.h"

void priority(Node *head) {
    Node *current = head;
    Node *highest_priority = current;

    while (current != NULL) {
        if (current->ship.priority > highest_priority->ship.priority) {
            highest_priority = current;
        }
        current = current->next;
    }
    printf("Running ship with highest priority ID %d\n", highest_priority->ship.id);
}
