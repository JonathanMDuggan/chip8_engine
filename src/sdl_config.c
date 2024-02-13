#pragma once
#include <stdio.h>
#include "sdl_config.h"
#include "chip8_names.h"
void sdlVideoStuff() {
	printf("[+] Starting up SDL Video Stuff!\n");
	return;
}
void SDLStart() {
	printf("[i] SDL Initializing.\n");
	SDLInitialize();
}
// returns 1 if the process wasn't able to compete
uint8_t SDLInitialize() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[-] SDL could not initialize");
	}
	SDL_Window* window = 
		SDL_CreateWindow(CHIP8_APPLICATION_NAME,    SDL_WINDOWPOS_UNDEFINED,
										 SDL_WINDOWPOS_UNDEFINED,   kChip8DefaultWindowLength,
										 kChip8DefaultWindowHeight, SDL_WINDOW_SHOWN |
									   SDL_WINDOW_OPENGL);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {
		printf("[-] SDL Renderer failted to initialize\n");
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);	return 0;
}
// Create window for the Chip 8 emulator

void SDLUpdate() {

}

void SDLInput() {

}

void SDLRendrer(SDL_Renderer* renderer, SDL_Surface* window){

}

void SDLDraw() {

}

// Close all SDL functions
uint8_t SDLCleanUp() {
	// Closes all the functions or something, you got to clear them all before
	// closing the app 
	printf("[+] SDL shuting down");
	SDL_Quit();
	return 0;
}