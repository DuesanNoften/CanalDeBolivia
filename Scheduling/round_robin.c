#include <stdio.h>
#include "scheduling.h"

void round_robin(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("Running ship ID %d in Round Robin\n", current->ship.id);
        current = current->next;
    }
}
