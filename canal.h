#ifndef CANAL_H
#define CANAL_H

#include "ships/ship.h"
#include <SDL2/SDL.h>

void drawShip(SDL_Renderer* renderer, Ship* ship);
void drawCanal(SDL_Renderer* renderer);

#endif