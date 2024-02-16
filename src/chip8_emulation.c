#pragma once
#include "include/chip8_emulation.h"
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include <stdio.h>
#include <stdlib.h>
uint8_t Chip8EmulationMainLoop(const char* file_name) {
  Chip8* chip8 = NULL;
  uint16_t opcode_function_lookup_table[0xFFFF];
  // Reset Chip8 CPU
  Chip8InitializeRegisters(chip8);
  // Clear the Chip8 Memory
  Chip8InitializeMemory(chip8);
  // Read the Chip8 Rom and store it in the process section of memory
  if (Chip8ReadFile(chip8, file_name) == 1) {
    return EXIT_FAILURE;
  }
  // Push the opcodes function pointers in memory
}