#include "canal.h"
#include <SDL2/SDL.h>

// Draw a ship on window
void drawShip(SDL_Renderer* renderer, Ship* ship) {
    SDL_Rect shipRect = { ship->x, ship->y, 50, 20 };

    // Change the color between ship types
    if (ship->type == 0) SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red for the normal ones
    else if (ship->type == 1) SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);  // Green for fishing ones
    else SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);  // Blue for patrol ones

    SDL_RenderFillRect(renderer, &shipRect);
}

// Draw the canal
void drawCanal(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);  // White for the canal
    SDL_Rect fillRect = {70, (SCREEN_HEIGHT / 2)-30, (SCREEN_WIDTH/2)+260, 60}; 
    SDL_RenderFillRect(renderer, &fillRect);
}