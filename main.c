// main.c

#include <stdio.h>
#include <stdlib.h>

#include "Canal/Factories/boatFactory.h"
#include "Canal/Factories/canalFactory.h"

int main() {
    printf("Testing Boats and Canals Initialization Only:\n");

    // Create an equity canal
    Canal* equityCanal = createCanal(EQUITY_CANAL, 100, 5);
    if (equityCanal != NULL) {
        printf("Equity Canal Created - Type: %d, Size: %d\n", equityCanal->type, equityCanal->size);
    }

    // Destroy the existing canal
    destroyCanal();

    // Create a boat
    Boat* normalBoat = createBoat(NORMAL_BOAT, 8, 1, 'N');
    if (normalBoat != NULL) {
        printf("Normal Boat Created - Speed: %d, Priority: %d, ID: %d, Direction: %c\n",
               normalBoat->speed, normalBoat->priority, normalBoat->id, normalBoat->direction);
        free(normalBoat);
    }

    return 0;
}