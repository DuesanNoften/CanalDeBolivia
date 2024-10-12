//canal.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "canal.h"

void drawShip(SDL_Renderer* renderer, Ship* ship) {
    SDL_Rect shipRect = { ship->x, ship->y, 50, 20 };

    // Change the color between ship types
    if (ship->priority == 3) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red for the normal ones
    } else if (ship->priority == 2) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);  // Green for fishing ones
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);  // Blue for patrol ones
    }
    SDL_RenderFillRect(renderer, &shipRect);
}

// Función para simular el paso de un barco
void process_ship(Ship *ship, SDL_Renderer *renderer) {
    drawShip(renderer, ship);
    SDL_RenderPresent(renderer);

    printf("Thread for ship ID: %d is starting.\n", ship->id);
    sleep(ship->time);  // Simula el tiempo de procesamiento del barco
    printf("Thread for ship ID: %d has completed.\n", ship->id);
}

// Función principal del canal
void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships, SDL_Renderer *renderer) {
    int w = config->W; // Número de barcos a pasar en cada dirección
    int left_count = 0;
    int right_count = 0;

    printf("Comenzando el paso de barcos por el canal:\n");

    while (*left_ships || *right_ships) {
        // Pasar barcos del lado izquierdo
        left_count = 0;
        while (*left_ships && left_count < w) {
            Ship ship = (*left_ships)->ship;
            process_ship(&ship, renderer);
            Node *temp = *left_ships;
            *left_ships = (*left_ships)->next;
            free(temp);
            left_count++;
        }

        // Pasar barcos del lado derecho
        right_count = 0;
        while (*right_ships && right_count < w) {
            Ship ship = (*right_ships)->ship;
            process_ship(&ship, renderer);
            Node *temp = *right_ships;
            *right_ships = (*right_ships)->next;
            free(temp);
            right_count++;
        }
    }

    printf("Todos los barcos han pasado por el canal.\n");
}
