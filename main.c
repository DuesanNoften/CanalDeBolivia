#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ship.h"  // create and move ships
#include "canal.h"  // Draw the canal and the ships on it
#include <time.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Create a mutex to sync the canal access
pthread_mutex_t canal_mutex;

// List of ships
Ship* ships[100];  // Until 100 ships to prevent array overflows
int numShips = 0;  // ship counter

int initSDL() {
    // // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializing SDL: %s\n", SDL_GetError());
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

// Gen random ship
void generarBarcoAleatorio() {
    int tipo = rand() % 3;  // Type: 0 = Normal, 1 = fishing, 2 = Patrol
    int yPos = 280 + rand() % (300 - 280 +1);  // Posición vertical aleatoria

    Ship* nuevoBarco = createShip(tipo, yPos);

    // Add ship to waiting list
    ships[numShips] = nuevoBarco;
    numShips++;

    // Create the ship thread
    pthread_t thread;
    pthread_create(&thread, NULL, moveShip, nuevoBarco);

    printf("Ship type %d gen in pos y=%d\n", tipo, yPos);
}

int main(int argc, char* args[]) {
    if (initSDL() != 0) return -1;

    srand(time(NULL));  // Random seed

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            // Verify if closed window
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            // Detect keyboard interruption <space>
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                // gen the ship when space bar press
                generarBarcoAleatorio();
            }
        }

        // Clean window
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Draw the canal and the ships
        drawCanal(renderer);

        for (int i = 0; i < numShips; i++) {
            drawShip(renderer, ships[i]);
        }

        // Refresh screen
        SDL_RenderPresent(renderer);
    }

    // Clean and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //kill canal mutex
    pthread_mutex_destroy(&canal_mutex);

    //Free memory
    for (int i = 0; i < numShips; i++) {
        free(ships[i]);
    }

    return 0;
}

//--------------------------------------------------------

//SDL_Window* window = NULL;
//SDL_Renderer* renderer = NULL;


/*

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
*/
