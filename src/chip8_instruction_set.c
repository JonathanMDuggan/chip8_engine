#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "include/sdl_config.h"
#include "include/chip8_processor.h"
#include "include/chip8_instruction_set.h"
#include "include/chip8_operators.h"

// Instruction Set functions:
// 
// Unconventional instruction naming convention, so here is the explaination: 
// 
// The number at the end of every instruction repersents the opcodes
// hexadecimal value. Whenever you see any letters outside of the hexadecimal
// space ( letter after F) that being kNNN, x, or kk it means the following...
// 
// * kNNN: The value kNNN is the address to the a location in opcode
// 
// * kk:  Store that value into Register 'X', 'X' being any of the 16 register
//        values in the Chip-8
// 
// * x:   The hexadecimal value at that location is the register number,
//        for example, if the opcode had A at that x location that means the
//        register is A.
// 
// * y:   This means the opcode is calling a second register, the hexidecimal
//        located at y is the second register number.
// 
// Therefore if you see the instruction 'Chip8_AddMemoryToRegisterX_7xkk' it means
// Add the value at 'kk' to Register 'x' 
// 
// Note: the reason we increament the Program counter by 2 each time is because
// The chip8 reads 16bit numbers, we must take 
//
//

// Set the display opcode to zero

void Chip8_ClearDisplay_00E0(Chip8* chip8, uint16_t memory){
  memset(chip8->screen, 0, sizeof(chip8->screen));
  chip8->reg->program_counter += kChip8NextInstruction;
}
// Return from subroutine Chip8 instruction
void Chip8_Return_00EE(Chip8* chip8, uint16_t memory){
  // You want to decrement the stack pointer when returning from the subroutine
  // ,if you don't, the program counter will read the wrong address, or worse,
  // go to address 0 since it never wrote an address there.
  chip8->reg->stack_pointer--;
  // Make the program counter point to where it was before 
  chip8->reg->program_counter = chip8->stack[chip8->reg->stack_pointer];
  chip8->reg->program_counter += kChip8NextInstruction;
}
void Chip8_JumpToLocation_1nnn(Chip8* chip8, uint16_t memory){
  // Go to that location in opcode.
  const uint16_t kNNN = Chip8_Read12bitFromWord(memory);
  chip8->reg->program_counter = kNNN;
}

