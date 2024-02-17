#pragma once 
#include"include/chip8_operators.h"
#include"include/chip8_instruction_set.h"
#include <stdlib.h>
#include <time.h>
// Note: ChatGPT wrote the nibble comments, they were too good to let down

// Shifts the nibbles (4 bits) of a 16-bit hexadecimal number
// to the right by a specified number of places.
inline uint16_t MoveNibbleRight(uint16_t value, uint8_t x_amount_of_places) {
  return value >> (x_amount_of_places * 4);
}
// 000X: Reads the first nibble (4 bits) from a 16-bit hexadecimal number.
inline uint8_t ReadFirstNibble(uint16_t value){
  return (uint8_t)((value & kNibble1BitMask));
}
// 00X0: Reads the second nibble (4 bits) from a 16-bit hexadecimal number.
inline uint8_t ReadSecondNibble(uint16_t value){
  return (uint8_t)((value & kNibble2BitMask) >> 4);
}
// 0X00: Reads the third nibble (4 bits) from a 16-bit hexadecimal number.
inline uint8_t ReadThirdNibble(uint16_t value){
  return (uint8_t)((value & kNibble3BitMask) >> 8);
}
// X000: Reads the fourth nibble (4 bits) from a 16-bit hexadecimal number.
inline uint8_t ReadForthNibble(uint16_t value){
  return (uint8_t)((value & kNibble4BitMask) >> 12);
}
// 00XX: Reads the low byte (8 bits) from a 16-bit hexadecimal number.
inline uint8_t ReadLoByteFromWord(uint16_t value){
  return (uint8_t)(value & 0x00FF);
}
// 0XXX: Reads the lower 12 bits from a 16-bit hexadecimal number.
inline uint16_t Read12bitFromWord(uint16_t value) {
  return (uint16_t)(value & 0x0FFF);
}

// Gets the input from the keyboard and changes the input value
// in the Chip8
void Chip8ProcessInput(Chip8* chip8, SDL_Keycode input){
  printf("user input\n");
  switch (input) {
    case SDLK_1: chip8->input |= kChip8KeyPad1; break;
    case SDLK_2: chip8->input |= kChip8KeyPad2; break;
    case SDLK_3: chip8->input |= kChip8KeyPad3; break;
    case SDLK_4: chip8->input |= kChip8KeyPadC; break;
    case SDLK_q: chip8->input |= kChip8KeyPad4; break;
    case SDLK_w: chip8->input |= kChip8KeyPad5; break;
    case SDLK_e: chip8->input |= kChip8KeyPad6; break;
    case SDLK_r: chip8->input |= kChip8KeyPadD; break;
    case SDLK_a: chip8->input |= kChip8KeyPad7; break;
    case SDLK_s: chip8->input |= kChip8KeyPad8; break;
    case SDLK_d: chip8->input |= kChip8KeyPad9; break;
    case SDLK_f: chip8->input |= kChip8KeyPadE; break;
    case SDLK_z: chip8->input |= kChip8KeyPadA; break;
    case SDLK_x: chip8->input |= kChip8KeyPad0; break;
    case SDLK_c: chip8->input |= kChip8KeyPadB; break;
    case SDLK_v: chip8->input |= kChip8KeyPadF; break;
  }
}
// When the user relase his fingers from the keyboard the chip8 updates the 
// input value
void Chip8ProcessRelease(Chip8* chip8, SDL_Keycode input) {
  printf("user released\n");
  switch (input) {
    case SDLK_1: chip8->input &= ~kChip8KeyPad1; break;
    case SDLK_2: chip8->input &= ~kChip8KeyPad2; break;
    case SDLK_3: chip8->input &= ~kChip8KeyPad3; break;
    case SDLK_4: chip8->input &= ~kChip8KeyPadC; break;
    case SDLK_q: chip8->input &= ~kChip8KeyPad4; break;
    case SDLK_w: chip8->input &= ~kChip8KeyPad5; break;
    case SDLK_e: chip8->input &= ~kChip8KeyPad6; break;
    case SDLK_r: chip8->input &= ~kChip8KeyPadD; break;
    case SDLK_a: chip8->input &= ~kChip8KeyPad7; break;
    case SDLK_s: chip8->input &= ~kChip8KeyPad8; break;
    case SDLK_d: chip8->input &= ~kChip8KeyPad9; break;
    case SDLK_f: chip8->input &= ~kChip8KeyPadE; break;
    case SDLK_z: chip8->input &= ~kChip8KeyPadA; break;
    case SDLK_x: chip8->input &= ~kChip8KeyPad0; break;
    case SDLK_c: chip8->input &= ~kChip8KeyPadB; break;
    case SDLK_v: chip8->input &= ~kChip8KeyPadF; break;
  }
}

