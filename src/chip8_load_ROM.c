#pragma once
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include <stdio.h>
#include <stdlib.h>
// Reads Chip8 ROM from OS
uint8_t Chip8ReadFile(Chip8* chip8, const char* file_name) {
  // NOTE TO THE PROGRAMMER: 
  // I haven't used the read file functions from C in a long time, so I'm
  // commmenting the process 
  FILE* file = fopen(file_name, "rb");
  // Check if the file has anything inside it.
  if (file == NULL) {
    printf("Could not find file name: %s", file_name);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}