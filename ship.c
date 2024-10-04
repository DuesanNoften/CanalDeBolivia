#include "ship.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Extern: make reference a canal mutex
extern pthread_mutex_t canal_mutex;

// Create a new ship
Ship* createShip(int stype, int yPos) {
    Ship* newShip = (Ship*)malloc(sizeof(Ship));
    newShip->x = 0;
    newShip->y = yPos;
    newShip->type = stype;
    newShip->speed = (stype == 0) ? 1 : (stype == 1) ? 2 : 3;  // 0 = Normal, 1 = Fishing, 2 = Patrol
    return newShip;
}

// Move the ship (on a thread)
void* moveShip(void* arg) {
    Ship* ship = (Ship*) arg;

    while (ship->x < SCREEN_WIDTH) {
        // Try to get the canal mutex, to locked it
        pthread_mutex_lock(&canal_mutex);

        //Move the ship through the canal (one at a time)
        printf("%d type ship is crossing.\n", ship->type);
        while(ship->x < (SCREEN_WIDTH/2)+80){
            ship->x += ship->speed;
            usleep(10000);  // Control the ship speed (microsec)
        }

        // Free the canal mutex
        pthread_mutex_unlock(&canal_mutex);

        // Finish moving the ship out of canal
        while (ship->x < SCREEN_WIDTH) {
            ship->x += ship->speed;
            usleep(10000);
        }
    }

    return NULL;
}