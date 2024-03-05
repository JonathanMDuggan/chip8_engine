// chip8_engine.h : Include file for standard system include files,
// or project specific include files.
#pragma once
#include <stdint.h>
#include <stdio.h>
enum Chip8_Constants {
  // Sizes of Registers and Memory
  kChip8MaxNumberOfGeneralPerposeRegisters = 16,
  kChip8MaxRAMSize = 4096, kChip8MaxStackSize = 16,
  // Chip8 screen specs is 64 by 32 pixels
  kChip8ScreenLenght = 64, kChip8ScreenHeight = 32, kChip8FontSetSize = 80,
  kChip8Background = 0, kChip8Foreground = 0xFFFFFFFF,

  // General purpose registers 
  kChip8V0 = 0, kChip8V1 = 1, kChip8V2 = 2, kChip8V3 = 3, kChip8V4 = 4,
  kChip8V5 = 5, kChip8V6 = 6, kChip8V7 = 7, kChip8V8 = 8, kChip8V9 = 9,
  kChip8VA = 10, kChip8VB = 11, kChip8VD = 13, kChip8VE = 14, kChip8VF = 15,

  // Memory Locations
  kChip8StackBase = 0x200, kChip8FontSetAddress = 0x00, kChipNothingSpace = 0x80,
  // Memory Space
  kChip8MaxROMSize = 0xDFE,
  // Input Output
  kKeyboard = 16,

  // program counter
  kChip8NextInstruction = 2, kChip8SkipNextInstruction = 4,

  kChip8InstructionsPerSecond = 700, kChip8ScreenRefreshRate = 60
};

enum Chip8_InputMask {
  kChip8KeyPad0 = 0x0001, kChip8KeyPad1 = 0x0002, kChip8KeyPad2 = 0x0004,
  kChip8KeyPad3 = 0x0008, kChip8KeyPad4 = 0x0010, kChip8KeyPad5 = 0x0020,
  kChip8KeyPad6 = 0x0040, kChip8KeyPad7 = 0x0080, kChip8KeyPad8 = 0x0100,
  kChip8KeyPad9 = 0x0200, kChip8KeyPadA = 0x0400, kChip8KeyPadB = 0x0800,
  kChip8KeyPadC = 0x1000, kChip8KeyPadD = 0x2000, kChip8KeyPadE = 0x4000,
  kChip8KeyPadF = 0x8000, kChip8KeyPadNull = 0xCC
};
typedef struct Register{
  uint16_t index;
  uint16_t program_counter;
  uint8_t  stack_pointer;
  uint8_t  general_purpose[kChip8MaxNumberOfGeneralPerposeRegisters];

  // Points to the 16th value in the general perpose array.
  // The 16th register or VF is used as a status register sometimes
  uint8_t* status;
  uint8_t  delay_timer;
  uint8_t  sound_timer;
}Register;

typedef struct Chip8{
  uint16_t opcode;
  Register* reg;
  uint16_t stack[kChip8MaxStackSize];
  uint8_t memory[kChip8MaxRAMSize];

  // The reason we're using 32 bits numbers has nothing to do with the CHIP-8
  // Specs and has to do with how SDL works. SDL has a RGBA notation, and it
  // based it's colors on bytes, FF being full brightness and 00 being off.
  // By having a 32 bit interger be 0xFFFFFFFF for white and 0x00000000 for 
  // black you can just put the value into the SDL instruction without needing
  // to translate the CHIP-8 display output.
  uint32_t screen[kChip8ScreenLenght][kChip8ScreenHeight];
  // Sets to 1 when the DRW instruction was called,
  // Sets to 0 when SDL Render drew to the screen.
  uint8_t draw_flag;
  // This is a 16bit variable that stores bools of 16 inputs
  uint16_t input;
}Chip8;

extern void Chip8_InitializeFontSet(Chip8* chip8);
extern void Chip8_InitializeRegisters(Chip8* chip8);
extern void Chip8_InitializeMemory(Chip8* chip8);