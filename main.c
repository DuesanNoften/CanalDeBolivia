#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ship.h"  // create and move ships
#include "canal.h"  // Draw the canal and the ships on it

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Create a mutex to sync the canal access
pthread_mutex_t canal_mutex;

int initSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    window = SDL_CreateWindow("Scheduling Ships Canal de Bolivia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return -1;
    }

    // Create Render
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating render: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize canal mutex
    pthread_mutex_init(&canal_mutex, NULL);

    return 0;
}

// Select 1st priority ship by speed
Ship* getHighestPriorityShip(Ship* ships[], int numShips) {
    Ship* highestPriorityShip = NULL;
    for (int i = 0; i < numShips; i++) {
        if (ships[i]->x < SCREEN_WIDTH / 2) {  // Ships that are waiting
            if (highestPriorityShip == NULL || ships[i]->speed > highestPriorityShip->speed) {
                highestPriorityShip = ships[i];  // Find the fastest in queue
            }
        }
    }
    return highestPriorityShip;
}

int main(int argc, char* args[]) {
    if (initSDL() != 0) return -1;

    int quit = 0;
    SDL_Event e;

    // Creating ships
    Ship* ship4 = createShip(2, 295);  // PAtrol
    Ship* ship1 = createShip(0, 280);  // Normal
    Ship* ship2 = createShip(1, 290);  // Fishing
    Ship* ship3 = createShip(2, 300);  // PAtrol

    //Priority line for ships
    //Ship* ships[] = {ship1, ship2, ship3};
    //int numShips = 3;

    // Creating the ships threads
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread1, NULL, moveShip, ship1);
    pthread_create(&thread2, NULL, moveShip, ship2);
    pthread_create(&thread3, NULL, moveShip, ship3);
    pthread_create(&thread4, NULL, moveShip, ship4);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clean window
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Draw the canal and the ships
        drawCanal(renderer);
        drawShip(renderer, ship1);
        drawShip(renderer, ship2);
        drawShip(renderer, ship3);
        drawShip(renderer, ship4);

        // Refresh screen
        SDL_RenderPresent(renderer);
    }

    // Clean and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Wait until the threads finished
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    //kill canal mutex
    pthread_mutex_destroy(&canal_mutex);

    //Free memory
    free(ship1);
    free(ship2);
    free(ship3);
    free(ship4);

    return 0;
}