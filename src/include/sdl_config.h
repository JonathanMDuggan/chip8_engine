#pragma once
#include "chip8_processor.h"
#include <stdint.h>
#include <SDL.h>
typedef struct Location {
  uint32_t x;
  uint32_t y;
}Location;

typedef struct Mouse{
  Location location;
}Mouse;

typedef struct SDL {
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_Renderer* renderer;
}SDL;

extern uint8_t Chip8_SDLInitialize(Chip8* chip8, SDL* sdl);
extern void Chip8_SDLQuit(SDL* sdl);
extern void Chip8_SDLRender(Chip8* chip8, SDL* sdl);
extern void Chip8_SDLReadInput(Chip8* chip8, SDL* sdl, uint8_t* emulating);