// Function call Chip8 instruction
void Chip8_Call_2nnn(Chip8* chip8, uint16_t memory){
  const uint16_t kNNN = Chip8_Read12bitFromWord(memory);
  // Store the opcode address where the program counter was before going to the
  // subroutine, that way when the program reads the call instruction, it'll
  // go back where it came from
  chip8->stack[chip8->reg->stack_pointer] = chip8->reg->program_counter;
  // Set the program counter to the new opcode address where the subroutine is
  chip8->reg->program_counter = kNNN;
  chip8->reg->stack_pointer++;
}
void Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk(Chip8* chip8,
                                                         uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kMemory = Chip8_ReadLoByteFromWord(memory);
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
    
  if (kRegisterX == kMemory) {
      chip8->reg->program_counter += kChip8SkipNextInstruction;
      return;
  }
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk(
      Chip8* chip8, uint16_t memory) {

  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kData = Chip8_ReadLoByteFromWord(memory);

  if (chip8->reg->general_purpose[kVx] != kData) {
    chip8->reg->program_counter += kChip8SkipNextInstruction;
    return;
  }

  chip8->reg->program_counter += kChip8NextInstruction;
}
void Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0(Chip8* chip8, 
                                                           uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);

  if (chip8->reg->general_purpose[kVx] == chip8->reg->general_purpose[kVy]){
    chip8->reg->program_counter += kChip8NextInstruction;
    return;
  }
  chip8->reg->program_counter += kChip8NextInstruction;
}
void Chip8_LoadMemoryToRegisterX_6xkk(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kKK = Chip8_ReadLoByteFromWord(memory);
  chip8->reg->general_purpose[kVx] = kKK;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_AddMemoryToRegisterX_7xkk(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kKK = Chip8_ReadLoByteFromWord(memory);
  chip8->reg->general_purpose[kVx] += kKK;
  chip8->reg->program_counter += kChip8NextInstruction;
}

// kRegisterX = kRegisterY
void Chip8_LoadRegisterYToRegsiterX_8xy0(Chip8* chip8, uint16_t memory) {
  Chip8_RegisterToRegisterOperation(chip8, memory, Chip8_RegisterXEqualData);
  chip8->reg->program_counter += kChip8NextInstruction;
}

// Bitwise register operations opcodes

// kRegisterX OR= kRegisterY
void Chip8_BitwiseOrRegisterXByRegisterY_8xy1(Chip8* chip8, uint16_t memory) {
  Chip8_RegisterToRegisterOperation(chip8, memory,
                                    Chip8_RegisterXBitwiseOrData);
  chip8->reg->program_counter += kChip8NextInstruction;
}
// kRegisterX AND= kRegisterY
void Chip8_BitwiseAndRegisterXByRegisterY_8xy2(Chip8* chip8, uint16_t memory) {
  Chip8_RegisterToRegisterOperation(chip8, memory,
                                    Chip8_RegisterXBitwiseAndData);
  chip8->reg->program_counter += kChip8NextInstruction;
}
// kRegisterX XOR= kRegisterY
void Chip8_BitwiseXorRegisterXByRegisterY_8xy3(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterY = chip8->reg->general_purpose[kVy];

  chip8->reg->general_purpose[kVx] ^= kRegisterY;
  chip8->reg->program_counter += kChip8NextInstruction;
}

// Operators on registers opcodes

// kRegisterX += kRegisterY
void Chip8_AddRegisterXByRegisterY_8xy4(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterY = chip8->reg->general_purpose[kVy];
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
  const uint16_t kResult = kRegisterX + kRegisterY;
  // Check if the 8bit number overflowed
  if (kResult > 255) {
    *chip8->reg->status = 1;
  } else {
    *chip8->reg->status = 0;
  }
  chip8->reg->general_purpose[kVx] += kRegisterY;
  chip8->reg->program_counter += kChip8NextInstruction;
}

// kRegisterX -= kRegisterY
void Chip8_SubRegisterXByRegisterY_8xy5(Chip8* chip8, uint16_t memory){
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterY = chip8->reg->general_purpose[kVy];
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
  const uint16_t kResult = kRegisterX - kRegisterY;
  // Check if the 8bit number underflowed
  if (kRegisterX >= kRegisterY) {
    *chip8->reg->status = 1;
  } else {
    *chip8->reg->status = 0;
  }
  chip8->reg->general_purpose[kVx] -= kRegisterY;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_ShiftRegisterXRight_8xy6(Chip8* chip8, uint16_t memory){
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
  const uint8_t kLeastSignificantBit = kRegisterX & 0x0001;

  *chip8->reg->status = kLeastSignificantBit;
  // This is divison by 2!. This is how it's done in the Chip 8
  chip8->reg->general_purpose[kVx] >>= 1;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_SubtractRegisterYbyRegisterX_8xy7(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterY = chip8->reg->general_purpose[kVy];
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];

  if (kRegisterY >= kRegisterX) {
    *(chip8->reg->status) = 1;
  } else {
    *(chip8->reg->status) = 0;
  }

  chip8->reg->general_purpose[kVx] = kRegisterY - kRegisterX;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_ShiftRegisterXLeft_8xyE(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
  const uint8_t kMostSignificantBit = kRegisterX & 0x7FFF;

  *chip8->reg->status = kMostSignificantBit;
  // This is the power of 2!. This is how it's done in the Chip 8
  chip8->reg->general_purpose[kVx] <<= 1;
  chip8->reg->program_counter += kChip8NextInstruction;
}

// I understand I could of created a for loop which bit shifted a word by one
// every index, but I don't care.

void Chip8_SkipIfKeyIsPressed_Ex9E(Chip8* chip8, uint16_t memory){
  uint16_t chip8_input_lookup_table[kKeyboard] = {
      kChip8KeyPad0, kChip8KeyPad1, kChip8KeyPad2, kChip8KeyPad3,
      kChip8KeyPad4, kChip8KeyPad5, kChip8KeyPad6, kChip8KeyPad7,
      kChip8KeyPad8, kChip8KeyPad9, kChip8KeyPadA, kChip8KeyPadB,
      kChip8KeyPadC, kChip8KeyPadE, kChip8KeyPadF};

  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kRegX = chip8->reg->general_purpose[kVx];

  if ((chip8_input_lookup_table[kRegX] & chip8->input) != 0) {

    chip8->reg->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->reg->program_counter += kChip8NextInstruction;
}

// I understand I could of created a for loop which bit shifted a word by one
// every index, but I don't care. 

void Chip8_SkipIfKeyIsNotPressed_ExA1(Chip8* chip8, uint16_t memory){
  uint16_t input_lookup_table[kKeyboard] = {
      kChip8KeyPad0, kChip8KeyPad1, kChip8KeyPad2, kChip8KeyPad3,
      kChip8KeyPad4, kChip8KeyPad5, kChip8KeyPad6, kChip8KeyPad7,
      kChip8KeyPad8, kChip8KeyPad9, kChip8KeyPadA, kChip8KeyPadB,
      kChip8KeyPadC, kChip8KeyPadE, kChip8KeyPadF};

  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kRegX = chip8->reg->general_purpose[kVx];

  if ((input_lookup_table[kRegX] & chip8->input) == 0) {
    chip8->reg->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_RegisterEqualDelayTimer_Fx07(Chip8* chip8, uint16_t memory){
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  chip8->reg->general_purpose[kVx] = chip8->reg->delay_timer;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_StoreKeyPressInRegisterX_Fx0A(Chip8* chip8, uint16_t memory){
  // If input = 0, that means no keys are pressed during at the instruction call
  if (chip8->input != 0) {

    chip8->reg->general_purpose[Chip8_ReadThirdNibble(memory)] =
        Chip8ConvertInputToHex(chip8->input);

    chip8->reg->program_counter += kChip8NextInstruction;
  }
}

void Chip8_DelayTimerEqualRegisterX_Fx15(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];

  chip8->reg->delay_timer = kRegisterX;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_SetSoundTimerToRegisterX_Fx18(Chip8* chip8, uint16_t memory){
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  chip8->reg->sound_timer = chip8->reg->general_purpose[kVx];
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_IndexPlusRegisterX_Fx1E(Chip8* chip8, uint16_t memory){
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  chip8->reg->index += chip8->reg->general_purpose[kVx];
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_IndexEqualsRegisterX_Fx29(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  chip8->reg->index = chip8->reg->general_purpose[kVx] * 5;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_BCDConversion_Fx33(Chip8* chip8,
                              uint16_t opcode) {
  const uint8_t kVx = Chip8_ReadThirdNibble(opcode);
  const uint16_t kIndex = chip8->reg->index;
  const uint8_t register_x = chip8->reg->general_purpose[kVx];
  const uint8_t one = register_x % 10;
  const uint8_t ten = (register_x / 10) % 10;
  const uint8_t hundred = register_x / 100;

  chip8->memory[kIndex] = hundred;
  chip8->memory[kIndex + 1] = ten;
  chip8->memory[kIndex + 2] = one;

  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_SkipIfRegisterXDoesNotEqualRegisterY_9xy0(Chip8* chip8,
                                                          uint16_t memory){
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kRegisterX = chip8->reg->general_purpose[kVx];
  const uint8_t kRegisterY = chip8->reg->general_purpose[kVy];

  if (kRegisterX != kRegisterY){
    chip8->reg->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_StoreMemoryInIndexRegister_Annn(Chip8* chip8, uint16_t opcode){

  const uint16_t kAddress = Chip8_Read12bitFromWord(opcode);
  chip8->reg->index = kAddress;
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn(Chip8*   chip8,
                                                   uint16_t memory){
  const uint8_t kRegister0 = chip8->reg->general_purpose[0];
  const uint16_t kValue = Chip8_Read12bitFromWord(memory);
  chip8->reg->program_counter = kValue + kRegister0;
}

void Chip8_SetRegisterXToRandomByteANDMemory_Cxkk(Chip8* chip8,
                                                  uint16_t memory) {

  const uint8_t kRamdomNumber = Chip8_GetRandom8bitNumber();
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kKK = Chip8_ReadLoByteFromWord(memory);
  const uint8_t result = kRamdomNumber & kKK;

  chip8->reg->general_purpose[kVx] = result;
  chip8->reg->program_counter += kChip8NextInstruction;
}
// Stores the kKK from opcode to V0 to VX starting at the opcode index pointed
// by the index register
void Chip8_IndexStoreIterator_Fx55(Chip8* chip8, uint16_t opcode) { 
  const uint8_t kRegisterIterator = Chip8_ReadThirdNibble(opcode) + 1;
  const uint16_t kIndex = chip8->reg->index;

  for (size_t i = 0; i < kRegisterIterator; i++) {
    chip8->memory[kIndex + i] = chip8->reg->general_purpose[i];
  }
  chip8->reg->index += kRegisterIterator + 1;
  chip8->reg->program_counter += kChip8NextInstruction;
}

// Fills all general perpose registers with opcode addresses starting at the
// opcode address stored in the index register, then add  X + 1 to the
// index register
void Chip8_IndexRegisterFill_Fx65(Chip8* chip8, uint16_t opcode) {
  const uint8_t kRegisterIterator = Chip8_ReadThirdNibble(opcode) + 1;
  const uint16_t kIndex = chip8->reg->index;
  size_t i;
  for (i = 0; i < kRegisterIterator; i++) {
    chip8->reg->general_purpose[i] = chip8->memory[i + kIndex];
  }
  chip8->reg->index += kRegisterIterator + 1;
  chip8->reg->program_counter += kChip8NextInstruction;
}
// I know this will be the hardest function to create, I have no idea
// What i'm doing
void Chip8_Display_Dxyn(Chip8* chip8, uint16_t memory) {
  const uint8_t kVx = Chip8_ReadThirdNibble(memory);
  const uint8_t kVy = Chip8_ReadSecondNibble(memory);
  const uint8_t n = Chip8_ReadFirstNibble(memory);

  uint8_t x = chip8->reg->general_purpose[kVx] % kChip8ScreenLength;
  uint8_t y = chip8->reg->general_purpose[kVy] % kChip8ScreenHeight;

  *(chip8->reg->status) = 0;
  uint8_t is_pixel_on = 0;

  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < 8; j++) {

      is_pixel_on = chip8->memory[chip8->reg->index + i] & (1 << (7 - j));
      
      if (is_pixel_on != 0) {
        if (chip8->screen[(x + j) % 64][(y + i) % 32] == kChip8Foreground) {
          *chip8->reg->status = 1;
        }
        chip8->screen[(x + j) % 64][(y + i) % 32] ^= kChip8Foreground;
      }

      if (x > kChip8ScreenLength) {
        break;
      }
    }

    if (y > kChip8ScreenHeight) {
      break;
    }

  }
  chip8->draw_flag = TRUE;
  chip8->reg->program_counter += kChip8NextInstruction;
}
  // 250 285
// The opcode parameter is here because almost all instruction are acessed by
// pointer through an array, and by default all kKK is sent via chip8 and 
// opcode, if this instruction didn't have opcode in it's parameters, the program
// will crash
void Chip8_NOP(Chip8* chip8, uint16_t memory) {
  chip8->reg->program_counter += kChip8NextInstruction;
}

void Chip8_Timers(Chip8* chip8, SDL* sdl) {

  if (chip8->reg->delay_timer != 0) {
    chip8->reg->delay_timer--; 
  }
  if (chip8->reg->sound_timer != 0) {
    SDL_PauseAudioDevice(sdl->audio.ID, 0);
    chip8->reg->sound_timer--;
    return;
  }
  SDL_PauseAudioDevice(sdl->audio.ID, 1);
}