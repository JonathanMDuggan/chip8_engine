#pragma once

#include "include/chip8_emulation.h"
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include "include/chip8_operators.h"
#include "include/chip8_instruction_set.h"
#include "include/chip8_names.h"
#include "include/sdl_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t Chip8_Emulate(const char* file_name) {
  Chip8 chip8 = {0};
  Register reg = {0};
  SDL sdl;
  uint16_t opcode = 0;
  uint8_t emulating = TRUE;
  
  Chip8_OpcodeHandler nnn_opcode_table[kOpcodeNNNLargestIdentifier + 1];
  Chip8_OpcodeHandler kk_opcode_table[kOpcodeKKLargestIdentifier + 1];
  Chip8_OpcodeHandler _8xy_opcode_table[kOpcode8LargestIdentifier + 1];
  Chip8_OpcodeHandler fx_opcode_table[kOpcodeFLargestIdentifier + 1];
  Chip8_OpcodeHandler xy_opcode_table[kOpcodeXYLargestIdentifier + 1];
  Chip8_OpcodeHandler x_opcode_table[kOpcodeXLargestIdentifier + 1];

  chip8.reg = &reg;

  Chip8_InitializeRegisters(&chip8);
  Chip8_InitializeMemory(&chip8);
  // We do not return since we need to see all possible errors.
  if (Chip8_ReadFile(&chip8, file_name) == EXIT_FAILURE) emulating = FALSE;
  if (Chip8_SDLInitialize(&chip8, &sdl) == EXIT_FAILURE) emulating = FALSE;
  
  // Create opcode table

  Chip8_CreateNNNOpcodeTable(&nnn_opcode_table);
  Chip8_CreateKKOpcodeTable(&kk_opcode_table);
  Chip8_Create8xyOpcodeTable(&_8xy_opcode_table);
  Chip8_CreateFxOpcodeTable(&fx_opcode_table);
  Chip8_CreateXYOpcodeTable(&xy_opcode_table);
  Chip8_CreateXOpcodeTable(&x_opcode_table);

  while (emulating) {
    Chip8_ProcessInstruction(&chip8, opcode, &nnn_opcode_table,
                             &kk_opcode_table, &_8xy_opcode_table,
                             &fx_opcode_table, &xy_opcode_table, 
                             &x_opcode_table);

    Chip8_SDLReadInput(&chip8, &sdl, &emulating);
    Chip8_SDLRender(&chip8, &sdl);
  }

  Chip8_SDLQuit(&sdl);
  return EXIT_SUCCESS;
}

