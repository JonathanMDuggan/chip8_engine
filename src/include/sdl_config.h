#pragma once
#include <stdint.h>
#include <SDL.h>
typedef struct{
  uint32_t x;
  uint32_t y;
}Location;

typedef struct {
  Location location;
}Mouse;
extern void sdlVideoStuff();
extern void SDLStart();
extern uint8_t SDLInitialize();
extern uint8_t SDLCleanUp();
extern void SDLUpdate();
extern void SDLInput();
extern void SDLRendrer(SDL_Renderer* renderer, SDL_Renderer* window);
extern void SDLDraw();
extern void SDLVideo();
extern void SDLQuit(SDL_Window* window, SDL_Renderer* renderer);