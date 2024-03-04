#pragma once
#include "chip8_names.h"
#include "chip8_processor.h"
#include <Windows.h>
#include <stdint.h>
enum Chip8_Opcodes {
  kSysAddr_0x000 = 0x000, kJPaddr_0x100 = 0x100, kCLS_0x00E0 = 0x00E0,
  // Opcodes which start with the number 0
  CLS = 0xE0,
  RET = 0xEE,
  // Opcodes where the first number is between 1 to 7
  kJPaddr   = 1, kCALLaddr  = 2, kSEVxbyte  = 3, kSNEVxbyte = 4, SEVxVy = 5,
  kLDVxbyte = 6, kADDVxbyte = 7,
  // Opcodes which start with the number 8
  kLDVxVy  = 0, kORVxVy  = 1, kANDVxVy  = 2, kXORVxVy = 3,   kADDVxVy = 4,
  kSUBVxVy = 5, kSHRVxVy = 6, kSUBNVxVy = 7, kSHLVxVy = 0xE,
  // Opcodes between 9 and D
  kSNEVxVy       = 0x9, kLDIaddr = 0xA, kJPV0addr = 0xB,  kRNDVxbyte = 0xC,
  kDRWVxVyNibble = 0xD, kE       = 0xE, kSKPVx    = 0x9E, SKNPVx     = 0xA1,
  // Opcodes which start with the number F
  kLDVxDT = 0x07, kLDVxK = 0x0A, kLDDTVx = 0x15, kLDSTVx = 0x18,
  kADDIVx = 0x1E, kLDFVx = 0x29, kLDBVx  = 0x33, kLDIVx  = 0x55,
  kLDVxI  = 0x65,

  // Function Pointer Size
  kOpcode8LargestIdentifier = 0xF,
  kOpcodeNNNLargestIdentifier = 0xB,
  kOpcodeStartsWith8NOP = 0xA,
  kOpcodeNNNNOP = 3,
  kMaxBufferSize = 50,
  kOpcodeKKLargestIdentifier = 0xC,
  kOpcodeKKNOP = 5,
  kOpcodeFLargestIdentifier = 0x65,
  kOpcodeXYLargestIdentifier = 0xD,
  kOpcodeXLargestIdentifier = 0xA1,
};

typedef void (*Chip8_OpcodeFunction)(Chip8*, uint16_t);

typedef struct Chip8_OpcodeHandler{
  Chip8_OpcodeFunction instruction;
  const char* kAssembly;
} Chip8_OpcodeHandler;

// About the Chip-8 Instrcution set
//
// The Chip-8 only has 3X instructions, which is tiny.

// Instruction Set functions:
// 
// Unconventional instruction naming convention, so here is the explaination: 
// 
// The number at the end of every instruction repersents the opcodes
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
// Therefore if you see the instruction 'Chip8_AddMemoryToRegisterX_7xkk' it means
// Add the value at 'kk' to Register 'x' 

// extern void Chip8_JumpToLocation_0nnn(chip8* chip8, uint16_t memory);
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
extern void Chip8_SubtractRegisterYbyRegisterX_8xy7(Chip8* chip8, uint16_t memory);
extern void Chip8_ShiftRegisterXLeft_8xyE(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfRegisterXDoesNotEqualRegisterY_9xy0(
    Chip8* chip8, uint16_t memory);
extern void Chip8_StoreMemoryInIndexRegister_Annn(Chip8*   chip8,
                                                  uint16_t opcode);
extern void Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn(Chip8* chip8,
                                                           uint16_t memory);
extern void Chip8_SetRegisterXToRandomByteANDMemory_Cxkk(Chip8*   chip8, 
                                                         uint16_t memory);
extern void Chip8_Display_Dxyn(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfKeyIsPressed_Ex9E(Chip8* chip8, uint16_t memory);
extern void Chip8_SkipIfKeyIsNotPressed_ExA1(Chip8* chip8, uint16_t memory);

extern void Chip8_RegisterEqualDelayTimer_Fx07(Chip8* chip8, uint16_t memory);
extern void Chip8_StoreKeyPressInRegisterX_Fx0A(Chip8* chip8, uint16_t memory);
extern void Chip8_Fx15(Chip8* chip8, uint16_t memory);
extern void Chip8_SetSoundTimerToRegisterX_Fx18(Chip8* chip8, uint16_t memory);
extern void Chip8_IndexPlusRegisterX_Fx1E(Chip8* chip8, uint16_t memory);
extern void Chip8_IndexEqualsRegisterX_Fx29(Chip8* chip8, uint16_t memory);
extern void Chip8_BCDConversion_Fx33(Chip8* chip8, uint16_t memory);
extern void Chip8_IndexStoreIterator_Fx55(Chip8* chip8, uint16_t memory);
// Fills all general perpose registers with memory addresses starting at the
// memory address stored in the index register, then sets the index register
// to X
extern void Chip8_IndexRegisterFill_Fx65(Chip8* chip8, uint16_t opcode);
extern void Chip8_Display_Dxyn(Chip8* chip8, uint16_t memory);
extern void Chip8_NOP(Chip8* chip8, uint16_t memory);
