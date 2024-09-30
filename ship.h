#ifndef SHIP_H
#define SHIP_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    int x, y; // ship position
    int speed; // speed movement
    int type; // ship type: 0 = Normal, 1 = Fishing, 2 = Patrol
} Ship;

Ship* createShip(int tipo, int yPos);
void* moveShip(void* arg);

#endif