#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
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
	Mouse				  mouse;
	size_t				i = 0;

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

	// We must load images to the chip8 to update the window icon
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("[-] SDL failed to initialise SDL_image" 
					 "chip8 engine will not have a window icon!: %s\n", IMG_GetError());
		// We don't leave with an EXIT_FAILURE since the problem doesn't effect
		// the emulation process
	}

	SDL_SetWindowTitle(window, CHIP8_APPLICATION_NAME);
	
	// Sets the color to blue!
	SDL_RenderSetScale(renderer, 40, 40);
	SDL_SetRenderDrawColor(renderer, 34, 14, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	// Makes the screen look sharper
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, 640, 480);

	// Set the window icon to the chip8_engine logo
	// BIG PROBLEM: I DON'T KNOW WHY, sdl cannot read from a relative path 
	// FOR NO REASON!, so I used the absolute path and it worked somehow?
	// needs to fix! ASAP!.
	SDL_Surface* chip8_engine_window_icon = 
		IMG_Load("I:/C projects/chip8_engine/chip8_engine/resources/chip8_engine_icon.png");
	if (chip8_engine_window_icon == NULL) {
		printf("[-] SDL failed to load window icon: %s\n", IMG_GetError());
		// Countinue running anyway, since it only effects the window and not the
		// emulation
	}
	SDL_SetWindowIcon(window, chip8_engine_window_icon);

	// Handles key press events
	mouse.location.x = 0;
	mouse.location.y = 0;

	while (emulator_is_running){
		while (SDL_PollEvent(&event)) {
			// Only print the location of the mouse when I move it
			if (event.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouse.location.x, &mouse.location.y);
				printf("[i] Mouse location: %d,%d\n", mouse.location.x, mouse.location.y);
				SDL_Delay(30);
			}
			switch (event.type) {
			case SDL_QUIT:
				emulator_is_running = FALSE;
				break;
			case SDL_KEYDOWN:
				printf("[i] Key was pressed: %s\n",
								SDL_GetKeyName(event.key.keysym.sym));
				break;
			}
		}
		SDL_Delay(2);
		i++;
	} 

	printf("[i] SDL shuting down\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(chip8_engine_window_icon);
	SDL_Quit();
	return 0;
}

void SDLQuit(SDL_Window* window, SDL_Renderer* readerer) {
	SDL_DestroyRenderer(readerer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}