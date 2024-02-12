#pragma once
#include "chip8_processor.h"

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

  for (i = 0; i < kChip8FontSetSize; i++){
    chip8->fontset[i] = kChip8FontsetData[i];
  }
  return;
}

void Chip8InitializeRegisters(Chip8* chip8){
  size_t i;
  chip8->_register->program_counter = kChip8StackBase;
  // Set all the general perpose register to 0
  for (i = 0; i < kChip8MaxNumberOfGeneralPerposeRegisters; i++){
    chip8->_register->general_perpose[i] = 0;
  }

  chip8->_register->stack_pointer = 0;
  Chip8InitializeFontSet(chip8);
  return;
}