#pragma once
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include <stdio.h>
#include <stdlib.h>
// Reads Chip8 ROM from OS
uint8_t Chip8_ReadFile(Chip8* chip8, const char* file_name) {
  uint64_t file_size = 0;
  size_t bytes_read  = 0;
  // NOTE TO THE PROGRAMMER: 
  // I haven't used the read file functions from C in a long time, so I'm
  // commmenting the process 
  FILE* file = fopen(file_name, "rb");
  // Check if the file has anything inside it.
  if (file == NULL) {
    printf("Could not find file name: %s\n", file_name);
    return EXIT_FAILURE;
  }
  // check the size of the file before puting it into the chip8 ram
  file_size = ftell(file);

  if (file_size > kChip8MaxROMSize) {
    printf("File is too large: %s\n", file_name);
    return EXIT_FAILURE;
  }

  bytes_read = fread(&chip8->memory[kChip8StackBase], 1,
                    sizeof(chip8->memory), file);
  fclose(file);
  if (bytes_read == 0) {
    printf("Failed to read file: %s\n", file_name);
  }
  return EXIT_SUCCESS;
}