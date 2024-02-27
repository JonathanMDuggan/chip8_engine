#pragma once
#define _CRT_SECURE_NO_WARNNINGS
#include <stdlib.h>
#include <SDL.h>
#include <ctype.h>
#include "include/chip8_processor.h"
#include "include/chip8_names.h"
#include "include/chip8_emulation.h"
int main(int argc, char** argv){
  CHIP8_LOG_INFO("Launching Chip8 Engine in _DEBUG mode!\n");
  if (Chip8_Emulate("ROM/IBM Logo.ch8") == EXIT_FAILURE) {
    CHIP8_LOG_INFO("FAILED TO EMULATE\n");
  }
  return EXIT_SUCCESS;
} 