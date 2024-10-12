#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "canal.h"

// Función para simular el paso de un barco
void process_ship(Ship *ship) {
    printf("Thread for ship ID: %d is starting.\n", ship->id);
    sleep(ship->time);  // Simula el tiempo de procesamiento del barco
    printf("Thread for ship ID: %d has completed.\n", ship->id);
}

// Función principal del canal
void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships) {
    int w = config->W; // Número de barcos a pasar en cada dirección
    int left_count = 0;
    int right_count = 0;

    printf("Comenzando el paso de barcos por el canal:\n");

    while (*left_ships || *right_ships) {
        // Pasar barcos del lado izquierdo
        left_count = 0;
        while (*left_ships && left_count < w) {
            Ship ship = (*left_ships)->ship;
            process_ship(&ship);
            Node *temp = *left_ships;
            *left_ships = (*left_ships)->next;
            free(temp);
            left_count++;
        }

        // Pasar barcos del lado derecho
        right_count = 0;
        while (*right_ships && right_count < w) {
            Ship ship = (*right_ships)->ship;
            process_ship(&ship);
            Node *temp = *right_ships;
            *right_ships = (*right_ships)->next;
            free(temp);
            right_count++;
        }
    }

    printf("Todos los barcos han pasado por el canal.\n");
}

void setCanalConfigFromFile(CanalConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value) {
            if (strcmp(key, "flow_control_method") == 0) {
                config->flow_control_method = atoi(value);
            } else if (strcmp(key, "canal_length") == 0) {
                config->canal_length = atoi(value);
            } else if (strcmp(key, "ship_speed") == 0) {
                config->ship_speed = atoi(value);
            } else if (strcmp(key, "num_ships") == 0) {
                config->num_ships = atoi(value);
            } else if (strcmp(key, "time_to_switch") == 0) {
                config->time_to_switch = atoi(value);
            } else if (strcmp(key, "W") == 0) {
                config->W = atoi(value);
            }
        }
    }

    fclose(file);
}
