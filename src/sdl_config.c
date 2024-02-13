#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "include/sdl_config.h"
#include "include/chip8_names.h"
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
	SDL_Window*   window = NULL;
	SDL_Renderer* renderer = NULL;
	uint8_t       emulator_is_running = TRUE;
	SDL_Event     event;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[-] SDL could not initialize");
		return EXIT_FAILURE;
	}
	// Creates a window instance
	// Cheaks if the window was created, if not, print the error to the console
	SDL_CreateWindowAndRenderer(kChip8DefaultWindowLength,
															kChip8DefaultWindowHeight,
															SDL_WINDOW_SHOWN, &window, &renderer);
	// Doing this will casue the window to not have a name for less than a second,
	// but I don't think I care to change this.

	// Check if the window and renderer were created, if not, print the error to
	// the console
	if (window == NULL) {
		printf("[-] SDL window could not open: %s\n", SDL_GetError());
		SDLQuit(window, renderer);
		return EXIT_FAILURE;
	}
	if (renderer == NULL) {
		printf("[-] SDL renderer failed to initialize: %s\n", SDL_GetError());
		SDLQuit(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_SetWindowTitle(window, CHIP8_APPLICATION_NAME);
	// Sets the color to blue!
	SDL_SetRenderDrawColor(renderer, 22, 0, 65, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// Makes the screen look sharper
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, 640, 480);


	while (emulator_is_running){
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				emulator_is_running = FALSE;
			}
		}
	} 

	printf("[i] SDL shuting down\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(window, renderer);
	return 0;
}
// Create window for the Chip 8 emulator

void SDLUpdate() {

}

void SDLInput() {

}

void SDLQuit(SDL_Window* window, SDL_Renderer* readerer) {
	SDL_DestroyRenderer(readerer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void SDLRendrer(SDL_Renderer* renderer, SDL_Surface* window){

}

void SDLDraw() {

}