void Chip8_ProcessInstruction(Chip8* chip8, uint16_t opcode,
                              Chip8_OpcodeHandler* nnn,
                              Chip8_OpcodeHandler* kk,
                              Chip8_OpcodeHandler* _8xy,
                              Chip8_OpcodeHandler* fx,
                              Chip8_OpcodeHandler* xy,
                              Chip8_OpcodeHandler* x) {
  uint8_t kIdentifierForFandE;
  uint8_t kIdentifier;
  opcode = chip8->memory[chip8->reg->program_counter];
  opcode <<= 8;
  opcode |= chip8->memory[chip8->reg->program_counter + 1];

  kIdentifierForFandE = Chip8_ReadLoByteFromWord(opcode);
  kIdentifier = Chip8_ReadForthNibble(opcode);

  // I didn't bother writing a opcode table for this
  if (kIdentifier == 0) {
    Chip8_OpcodesStartsWith0(chip8, opcode);
    return;
  }

  // All NNN opcodes start with 1,2 and A,B
  if (Chip8_IsNNNOpcode(kIdentifier)) {
    if (kIdentifier > kOpcodeNNNLargestIdentifier) {
      kIdentifier = kOpcodeNNNNOP;
    }
#ifdef _DEBUG
      Chip8_PrintNNNAssembly(chip8, opcode, nnn[kIdentifier].kAssembly);
#endif
      nnn[kIdentifier].instruction(chip8, opcode);

  }

  if (Chip8_IsKKOpcode(kIdentifier)) {
    if (kIdentifier > kOpcodeKKLargestIdentifier) {
      kIdentifier = kOpcodeKKNOP;
    }
     
#ifdef _DEBUG
    Chip8_PrintKKAssembly(chip8, opcode, kk[kIdentifier].kAssembly);
#endif
    kk[kIdentifier].instruction(chip8, opcode);
    return;
  }


  if (kIdentifier == 8) {
    if (kIdentifier > kOpcode8LargestIdentifier) {
      kIdentifier = kOpcodeStartsWith8NOP;
    }
#ifdef _DEBUG
    Chip8_PrintXYAssembly(chip8, opcode, _8xy[kIdentifier].kAssembly);
#endif
    _8xy[kIdentifier].instruction(chip8, opcode);
    return;
  }
  if (kIdentifier == 0xE) {
    if (kIdentifier > kOpcodeXLargestIdentifier) {
      // We can do this because there are only 2 opcodes
      // in table, the other 158 are not.
      kIdentifier = kOpcodeStartsWith8NOP;
    }
#ifdef _DEBUG
    Chip8_PrintXAssembly(chip8, opcode, x[kIdentifier].kAssembly);
#endif
    x[kIdentifier].instruction(chip8, opcode);
  }

  if (kIdentifier == 0xF) {
    if (kIdentifier > kOpcodeFLargestIdentifier) {
      kIdentifierForFandE = kOpcodeNNNNOP;
    }
#ifdef _DEBUG
    Chip8_PrintXAssembly(chip8, opcode, fx[kIdentifierForFandE].kAssembly);
#endif
    fx[kIdentifierForFandE].instruction(chip8, opcode);
  }

  if (Chip8_IsXYOpcode(kIdentifier)){
    if (kIdentifier > kOpcodeXYLargestIdentifier) {
      kIdentifier = kOpcodeNNNNOP;
    }

#ifdef _DEBUG
    Chip8_PrintXYAssembly(chip8, opcode, xy[kIdentifier].kAssembly);
#endif
    xy[kIdentifier].instruction(chip8, opcode);
  }
}

// I do break the 80 col rule here, but if I followed it, it would make this code
// pretty hard to debug
void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
  switch (least_significant_byte) {
    case CLS:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x  CLS\n",
                            chip8->reg->program_counter, opcode);
      Chip8_ClearDisplay_00E0(chip8, opcode);
      break;
    case RET:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x  RET 0x%04x\n",
                            chip8->reg->program_counter, opcode,
                            chip8->stack[chip8->reg->stack_pointer]);
      Chip8_Return_00EE(chip8, opcode);
      break;
      // If for some reason the ROM uses the old jump to location kk
      // call this instruction instead
    default:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x  NOP\n",
                            chip8->reg->program_counter, opcode);
      Chip8_JumpToLocation_1nnn(chip8, opcode);
      break;
  }
}

uint8_t Chip8_IsNNNOpcode(const uint8_t kIdentifier) {
  if ((kIdentifier > 0 && kIdentifier < 3) || (kIdentifier > 9 && kIdentifier < 11))
    return TRUE;
  return FALSE;
}

uint8_t Chip8_IsKKOpcode(const uint8_t kIdentifier) {
  if ((kIdentifier > 2 && kIdentifier < 8 && kIdentifier != 5) ||
      kIdentifier == 0xC) {
    return TRUE;
  }
  return FALSE;
}

uint8_t Chip8_IsXYOpcode(const uint8_t kIdentifier) {
  if (kIdentifier == 5 || kIdentifier == 9 || kIdentifier == 0xd) {
    return TRUE;
  }
  return FALSE;
}
    // These tables are used for opcodes which use the same arguments in the
// kAssembly message

