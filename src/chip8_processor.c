#pragma once
#include "include/chip8_processor.h"
#include <string.h>

void Chip8_InitializeRegisters(Chip8* chip8){
  // Set all the general perpose register to 0
  memset(chip8->reg->general_purpose, 0, 
         sizeof(chip8->reg->general_purpose));
  // Set the screen to black
 
  memset(chip8->screen, 0, sizeof(chip8->screen));
  // Set the stack to nothing but zeros 
  memset(chip8->stack, 0, sizeof(chip8->stack));

  // Point to where the programs are in memory at bootup
  chip8->reg->program_counter = kChip8StackBase;
  chip8->reg->stack_pointer   = 0;
  chip8->reg->delay_timer     = 0;
  chip8->reg->sound_timer     = 0;
  chip8->reg->index           = 0;
  chip8->reg->status = &chip8->reg->general_purpose[kChip8VF];
  
  return;
}
// Initialize memory space for chip8 4k Memory layout
// +------------------+ 0xFFF
// | Chip-8 Program   |
// | Data Space       |
// |                  |
// |                  |
// |                  |
// |                  |
// |                  |
// |                  |
// |                  |
// |                  |
// |                  |
// +------------------+ 0x200
// | Nothing Space    |
// +------------------+ 0x080
// | Font Set Space   |
// |                  |
// +------------------+ 0x000
void Chip8_InitializeMemory(Chip8* chip8) {
  // Store the font set in the memory
  Chip8_InitializeFontSet(chip8);
  // This is for the nothing space
  memset(&chip8->memory[kChipNothingSpace], 0, 0x180);
  // Set the inputs all to 0
  chip8->input = 0;
}

// Sets the Chip8 font in memory
void Chip8_InitializeFontSet(Chip8* chip8) {

  const uint8_t kChip8FontsetData[] = {
    // 0
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
    // 1
    0b01000000,
    0b11000000,
    0b01000000,
    0b01000000,
    0b11100000,
    // 2
    0b11110000,
    0b00010000,
    0b11110000,
    0b10000000,
    0b11110000,
    // 3
    0b11110000,
    0b00010000,
    0b11110000,
    0b00010000,
    0b11110000,
    // 4
    0b10010000,
    0b10010000,
    0b11110000,
    0b00010000,
    0b00010000,
    // 5
    0b11110000,
    0b10000000,
    0b11110000,
    0b00010000,
    0b11110000,
    // 6
    0b11110000,
    0b10000000,
    0b11110000,
    0b10010000,
    0b11110000,
    // 7
    0b11110000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b01000000,
    // 8
    0b11110000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b11110000,
    // 9
    0b11110000,
    0b10010000,
    0b11110000,
    0b00010000,
    0b11110000,
    // A
    0b11110000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,
    // B
    0b11100000,
    0b10010000,
    0b11100000,
    0b10010000,
    0b11100000,
    // C
    0b11110000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11110000,
    // D
    0b11100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11100000,
    // E
    0b11110000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b11110000,
    // F
    0b11110000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000
  };

  memcpy(chip8->memory, kChip8FontsetData, sizeof(kChip8FontsetData));
  return;
}