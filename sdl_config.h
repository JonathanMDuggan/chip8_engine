#pragma once
#include <stdint.h>
#include <SDL.h>
extern void sdlVideoStuff();
extern void SDLStart();
extern uint8_t SDLInitialize();
extern uint8_t SDLCleanUp();
extern void SDLUpdate();
extern void SDLInput();
extern void SDLRendrer(SDL_Renderer* renderer, SDL_Surface* window);
extern void SDLDraw();
extern void SDLVideo(SDL_Surface* window);