#pragma once
#include "chip8_processor.h"
#include <stdint.h>
#include <SDL.h>
typedef struct Location {
  uint32_t x;
  uint32_t y;
}Location;
typedef struct Audio {
  SDL_AudioSpec set;
  SDL_AudioSpec get;
  SDL_AudioDeviceID ID;
  SDL_AudioFormat wav;
  uint32_t length;
  uint8_t* buffer;
  uint8_t* lower_volume_buffer;
} Audio;

typedef struct AudioData {
  uint16_t volume;
  uint16_t freq;
  uint16_t time;
} AudioData;

typedef struct Mouse{
  Location location;
}Mouse;

typedef struct SDL {
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_Renderer* renderer;
  Audio audio;
}SDL;

extern uint8_t Chip8_SDLInitialize(Chip8* chip8, SDL* sdl);
extern void Chip8_SDLQuit(SDL* sdl);
extern void Chip8_SDLRender(Chip8* chip8, SDL* sdl);
extern void Chip8_SDLReadInput(Chip8* chip8, SDL* sdl, uint8_t* emulating);
extern void Chip8_SDLAudioCallback(void* userdata, uint8_t* stream, int len);
extern void Chip8_CreateSquareWave(SDL* sdl);