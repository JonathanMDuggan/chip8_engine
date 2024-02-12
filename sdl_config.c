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
	SDL_Window* window;
	SDL_Renderer* renderer;
	int frameCount, timerFPS, lastFrame, fps;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1); {
		printf("[-] SDL could not initialize subsystem: %s.\n", SDL_GetError());
		return 1;
	}
	printf("[+] SDL Initialized.\n");
	printf("[i] SDL Setting up video\n");
	SDLVideo(window);
	SDLRendrer(renderer, window);

	// This function call must happen before any other sdl function is 
	return 0;
}
// Create window for the Chip 8 emulator
void SDLVideo(SDL_Surface* window) {
	// Initialzing the values to properties of the window
	window = SDL_CreateWindow(CHIP8_APPLICATION_NAME,    SDL_WINDOWPOS_UNDEFINED,
														SDL_WINDOWPOS_UNDEFINED,   kChip8DefaultWindowLength,
														kChip8DefaultWindowHeight, SDL_WINDOW_FULLSCREEN | 
														SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf("[-] SDL Window failed to initialize\n");
		return;
	}
	printf("[+] SDL Window initialized .\n");
}

void SDLUpdate() {

}

void SDLInput() {

}

void SDLRendrer(SDL_Renderer* renderer, SDL_Surface* window){
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		printf("[-] SDL Renderer failted to initialize\n");
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
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