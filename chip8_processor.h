// chip8_engine.h : Include file for standard system include files,
// or project specific include files.
#pragma once
#include <stdint.h>
#include <stdio.h>
enum Chip8Constants{
  // Sizes of Registers and Memory
  kChip8MaxNumberOfGeneralPerposeRegisters = 16,
  kChip8MaxRAMSize = 4096, kChip8MaxStackSize = 16,
  // Chip8 screen specs is 64 by 32 pixels
  kChip8ScreenLenght = 64, kChip8ScreenHeight = 32, kChip8FontSetSize = 80,

  // General perpose registers 
  kChip8V0 = 0, kChip8V1 = 1, kChip8V2 = 2, kChip8V3 = 3, kChip8V4 = 4,
  kChip8V5 = 5, kChip8V6 = 6, kChip8V7 = 7, kChip8V8 = 8, kChip8V9 = 9,
  kChip8VA = 10, kChip8VB = 11, kChip8VD = 13, kChip8VE = 14, kChip8VF = 15,

  // Memory Locations
  kChip8StackBase = 0x200, kChip8FontsetAddress = 0x50, 

  // Input Output
  kKeyboard = 16
};

typedef struct {
  uint16_t index;
  uint16_t program_counter;
  uint8_t  stack_pointer;
  uint8_t  general_perpose[kChip8MaxNumberOfGeneralPerposeRegisters];

  // Points to the 16th value in the general perpose array.
  // The 16th register or VF is used as a status register sometimes
  uint8_t* status_register;
  uint8_t  delay_timer;
  uint8_t  sound_timer;
}Register;

typedef struct {
  uint16_t opcode;
  Register* _register;
  uint16_t stack[kChip8MaxStackSize];
  uint8_t memory[kChip8MaxRAMSize];

  // The reason we're using 32 bits numbers has nothing to do with the CHIP-8
  // Specs and has to do with how SDL works. SDL has a RGBA notation, and it
  // based it's colors on bytes, FF being full brightness and 00 being off.
  // By having a 32 bit interger be 0xFFFFFFFF for white and 0x00000000 for 
  // black you can just put the value into the SDL function without needing
  // to translate the CHIP-8 display output.
  uint32_t screen[kChip8ScreenLenght][kChip8ScreenHeight];
  uint8_t fontset[kChip8FontSetSize];
  uint8_t keyboard[kKeyboard];
}Chip8;

extern void Chip8InitializeFontSet(Chip8* chip8);
extern void Chip8InitializeRegisters(Chip8* chip8);