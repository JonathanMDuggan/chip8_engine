#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/chip8_processor.h"
#include "include/chip8_instruction_set.h"
#include "include/chip8_operators.h"

// Instruction Set functions:
// 
// Unconventional function naming convention, so here is the explaination: 
// 
// The number at the end of every function repersents the opcodes
// hexadecimal value. Whenever you see any letters outside of the hexadecimal
// space ( letter after F) that being nnn, x, or kk it means the following...
// 
// * nnn: The value nnn is the address to the a location in memory
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
// Therefore if you see the function 'Chip8_AddMemoryToRegisterX_7xkk' it means
// Add the value at 'kk' to Register 'x' 
// 
// Note: the reason we increament the Program counter by 2 each time is because
// The chip8 reads 16bit numbers, we must take 
//
//

// Set the display memory to zero
void Chip8_ClearDisplay_00E0(Chip8* chip8, uint16_t memory){
  memset(chip8->screen, 0, sizeof(chip8->screen));
  chip8->_register->program_counter += kChip8NextInstruction;
}
// Return from subroutine Chip8 instruction
void Chip8_Return_00EE(Chip8* chip8, uint16_t memory){
  // You want to decrement the stack pointer when returning from the subroutine
  // ,if you don't, the program counter will read the wrong address, or worse,
  // go to address 0 since it never wrote an address there.
  chip8->_register->stack_pointer--;
  // Make the program counter point to where it was before 
  chip8->_register->program_counter = 
    chip8->stack[chip8->_register->stack_pointer];
}
void Chip8_JumpToLocation_1nnn(Chip8* chip8, uint16_t memory){
  // Go to that location in memory.
  chip8->_register->program_counter = Chip8_Read12bitFromWord(memory);
}

// Function call Chip8 instruction
void Chip8_Call_2nnn(Chip8* chip8, uint16_t memory){
  // Store the memory address where the program counter was before going to the
  // subroutine, that way when the program reads the call instruction, it'll
  // go back where it came from
  chip8->stack[chip8->_register->stack_pointer] =
    chip8->_register->program_counter;
  // Set the program counter to the new memory address where the subroutine is
  chip8->_register->program_counter = Chip8_Read12bitFromWord(memory);
  // increment the stack pointer by one, so it points to a new area in the stack
  // where it can put the memory address of another subruntine and doesn't
  // overide the address where we came from
  chip8->_register->program_counter += kChip8NextInstruction;
}
  void Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk(Chip8* chip8,
                                                          uint16_t memory){
    if (
      chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] ==
      Chip8_ReadLoByteFromWord(memory)
    ) {
      chip8->_register->program_counter += kChip8SkipNextInstruction;
      return;
    }
    chip8->_register->program_counter += kChip8NextInstruction;
  }
void Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk(Chip8* chip8,
                                                               uint16_t
                                                               memory){
  if (
    chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] !=
    Chip8_ReadLoByteFromWord(memory)
    ) {
    chip8->_register->program_counter += kChip8NextInstruction;
    return;
  }
  chip8->_register->program_counter += kChip8NextInstruction;
}
void Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0(Chip8* chip8, 
                                                           uint16_t memory){
  if (
    chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] ==
    chip8->_register->general_perpose[Chip8_ReadSecondNibble(memory)]
    ){
    chip8->_register->program_counter += kChip8NextInstruction;
    return;
  }
  chip8->_register->program_counter += kChip8NextInstruction;
}
void Chip8_LoadMemoryToRegisterX_6xkk(Chip8* chip8, uint16_t memory){
  Chip8_MemoryRead(chip8, memory, Chip8_RegisterXEqualData);
  chip8->_register->program_counter++;
}

void Chip8_AddMemoryToRegisterX_7xkk(Chip8* chip8, uint16_t memory) {
  Chip8_MemoryReadFlag(chip8, memory, Chip8_RegisterXPlusData);
  chip8->_register->program_counter += kChip8NextInstruction;
}

// RegisterX = RegisterY
void Chip8_LoadRegisterYToRegsiterX_8xy0(Chip8* chip8, uint16_t memory) {
  Chip8_RegisterToRegisterOperation(chip8, memory, Chip8_RegisterXEqualData);
  chip8->_register->program_counter += kChip8NextInstruction;
}

// Bitwise register operations opcodes

// RegisterX OR= RegisterY
void Chip8_BitwiseOrRegisterXByRegisterY_8xy1(Chip8* chip8, uint16_t memory){
  Chip8_RegisterToRegisterOperation(chip8, memory, Chip8_RegisterXBitwiseOrData);
  chip8->_register->program_counter += kChip8NextInstruction;
}
// RegisterX AND= RegisterY
void Chip8_BitwiseAndRegisterXByRegisterY_8xy2(Chip8* chip8, uint16_t memory){
  Chip8_RegisterToRegisterOperation(chip8, memory, Chip8_RegisterXBitwiseAndData);
  chip8->_register->program_counter += kChip8NextInstruction;
}
// RegisterX XOR= RegisterY
void Chip8_BitwiseXorRegisterXByRegisterY_8xy3(Chip8* chip8, uint16_t memory){
  Chip8_RegisterToRegisterOperation(chip8, memory, Chip8_RegisterXBitwiseXorData);
  chip8->_register->program_counter += kChip8NextInstruction;
}

