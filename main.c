#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Proyecto Canal de Bolivia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create Render
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating render: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Main event loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clean screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); 
        SDL_RenderClear(renderer);

        // Render -> a rectangle
        SDL_SetRenderDrawColor(renderer, 0x86, 0x57, 0xDB, 0xFF);//entry (window, R, G, B, alpha)
        SDL_Rect fillRect = { 200, 100, 200, 400}; //entry (x_0, y_0, x_final, y_final)
        SDL_RenderFillRect(renderer, &fillRect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