// Helper Functions: for operations on register to register and memory reads

void RegisterXBitwiseAndData(uint8_t* register_x, const uint8_t kData){
  *register_x &= kData;
}
void RegisterXBitwiseXorData(uint8_t* register_x, const uint8_t kData){
  *register_x ^= kData;
}
void RegisterXBitwiseOrData(uint8_t* register_x, const uint8_t kData){
  *register_x |= kData;
}
void RegisterXEqualData(uint8_t* register_x, const uint8_t kData){
  *register_x = kData;
}
void RegisterXPlusData(uint8_t* register_x, const uint8_t kData, Chip8* chip8){
  // Before doing the operation we must check if the expreesion will overflow
  // the 8bit register, if it does, we must change the flag register to reflex
  // that.
  uint16_t sum = *register_x + kData;
  if (sum > 255) {
    // The change is done in the instruction set function
    *chip8->_register->status |= 1;
  }
  // If the value didn't overflow, we pass the value and don't change anything
  // in the status register
  *register_x += kData;
}
void RegisterXMinusData(uint8_t* register_x, const uint8_t kData, Chip8 *chip8){
  int16_t difference = *register_x - kData;
  if (difference < 0) {
    // The change is done in the instruction set function
    *chip8->_register->status |= 1;
  }
  // If the value didn't overflow, we pass the value and don't change anything
  // in the status register
  *register_x -= kData;
}


// Higher Order Function: Call when instruction Set function uses Register X
// as the operand for RegisterY (RegisterX (Operation)= RegisterY)
void Chip8RegisterToRegisterOperation(Chip8* chip8, uint16_t memory,
                                      void(*operation)(uint8_t*,const uint8_t)){

  // This function was created because the Chip8 uses four opcodes that do
  // practically the same thing but the operation. Therefore whenever those
  // functions are called, we use this function instead, and pass the operation
  // in the parameters.

  // The Third Nibble in the 16bit memory is the register number

  // Will the function use ^=, |=, &= ?, The helper functions (operation) 
  // determines that. 
  operation(&chip8->_register->general_perpose[ReadThirdNibble(memory)],
             chip8->_register->general_perpose[ReadSecondNibble(memory)]);
}
// Higher Order Function: Call when instruction Set function uses Register X
// as the operand for RegisterY. And! this operation changes the flag register
// if certain conditions have met (RegisterX (Operation)= RegisterY)
// This function is only called for Add and Minus instructions!
void Chip8RegisterToRegisterOperationFlag(Chip8* chip8, uint16_t memory,
  void(*operation)(uint8_t*, const uint8_t, Chip8*)) {

  // Will the function use +=, -=, ?, The helper functions (operation) 
  // determines that. 
  operation(&chip8->_register->general_perpose[ReadThirdNibble(memory)],
    chip8->_register->general_perpose[ReadSecondNibble(memory)],
    chip8);
}
// Higher Order Function: Call when instruction Set function uses Register X
// as the operand for Memory (RegisterX (Operation)= Memory)
void Chip8MemoryRead(Chip8* chip8, uint16_t memory, 
                     void(*operation)(uint8_t*, const uint8_t)){

  // This function was created since reading memory is the same for most opcodes.
  // The Chip8 can only read 8bits of memory data to the registers, therefore
  // we need to read the LoByte of the memory to the register to operate on it

  // Will the function use |=, ^=, &= ?, The helper functions (operation) 
  // determines that. 
  operation(&chip8->_register->general_perpose[ReadThirdNibble(memory)],
             ReadLoByteFromWord(memory));
}
// Higher Order Function: Call when instruction Set function uses Register X
// as the operand for Memory. And! this operation changes the flag register if
// certain conditions are met (RegisterX (Operation)= Memory) This function is
// only called for Add and Minus instructions!
void Chip8MemoryReadFlag(Chip8* chip8, uint16_t memory,
  void(*operation)(uint8_t*, const uint8_t, Chip8*)) {

  // This function was created since reading memory is the same for two opcodes.
  // The Chip8 can only read 8bits of memory data to the registers, therefore
  // we need to read the LoByte of the memory to the register to operate on it

  // Will the function use +=, -=, ?, The helper functions (operation) 
  // determines that. 
  operation(&chip8->_register->general_perpose[ReadThirdNibble(memory)],
    ReadLoByteFromWord(memory),chip8);
}
uint8_t Chip8GetRandom8bitNumber() {
  srand(time(0));
  return (uint8_t)(rand() % 256);
}