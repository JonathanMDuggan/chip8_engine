#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL_image.h>

#include "include/sdl_config.h"
#include "include/chip8_names.h"
#include "include/chip8_processor.h"
#include "include/chip8_operators.h"

void Chip8_SDLStart(Chip8* chip8) {
  CHIP8_SDL_LOG_INFO("[i] SDL Initializing.\n");
	SDLInitialize(chip8);
}
// returns 1 if the process wasn't able to compete
uint8_t SDLInitialize(Chip8* chip8) {
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_Surface* chip8_engine_window_icon = NULL;
  uint8_t emulator_is_running = TRUE;
  SDL_Event event;
  Mouse mouse;
  size_t i = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    CHIP8_SDL_LOG_ERROR("could not initialize\n");
    return EXIT_FAILURE;
  }
  // Creates a window instance
  // Cheaks if the window was created, if not, print the error to the console
  SDL_CreateWindowAndRenderer(kChip8DefaultWindowLength,
                              kChip8DefaultWindowHeight, SDL_WINDOW_SHOWN,
                              &window, &renderer);
  // Doing this will casue the window to not have a name for less than a second,
  // but I don't think I care to change this.

  // Check if the window and renderer were created, if not, print the error to
  // the console
  if (window == NULL) {
    CHIP8_SDL_LOG_ERROR("window could not open: %s\n", SDL_GetError());
    Chip8_SDLQuit(window, renderer, chip8_engine_window_icon);
    return EXIT_FAILURE;
  }
  if (renderer == NULL) {
    CHIP8_SDL_LOG_ERROR("renderer failed to initialize: %s\n", SDL_GetError());
    Chip8_SDLQuit(window, renderer, chip8_engine_window_icon);
    return EXIT_FAILURE;
  }

  // We must load images to the chip8 to update the window icon
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    CHIP8_SDL_LOG_ERROR(
        "failed to initialise SDL_image"
        "chip8 engine will not have a window icon!: %s\n",
        IMG_GetError());
    // We don't leave with an EXIT_FAILURE since the problem doesn't effect
    // the emulation process
  }

  SDL_SetWindowTitle(window, CHIP8_APPLICATION_NAME);

  SDL_RenderSetScale(renderer, 40, 40);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  // Makes the screen look sharper
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(renderer, 640, 480);

  // Load the chip8 engine icon to the window
  chip8_engine_window_icon = IMG_Load("resources/chip8_engine_icon.png");
  if (chip8_engine_window_icon == NULL) {
    CHIP8_SDL_LOG_ERROR("failed to load window icon: %s\n", IMG_GetError());
    // Continue running anyway, since it only effects the window and not the
    // emulation
  }
  SDL_SetWindowIcon(window, chip8_engine_window_icon);
  // Handles key press events

  mouse.location.x = 0;
  mouse.location.y = 0;
  char last_key[20] = "test1";
  char current_key[20] = "test2"; 

  while (emulator_is_running) {

    while (SDL_PollEvent(&event)) {
      // Only print the location of the mouse when I move it

      if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&mouse.location.x, &mouse.location.y);
        CHIP8_SDL_LOG_EXTRA_INFO("Mouse location: %d,%d\n", mouse.location.x,
               mouse.location.y);
        SDL_Delay(30);
      }

      switch (event.type) {
        case SDL_QUIT:
          emulator_is_running = FALSE;
          break;

        case SDL_KEYDOWN:

          strcpy(current_key, SDL_GetKeyName(event.key.keysym.sym));

          // We do this since we do not want the inputs to spam the console
          // We only care if a new key was pressed, not if the same one was
          // entered again
          if (strcmp(last_key, current_key) == 0) {
            break;
          }
          
          CHIP8_SDL_LOG_INFO("Key was pressed: %s : %d\n",
                              SDL_GetKeyName(event.key.keysym.sym), 
                              event.key.keysym.sym);

          if (strlen(SDL_GetKeyName(event.key.keysym.sym)) == 1) {
            strcpy(last_key, SDL_GetKeyName(event.key.keysym.sym));
            Chip8_ProcessInput(chip8, event.key.keysym.sym);
            break;
          }

        case SDL_KEYUP:
          if (strlen(SDL_GetKeyName(event.key.keysym.sym)) == 1) {
            Chip8_ProcessRelease(chip8, event.key.keysym.sym);
            break;
          }
      }

      // We must clear the screen and draw it every emulated frame
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      // Now we do our little drawing
      // NOTE TO PRORGAMMER: This is my first time using SDL
      // 
      // To renderer something on the screen you MUST chose a color before you
      // Draw a line.
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawLine(renderer, 5, 5, 100, SDL_ALPHA_OPAQUE);
      // This shows what we've drawn
      
      SDL_RenderPresent(renderer);
    }
    SDL_Delay(2);
    
  }

  CHIP8_SDL_LOG_INFO("shuting down\n");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_FreeSurface(chip8_engine_window_icon);
  SDL_Quit();
  IMG_Quit();
  return 0;
}

void Chip8_SDLQuit(SDL_Window* window, SDL_Renderer* readerer, SDL_Surface* surface) {
	SDL_DestroyRenderer(readerer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surface);
	SDL_Quit();
	IMG_Quit();
}