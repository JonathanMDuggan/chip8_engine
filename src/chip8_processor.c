#pragma once
#include "include/chip8_processor.h"
#include <string.h>

void Chip8InitializeRegisters(Chip8* chip8){
  // Set all the general perpose register to 0
  memset(chip8->_register->general_perpose, 0, 
         sizeof(chip8->_register->general_perpose));
  // Set the screen to black
 
  memset(chip8->screen, 0, sizeof(chip8->screen));
  // Set the stack to nothing but zeros 
  memset(chip8->stack, 0, sizeof(chip8->stack));

  // Point to where the programs are in memory at bootup
  chip8->_register->program_counter = kChip8StackBase;
  chip8->_register->stack_pointer   = 0;
  chip8->_register->delay_timer     = 0;
  chip8->_register->sound_timer     = 0;
  chip8->_register->index           = 0;
  chip8->_register->status = &chip8->_register->general_perpose[kChip8VF];
  
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
void Chip8InitializeMemory(Chip8* chip8) {
  // Store the font set in the memory
  Chip8InitializeFontSet(chip8);
  // This is for the nothing space
  memset(&chip8->memory[kChipNothingSpace], 0, 0x180);
  // Set the inputs all to 0
  chip8->input = 0;
}

// Sets the Chip8 font to the 04b_21 standard on runtime
void Chip8InitializeFontSet(Chip8* chip8) {
  size_t i;
  // Used the 04b_21 font for this emulator; 04b_21 is standardized and It's in
  // the public domain. Letters are from 0 to F. The Chip-8 doesn't have a
  // default alphabet. In the declaration, I tried to write the numbers in 0b
  // format, Hope you can read this.
  const uint8_t kChip8FontsetData[] = {
    // 0
    0b00111000,
    0b01001100,
    0b01010100,
    0b01100100,
    0b00111000,
    // 1
    0b00100000,
    0b01100000,
    0b00100000,
    0b00100000,
    0b01110000,
    // 2
    0b01110000,
    0b10001000,
    0b00110000,
    0b01000000,
    0b11111000,
    // 3
    0b11110000,
    0b00001000,
    0b01110000,
    0b00001000,
    0b11110000,
    // 4
    0b10010000,
    0b10010000,
    0b11111000,
    0b00010000,
    0b00010000,
    // 5
    0b11111000,
    0b10000000,
    0b11110000,
    0b00001000,
    0b11110000,
    // 6
    0b01111000,
    0b10000000,
    0b11110000,
    0b10001000,
    0b01110000,
    // 7
    0b11111000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000,
    // 8
    0b01110000,
    0b10001000,
    0b01110000,
    0b10001000,
    0b01110000,
    // 9
    0b01110000,
    0b10001000,
    0b01111000,
    0b00001000,
    0b11110000,
    // A
    0b01110000,
    0b10001000,
    0b11111000,
    0b10001000,
    0b10001000,
    // B
    0b11110000,
    0b10001000,
    0b11110000,
    0b10001000,
    0b11110000,
    // C
    0b01110000,
    0b10001000,
    0b10000000,
    0b10001000,
    0b01110000,
    // D
    0b11110000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b11110000,
    // E
    0b11111000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b11111000,
    // F
    0b11111000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000
  };

  memcpy(chip8->memory, kChip8FontsetData, sizeof(kChip8FontsetData));
  return;
}