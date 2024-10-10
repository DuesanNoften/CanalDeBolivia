#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "ship.h"
#include "shipGenerator.h"

// List of ships
extern Ship* shipsLeftToRight[100];  // Until 100 ships to prevent array overflows
extern int numShipsLeftToRight;  // ship counter
extern Ship* shipsRightToLeft[100];  // Until 100 ships to prevent array overflows
extern int numShipsRightToLeft;  // ship counter

// Gen random ship
void generarBarcoAleatorio() {
    int tipo = rand() % 3;  // Type: 0 = Normal, 1 = fishing, 2 = Patrol
    int yPos = 280 + rand() % (300 - 280 + 1);  // Random Pos y in canal
    int direction = rand() % 2;  //Random direction: 0 = init left, 1 = init right

    generarBarcoEspecifico(tipo, yPos, direction);
}

// Gen specific ship
void generarBarcoEspecifico(int tipo, int yPos, int direction) {
    Ship* nuevoBarco = createShip(tipo, yPos, direction);

    // Add ship to waiting list
    if (direction == 0) {  //left to right
        shipsLeftToRight[numShipsLeftToRight] = nuevoBarco;
        numShipsLeftToRight++;
    } else {  //right to left
        shipsRightToLeft[numShipsRightToLeft] = nuevoBarco;
        numShipsRightToLeft++;
    }

    // Create the ship thread
    pthread_t thread;
    pthread_create(&thread, NULL, moveShip, nuevoBarco);

    printf("Ship type %d gen in pos y=%d, direction=%d\n", tipo, yPos, direction);
}