// These opcodes were my absolute favourate. Almost all of them went from
// 0 to 6, all of them took the same argurments (two chip8 registers) 
// tldr: all the xy0 functions but 9xy0, 5xy0 and Dxyn
void Chip8_Create8xyOpcodeTable(Chip8_OpcodeHandler* _8xy) {

  _8xy[kLDVxVy].instruction  = Chip8_LoadRegisterYToRegsiterX_8xy0;
  _8xy[kORVxVy].instruction  = Chip8_BitwiseOrRegisterXByRegisterY_8xy1;
  _8xy[kANDVxVy].instruction = Chip8_BitwiseAndRegisterXByRegisterY_8xy2;
  _8xy[kXORVxVy].instruction = Chip8_BitwiseXorRegisterXByRegisterY_8xy3;
  _8xy[kADDVxVy].instruction = Chip8_AddRegisterXByRegisterY_8xy4;
  _8xy[kSUBVxVy].instruction = Chip8_SubRegisterXByRegisterY_8xy5;
  _8xy[kSHRVxVy].instruction = Chip8_ShiftRegisterXRight_8xy6;
  _8xy[kSHLVxVy].instruction = Chip8_ShiftRegisterXLeft_8xyE;
#ifdef _DEBUG
  _8xy[kLDVxVy].kAssembly  = "0x%03X 0x%04X  LD   V%01X  V%01X \n";
  _8xy[kORVxVy].kAssembly  = "0x%03X 0x%04X  OR   V%01X  V%01X \n";
  _8xy[kANDVxVy].kAssembly = "0x%03X 0x%04X  AND  V%01X  V%01X \n";
  _8xy[kXORVxVy].kAssembly = "0x%03X 0x%04X  XOR  V%01X  V%01X \n";
  _8xy[kADDVxVy].kAssembly = "0x%03X 0x%04X  ADD  V%01X  V%01X \n";
  _8xy[kSUBVxVy].kAssembly = "0x%03X 0x%04X  SUB  V%01X  V%01X \n";
  _8xy[kSHRVxVy].kAssembly = "0x%03X 0x%04X  SHR  V%01X  V%01X \n";
  _8xy[kSHLVxVy].kAssembly = "0x%03X 0x%04X  SHL  V%01X  V%01X \n";
#endif
  // There are no instructions for numbers 7 to 13, use a nop if called.
  for (size_t i = 7; i < 0xE; i++) {
    _8xy[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    _8xy[i].kAssembly   = "0x%03X 0x%04X  NOP  V%01X, V%01X\n";
#endif
  }
  return;
}

// If the Assembly message requires the memory address and nothing else.
// tdlr: all the NNN functions
void Chip8_CreateNNNOpcodeTable(Chip8_OpcodeHandler* nnn) {
  // Rare. but there are some chip8 programs that use 0 to jump to a location
  // the kk 0 instruction won't be called ever, but leaving it there to fill
  // the array.
  nnn[0].instruction = Chip8_NOP;
  nnn[kJPaddr].instruction   = Chip8_JumpToLocation_1nnn;
  nnn[kCALLaddr].instruction = Chip8_Call_2nnn;
  nnn[kLDIaddr].instruction  = Chip8_StoreMemoryInIndexRegister_Annn;
  nnn[kJPV0addr].instruction = Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn;
#ifdef _DEBUG
  nnn[0].kAssembly           = "0x%03X 0x%04X  JP    0x%03X \n";
  nnn[kJPaddr].kAssembly     = "0x%03X 0x%04X  JP    0x%03X \n";
  nnn[kCALLaddr].kAssembly   = "0x%03X 0x%04X  CALL  0x%03X \n";
  nnn[kLDIaddr].kAssembly    = "0x%03X 0x%04X  LD[I] 0x%03X \n";
  nnn[kJPV0addr].kAssembly   = "0x%03X 0x%04X  JPV   0x%03X \n";
#endif
  // There are no instructions for numbers 3 to 9, use a nop if called.
  for (size_t i = 3; i < 0xA; i++) {
    nnn[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    nnn[i].kAssembly   = "0x%03X 0x%04X  NOP  V%03X \n";
#endif
  }
  return;
}

// tdlr: all the KK functions
// I understand this is against the number of col rule, it was just really hard
// to read without breaking the rules
void Chip8_CreateKKOpcodeTable(Chip8_OpcodeHandler* kk) {

  kk[kSEVxbyte].instruction  = Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk;
  kk[kSNEVxbyte].instruction = Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk;
  // This kk is a Register Register funny
  kk[5].instruction = Chip8_NOP;
  kk[kLDVxbyte].instruction  = Chip8_LoadMemoryToRegisterX_6xkk;
  kk[kADDVxbyte].instruction = Chip8_AddMemoryToRegisterX_7xkk;
  kk[kRNDVxbyte].instruction = Chip8_SetRegisterXToRandomByteANDMemory_Cxkk;
#ifdef _DEBUG
  kk[kSEVxbyte].kAssembly  = "0x%03X 0x%04X  SE    V%01X  0x%02X \n";
  kk[kSNEVxbyte].kAssembly = "0x%03X 0x%04X  SNE   V%01X  0x%02X \n";
  kk[kLDVxbyte].kAssembly  = "0x%03X 0x%04X  LD    V%01X  0x%02X \n";
  kk[kADDVxbyte].kAssembly = "0x%03X 0x%04X  ADD   V%01X  0x%02X \n";
  kk[kRNDVxbyte].kAssembly = "0x%03X 0x%04X  RND   V%01X  0x%02X \n";
#endif
  for (size_t i = 8; i < 0xC; i++) {
    kk[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    kk[i].kAssembly = "0x%03X 0x%04X  NOP  0x%02X kk\n";
#endif
  }
  return;
}

void Chip8_CreateFxOpcodeTable(Chip8_OpcodeHandler* fx) {
  // Fill the whole array with nop, then replace the nops with with real
  // instructions, doing with will make the code slower, but it's way easier
  // to debug otherwise.
  for (size_t i = 0; i < kOpcodeFLargestIdentifier; i++) {
    fx[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    fx[i].kAssembly = "0x%03X 0x%04X NOP V%0x1 Fx\n";
#endif
  }
 
  fx[kLDVxDT].instruction = Chip8_RegisterEqualDelayTimer_Fx07;
  fx[kLDVxK].instruction  = Chip8_StoreKeyPressInRegisterX_Fx0A;
  fx[kLDDTVx].instruction = Chip8_Fx15;
  fx[kLDSTVx].instruction = Chip8_SetSoundTimerToRegisterX_Fx18;
  fx[kADDIVx].instruction = Chip8_IndexPlusRegisterX_Fx1E;
  fx[kLDFVx].instruction  = Chip8_IndexEqualsRegisterX_Fx29;
  fx[kLDBVx].instruction  = Chip8_BCDConversion_Fx33;
  fx[kLDIVx].instruction  = Chip8_IndexStoreIteratorFx55;
  fx[kLDVxI].instruction  = Chip8_IndexRegisterFill_Fx65;
#ifdef _DEBUG
  fx[kLDVxDT].kAssembly = "0x%03X 0x04X LD    V%0X1  DT\n";
  fx[kLDVxK].kAssembly  = "0x%03X 0x04X LD    V%0X1  K\n";
  fx[kLDDTVx].kAssembly = "0x%03X 0x04X LD DT V%0X1\n";
  fx[kLDSTVx].kAssembly = "0x%03X 0x04X LD ST V%0X1\n";
  fx[kADDIVx].kAssembly = "0x%03X 0x04X ADD I V%0X1\n";
  fx[kLDFVx].kAssembly  = "0x%03X 0x04X LD F  V%0X1\n";
  fx[kLDBVx].kAssembly  = "0x%03X 0x04X LD B  V%0X1\n";
  fx[kLDIVx].kAssembly  = "0x%03X 0x04X LD[I] V%0X1\n";
  fx[kLDVxI].kAssembly  = "0x%03X 0x04X LD    V%0X1  [I]\n";
#endif
}

void Chip8_CreateXYOpcodeTable(Chip8_OpcodeHandler* xy) { 
    for (size_t i = 0; i < kOpcodeXYLargestIdentifier; i++) {
    xy[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    xy[i].kAssembly = "0x%03X 0x%04X NOP V%0x1 xy\n";
#endif
  }
  xy[SEVxVy].instruction =
      Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0;
  xy[kSNEVxVy].instruction = Chip8_SkipIfRegisterXDoesNotEqualStatusRegister_9xy0;
  xy[kDRWVxVyNibble].instruction = Chip8_Display_Dxyn;
#ifdef _DEBUG
  xy[SEVxVy].kAssembly         = "0X%03X 0x%04X  SE    V%01X  V%01X\n";
  xy[kSNEVxVy].kAssembly       = "0X%03X 0x%04X  SNE   V%01X  V%01X\n";
  xy[kDRWVxVyNibble].kAssembly = "0X%03X 0x%04X  DRW   V%01X  V%01X\n";
#endif  // _DEBUG
}

void Chip8_CreateXOpcodeTable(Chip8_OpcodeHandler* x) { 
  // wasting maybe over a 1kb of ram to store 2 instructions and a message
  // for more code readability 
  for (size_t i = 0; i < kOpcodeXLargestIdentifier; i++) {
    x[i].instruction = Chip8_NOP;
#ifdef _DEBUG
    x[i].kAssembly = "0x%03X 0x%04X NOP  V%01X x\n";
#endif
  }
  x[kSKPVx].instruction = Chip8_SkipIfKeyIsPressed_Ex9E;
  x[SKNPVx].instruction = Chip8_SkipIfKeyIsNotPressed_ExA1;
#ifdef _DEBUG
  x[kSKPVx].kAssembly = "0x%03X 0x%04X  SKP  V%01X\n";
  x[SKNPVx].kAssembly = "0x%03X 0x%04X  SKNP V%01X\n";
#endif
}

// Don't bother with creating these instruction if I didn't enable the
// Chip8 Log Instruction meme
#ifdef _DEBUG

void Chip8_PrintXYAssembly(Chip8* chip8, uint16_t opcode,
                                   const char* kAssembly) {
  const char* const kAssemblyMessage = kAssembly;
  const uint8_t kVx = Chip8_ReadThirdNibble(opcode);
  const uint8_t kVy = Chip8_ReadSecondNibble(opcode);
  char buffer[50] = {0};

  sprintf_s(buffer, sizeof(buffer), kAssemblyMessage,
            chip8->reg->program_counter, opcode,
            kVx, kVy);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintKKAssembly(Chip8* chip8, uint16_t opcode,
                               const char* kAssembly) {
  const char* const kAssemblyMessage = kAssembly;
  const uint8_t kVx = Chip8_ReadThirdNibble(opcode);
  const uint16_t kKK = Chip8_ReadLoByteFromWord(opcode);
  char buffer[50] = {0};

  sprintf_s(buffer, sizeof(buffer), kAssemblyMessage,
            chip8->reg->program_counter, opcode,
            kVx, kKK);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintXAssembly(Chip8* chip8, uint16_t opcode,
                              const char* kAssembly) {
  const char* const kAssemblyMessage = kAssembly;
  const uint8_t kVx = Chip8_ReadThirdNibble(opcode);
  char buffer[50] = {0};

  sprintf_s(buffer, sizeof(buffer), kAssemblyMessage,
            chip8->reg->program_counter, opcode,
            kVx);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintNNNAssembly(Chip8* chip8, uint16_t opcode,
                                const char* kAssembly) {
  const uint16_t kKK = Chip8_Read12bitFromWord(opcode);
  const char* const kAssemblyMessage = kAssembly;
  char buffer[50] = {0};

  sprintf_s(buffer, sizeof(buffer), kAssemblyMessage,
            chip8->reg->program_counter, opcode, kKK);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

#endif