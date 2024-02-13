#pragma once
#include <stdio.h>
#include <stdlib.h>
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
	SDL_Window*   window   = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface*  surface  = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[-] SDL could not initialize");
		return EXIT_FAILURE;
	}
	// Creates a window instance
	window = SDL_CreateWindow(CHIP8_APPLICATION_NAME,    SDL_WINDOWPOS_UNDEFINED,
														SDL_WINDOWPOS_UNDEFINED,   kChip8DefaultWindowLength,
														kChip8DefaultWindowHeight, SDL_WINDOW_SHOWN |
														SDL_WINDOW_OPENGL);
	// Cheaks if the window was created, if not, print the error to the console
	if (window == NULL) {
		printf("[-] SDL Window could not open: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	// Create surface:
	// TODO: Learn what a surface is
	surface = SDL_GetWindowSurface(window);

	if (surface == NULL) {
		printf("[-] SDL surface failed to initialize");
		return EXIT_FAILURE;
	}

	// Create renderer:
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {
		printf("[-] SDL Renderer failted to initialize\n");
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);	
	return 0;
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