// Operators on registers opcodes

// RegisterX += RegisterY
void Chip8_AddRegisterXByRegisterY_8xy4(Chip8* chip8, uint16_t memory){
  Chip8_RegisterToRegisterOperationFlag(chip8, memory, Chip8_RegisterXPlusData);
  chip8->_register->program_counter += kChip8NextInstruction;
}
// RegisterX -= RegisterY
void Chip8_SubRegisterXByRegisterY_8xy5(Chip8* chip8, uint16_t memory){
  Chip8_RegisterToRegisterOperationFlag(chip8, memory, Chip8_RegisterXMinusData);
  chip8->_register->program_counter += kChip8NextInstruction;
}
// I chould not think of a good name for this function:
//
// This function checks if the least significant bit of Register X is 1, if it
// is one the status register is set to 1, else, it's set to 0. Then Register X
// is bit shifted by 1
void Chip8_ShiftRegisterXRight_8xy6(Chip8* chip8, uint16_t memory){
  if (Chip8_ReadFirstNibble(
    chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)]) == 1) {
    chip8->_register->status = 1;
  }
  else {
    chip8->_register->status = 0;
  }
  // This is divison by 2!. This is how it's done in the Chip 8
  chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] >>= 1;
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_SkipIfKeyIsPressed_Ex9E(Chip8* chip8, uint16_t memory){
  if ((chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] &
    chip8->input) != 0) {
    chip8->_register->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_SkipIfKeyIsNotPressed_ExA1(Chip8* chip8, uint16_t memory){
  if (chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] &
    chip8->input == 0) {
    chip8->_register->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_RegisterEqualDelayTimer_Fx07(Chip8* chip8, uint16_t memory){
  chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] =
    chip8->_register->delay_timer;
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_StoreKeyPressInRegisterX_Fx0A(Chip8* chip8, uint16_t memory){
  // If input = 0, that means no keys are pressed during at the function call
  if (chip8->input != 0) {
    chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] = chip8->input;
    chip8->_register->program_counter += kChip8NextInstruction;
  }
}

void Chip8_SetSoundTimerToRegisterX_Fx18(Chip8* chip8, uint16_t memory)
{
  chip8->_register->sound_timer = 
    chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)];
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_IndexPlusRegisterX_Fx1E(Chip8* chip8, uint16_t memory){
  chip8->_register->index = chip8->_register->general_perpose[
                            Chip8_ReadThirdNibble(memory)
  ];
}
// Fills all general perpose registers with memory addresses starting at the
// memory address stored in the index register, then add  X + 1 to the
// index register
void Chip8_IndexRegisterFill_Fx65(Chip8*   chip8, 
                                 uint16_t memory[], 
                                 uint16_t opcode){
  size_t i;
  for (i = 0; i < kChip8MaxNumberOfGeneralPerposeRegisters; i++){
    chip8->_register->general_perpose[i] = memory[chip8->_register->index + i];
  }
  chip8->_register->index += Chip8_ReadThirdNibble(opcode) + 1;
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_ShiftRegisterXLeft_8xyE(Chip8* chip8, uint16_t memory){
  if (Chip8_ReadForthNibble(chip8->_register->general_perpose[
                            Chip8_ReadThirdNibble(memory)]) == 1) {
    chip8->_register->status = 1;
  }
  else {
    chip8->_register->status = 0;
  }
  // This is the power of 2!. This is how it's done in the Chip 8
  chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] <<= 1;
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_SkipIfRegisterXDoesNotEqualStatusRegister_9xy0(Chip8* chip8,
                                                          uint16_t memory){
  if (chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] !=
      chip8->_register->status){
    chip8->_register->program_counter += kChip8SkipNextInstruction;
    return;
  }
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_StoreMemoryInIndexRegister_Annn(Chip8* chip8, uint16_t opcode){
  chip8->_register->index = Chip8_Read12bitFromWord(opcode);
  chip8->_register->program_counter += kChip8NextInstruction;
}

void Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn(Chip8*   chip8,
                                                   uint16_t memory){
  chip8->_register->program_counter = Chip8_Read12bitFromWord(memory) +
    chip8->_register->general_perpose[0];
}

void Chip8_SetRegisterXToRandomByteANDMemory_Cxkk(Chip8* chip8, uint16_t memory){
  chip8->_register->general_perpose[Chip8_ReadThirdNibble(memory)] 
    = Chip8_GetRandom8bitNumber() & memory;
  chip8->_register->program_counter += kChip8NextInstruction;
}