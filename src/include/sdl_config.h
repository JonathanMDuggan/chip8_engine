#pragma once
#include "chip8_processor.h"
#include <stdint.h>
#include <SDL.h>
typedef struct{
  uint32_t x;
  uint32_t y;
}Location;

typedef struct {
  Location location;
}Mouse;

extern void Chip8_SDLStart(Chip8* chip8);
extern uint8_t SDLInitialize(Chip8* chip8);
extern void Chip8_SDLQuit(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface);