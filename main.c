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
Ship* shipsLeftToRight[100];  // Until 100 ships to prevent array overflows
int numShipsLeftToRight = 0;  // ship counter
Ship* shipsRightToLeft[100];  // Until 100 ships to prevent array overflows
int numShipsRightToLeft = 0;  // ship counter

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
    int yPos = 275 + rand() % (305 - 275 +1);  // Random Pos y in canal
    int direction = rand() % 2; //Random direction: 0 = init left, 1 = init right

    Ship* nuevoBarco = createShip(tipo, yPos, direction);

    // Add ship to waiting list
    if (direction==0){
        shipsLeftToRight[numShipsLeftToRight] = nuevoBarco;
        numShipsLeftToRight++; //left to right
    } else {
        shipsRightToLeft[numShipsRightToLeft] = nuevoBarco;
        numShipsRightToLeft++; //right to left
    }
    
    // Create the ship thread
    pthread_t thread;
    pthread_create(&thread, NULL, moveShip, nuevoBarco);

    printf("Ship type %d gen in pos y=%d, direction=%d\n", tipo, yPos, direction);
}

void generarBarcoEspecifico(int tipo, int yPos, int direction) {
    Ship* nuevoBarco = createShip(tipo, yPos, direction);

    // Add ship to correspondent list
    if (direction == 0) {  // Left to right
        shipsLeftToRight[numShipsLeftToRight] = nuevoBarco;
        numShipsLeftToRight++;
    } else {  // Right to left
        shipsRightToLeft[numShipsRightToLeft] = nuevoBarco;
        numShipsRightToLeft++;
    }

    // Create the thread for the ship
    pthread_t thread;
    pthread_create(&thread, NULL, moveShip, nuevoBarco);

    printf("Ship type %d gen in pos y= %d , direction=%d\n", tipo, yPos, direction);
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

            // Detect keyboard interruption
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    // Ships left to right
                    case SDLK_q:  // Patrol left
                        generarBarcoEspecifico(2, 275 + rand() % (305 - 275 + 1), 0);  // Type 2 (Patrol), Direction 0 (right to left)
                        break;
                    case SDLK_w:  // Fishing left
                        generarBarcoEspecifico(1, 275 + rand() % (305 - 275 + 1), 0);  // Type 1 (Fishing), Direction 0
                        break;
                    case SDLK_e:  // Normal left
                        generarBarcoEspecifico(0, 275 + rand() % (305 - 275 + 1), 0);  // Type 0 (Normal), Direction 0
                        break;
                    
                    // Ships left to right
                    case SDLK_i:  // Normal right
                        generarBarcoEspecifico(0, 275 + rand() % (305 - 275 + 1), 1);  // Type 0 (Normal), Direction 1 (left to right)
                        break;
                    case SDLK_o:  // Fishing right
                        generarBarcoEspecifico(1, 275 + rand() % (305 - 275 + 1), 1);  // Type 1 (Fishing), Direction 1
                        break;
                    case SDLK_p:  // Patrol right
                        generarBarcoEspecifico(2, 275 + rand() % (305 - 275 + 1), 1);  // Type 2 (Patrol), Direction 1
                        break;

                    // Ships generated random with space bar
                    case SDLK_SPACE:
                        generarBarcoAleatorio();
                        break;                
                }
            }
        }

        // Clean window
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Draw the canal and the ships
        drawCanal(renderer);

        for (int i = 0; i < numShipsLeftToRight; i++) {
            drawShip(renderer, shipsLeftToRight[i]);
        }

        for (int i = 0; i < numShipsRightToLeft; i++) {
            drawShip(renderer, shipsRightToLeft[i]);
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
    for (int i = 0; i < numShipsLeftToRight; i++) {
        free(shipsLeftToRight[i]);
    }

    for (int i = 0; i < numShipsRightToLeft; i++) {
        free(shipsRightToLeft[i]);
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
