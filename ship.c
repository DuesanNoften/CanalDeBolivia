#include "ship.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>

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
        ship->x += ship->speed;
        usleep(10000);  // Control the ship speed (microsec)
    }

    return NULL;
}