//To compile: gcc -o test_sdl2 test_sdl2.c -lSDL2
//To run: ./test_sdl2

#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
	if (SDL_Init(SDL_INIT_VIDEO)<0){
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return -1;
	}
	printf("SDL initialized successfully!\n");
	SDL_Quit();
	return 0;
}
