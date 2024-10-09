#include "ship.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Extern: make reference a canal mutex
extern pthread_mutex_t canal_mutex;

// Create a new ship
Ship* createShip(int stype, int yPos, int direction) {
    Ship* newShip = (Ship*)malloc(sizeof(Ship));
    newShip->y = yPos;
    newShip->type = stype;
    newShip->speed = (stype == 0) ? 1 : (stype == 1) ? 2 : 3;  // 0 = Normal, 1 = Fishing, 2 = Patrol
    newShip->direction = direction;

    if (direction==0){
        newShip->x = 0; //left to right
    } else {
        newShip->x = SCREEN_WIDTH-50; //right to left
    }

    return newShip;
}

// Move the ship (on a thread)
void* moveShip(void* arg) {
    Ship* ship = (Ship*) arg;

    while (1) {
        // Try to get the canal mutex, to lock it
        pthread_mutex_lock(&canal_mutex);

        // Move the ship through the canal (one at a time)
        if (ship->direction == 0) {  // Left to right
            while (ship->x < SCREEN_WIDTH / 2) {
                ship->x += ship->speed;
                usleep(10000);  // Control the speed
            }
        } else {  // Right to left
            while (ship->x > SCREEN_WIDTH / 2) {
                ship->x -= ship->speed;
                usleep(10000);  // Control the speed
            }
        }

        printf("%d type ship is crossing in direction %d.\n", ship->type, ship->direction);

        

        // Continue moving the ship out of the canal
        if (ship->direction == 0) {  // Left to right
            while (ship->x < SCREEN_WIDTH) {
                ship->x += ship->speed;
                usleep(10000);  // Control the speed
            }
        } else {  // Right to left
            while (ship->x > -50) {  // Move out of the screen on the left
                ship->x -= ship->speed;
                usleep(10000);  // Control the speed
            }
        }
        // Free the canal mutex after crossing the center
        pthread_mutex_unlock(&canal_mutex);

        break;  // The ship has finished crossing the canal
    }

    return NULL;
}