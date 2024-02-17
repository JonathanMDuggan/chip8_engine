#pragma once
#include "chip8_processor.h"
// Yes, I broke the convention by naming enums without the k at the start
enum Chip8_Opcodes {
  kSysAddr_0x000 = 0x000, kJPaddr_0x100 = 0x100, kCLS_0x00E0 = 0x00E0,

  // Opcodes which start with the number 8
  LDVxVy = 0,
  ORVxVy = 1,
  ANDVxVy = 2,
  XORVxVy = 3,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2,
  //kChip8_VxVy_ = 2
};

// About the Chip-8 Instrcution set
//
// The Chip-8 only has 3X instructions, which is tiny.

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

extern void Chip8_ClearDisplay_00E0(Chip8* chip8, uint16_t memory);
extern void Chip8_Return_00EE(Chip8* chip8, uint16_t memory);
extern void Chip8_JumpToLocation_1nnn(Chip8* chip8, uint16_t memory);
extern void Chip8_Call_2nnn(Chip8* chip8,uint16_t memory);

extern void Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk(
  Chip8*   chip8,
  uint16_t memory);
extern void Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk(
  Chip8*   chip8, 
  uint16_t memory
);
extern void Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0(
  Chip8*   chip8, 
  uint16_t memory
);
extern void Chip8_LoadMemoryToRegisterX_6xkk(Chip8* chip8, uint16_t memory);
extern void Chip8_AddMemoryToRegisterX_7xkk(Chip8* chip8, uint16_t memory);
extern void Chip8_LoadRegisterYToRegsiterX_8xy0(Chip8* chip8, uint16_t memory);
extern void Chip8_LoadRegisterYToRegsiterX_8xy0(Chip8* chip8, uint16_t memory);

extern void Chip8_BitwiseOrRegisterXByRegisterY_8xy1 (Chip8*   chip8, 
                                                     uint16_t memory);

extern void Chip8_BitwiseAndRegisterXByRegisterY_8xy2(Chip8*   chip8,
                                                     uint16_t memory);

extern void Chip8_BitwiseXorRegisterXByRegisterY_8xy3(Chip8*   chip8, 
                                                     uint16_t memory);

extern void Chip8_AddRegisterXByRegisterY_8xy4(Chip8* chip8, uint16_t memory);
extern void Chip8_SubRegisterXByRegisterY_8xy5(Chip8* chip8, uint16_t memory);
extern void Chip8_ShiftRegisterXRight_8xy6(Chip8* chip8, uint16_t memory);
extern void Chip8_8xy7(Chip8* chip8, uint16_t memory);
extern void Chip8_ShiftRegisterXLeft_8xyE(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfRegisterXDoesNotEqualStatusRegister_9xy0(Chip8* chip8,
                                                                uint16_t memory);
extern void Chip8_StoreMemoryInIndexRegister_Annn(Chip8*   chip8,
                                                  uint16_t opcode);
extern void Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn(Chip8*   chip8, 
                                                          uint16_t memory);
extern void Chip8_SetRegisterXToRandomByteANDMemory_Cxkk(Chip8*   chip8, 
                                                         uint16_t memory);
extern void Chip8_Dxyn(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfKeyIsPressed_Ex9E(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfKeyIsNotPressed_ExA1(Chip8* chip8, uint16_t memory);

extern void Chip8_RegisterEqualDelayTimer_Fx07(Chip8* chip8, uint16_t memory);
extern void Chip8_StoreKeyPressInRegisterX_Fx0A(Chip8* chip8, uint16_t memory);
extern void Chip8_Fx15(Chip8* chip8, uint16_t memory);
extern void Chip8_SetSoundTimerToRegisterX_Fx18(Chip8* chip8, uint16_t memory);
extern void Chip8_IndexPlusRegisterX_Fx1E(Chip8* chip8, uint16_t memory);
extern void Chip8_Fx29(Chip8* chip8, uint16_t memory);
extern void Chip8_Fx33(Chip8* chip8, uint16_t memory);
extern void Chip8_Fx55(Chip8* chip8, uint16_t memory);
// Fills all general perpose registers with memory addresses starting at the
// memory address stored in the index register, then sets the index register
// to X
extern void Chip8_IndexRegisterFill_Fx65(Chip8*   chip8,
                                        uint16_t memory[],
                                        uint16_t opcode);