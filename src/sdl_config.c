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
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* surface = NULL;
	uint8_t emulator_is_running = TRUE;
	SDL_Event event;

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
		printf("[-] SDL window could not open: %s\n", SDL_GetError());
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	if (renderer == NULL) {
		printf("[-] SDL renderer failed to initialize\n");
	}

	do{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				emulator_is_running = FALSE;
			}
		}
	} while (emulator_is_running);

	printf("[i] SDL shuting down\n");
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
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