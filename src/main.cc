#pragma once

#include "imgui.h"
#include "include/chip8_names.h"
#include "include/chip8_emulation.h"
#include "include/imgui_config.h"

#include <iostream>
#include <cstdlib>
#include <SDL.h>
#include <ctype.h>

int main(int argc, char** argv){
  CHIP8_LOG_INFO("Launching Chip8 Engine in _DEBUG mode!\n");
  SDL sdl;
  // If the application is called in the console, don't load the gui
  // just run the emulation
  if (argc == 2) {
    Chip8_SDLInitialize(&sdl);
    Chip8_Emulate(argv[2], &sdl);
    Chip8_SDLQuit(&sdl);
    return EXIT_SUCCESS;
  }
  if (argc > 2) {
    std::cout << "Too many arguments: chip8_engine [File Path]\n";
    return EXIT_FAILURE;
  }
  // If no arguments were called, launch the IMGUI
  Chip8GUI::Create(&sdl);
  Chip8GUI::Run(&sdl);
  return EXIT_SUCCESS;
}