//scheduling.c
#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"


// Algoritmo Round Robin: solo ordena la lista por quantum
void round_robin(Node **head, int quantum) {
    printf("no se sabe como se debe ordenar la lista con RR\n");
}

// Algoritmo de Prioridad (ordena la lista por prioridad)
void priority_scheduling(Node **head) {
    Node *sorted = NULL;
    Node *current = *head;
    
    // Insertar en orden de prioridad (menor número = mayor prioridad)
    while (current != NULL) {
        Node *next = current->next;
        if (sorted == NULL || sorted->ship.priority > current->ship.priority) {
            current->next = sorted;
            sorted = current;
        } else {
            Node *temp = sorted;
            while (temp->next != NULL && temp->next->ship.priority <= current->ship.priority) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
    printf("Prioridad: Cola ordenada por prioridad\n");
}

// Algoritmo SJF (ordena la lista por el tiempo de procesamiento)
void sjf_scheduling(Node **head) {
    Node *sorted = NULL;
    Node *current = *head;
    
    // Insertar en orden del tiempo de procesamiento
    while (current != NULL) {
        Node *next = current->next;
        if (sorted == NULL || sorted->ship.time > current->ship.time) {
            current->next = sorted;
            sorted = current;
        } else {
            Node *temp = sorted;
            while (temp->next != NULL && temp->next->ship.time <= current->ship.time) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
    printf("SJF: Cola ordenada por tiempo de procesamiento\n");
}

// Algoritmo FCFS (ya está en orden de llegada)
void fcfs_scheduling(Node **head) {
    // FCFS ya está ordenado en orden de llegada, así que no es necesario modificar la lista
    printf("FCFS: Lista ya está en orden de llegada\n");
}

// Algoritmo Tiempo Real (ordena por el tiempo máximo permitido)
void real_time_scheduling(Node **head) {
    Node *sorted = NULL;
    Node *current = *head;
    
    // Insertar en orden del tiempo máximo permitido
    while (current != NULL) {
        Node *next = current->next;
        if (sorted == NULL || sorted->ship.real_time_max > current->ship.real_time_max) {
            current->next = sorted;
            sorted = current;
        } else {
            Node *temp = sorted;
            while (temp->next != NULL && temp->next->ship.real_time_max <= current->ship.real_time_max) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
    printf("Tiempo Real: Cola ordenada por tiempo máximo permitido\n");
}
