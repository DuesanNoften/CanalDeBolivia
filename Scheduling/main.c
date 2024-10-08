#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"

int main() {
    // Crear algunos barcos con diferentes parámetros
    Ship ship1 = {1, 3, 6, 6, 20};
    Ship ship2 = {2, 1, 4, 4, 10};
    Ship ship3 = {3, 2, 8, 8, 15};

    // Inicializar la lista de barcos
    Node *head = NULL;
    insert_ship(&head, ship1);
    insert_ship(&head, ship2);
    insert_ship(&head, ship3);

    printf("Lista de barcos inicial:\n");
    Node *temp = head;
    while (temp != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d\n", 
               temp->ship.id, temp->ship.priority, temp->ship.time, temp->ship.real_time_max);
        temp = temp->next;
    }
    
    // Seleccionar un algoritmo de calendarización para probar
    
    //printf("\nProbando Round Robin:\n");
    //round_robin(&head, 2);  // Quantum es 2

     printf("\nProbando Prioridad:\n");
     priority_scheduling(&head);

    // printf("\nProbando SJF (Shortest Job First):\n");
    // sjf_scheduling(&head);

    // printf("\nProbando FCFS (First Come First Served):\n");
    // fcfs_scheduling(&head);

    // printf("\nProbando Tiempo Real:\n");
    // real_time_scheduling(&head);

    printf("\nLista de barcos después de aplicar el algoritmo:\n");
    temp = head;
    while (temp != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d\n", 
               temp->ship.id, temp->ship.priority, temp->ship.time, temp->ship.real_time_max);
        temp = temp->next;
    }

    return 0;
}
