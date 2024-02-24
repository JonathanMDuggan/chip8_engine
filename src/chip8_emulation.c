#pragma once

#include "include/chip8_emulation.h"
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include "include/chip8_operators.h"
#include "include/chip8_instruction_set.h"
#include "include/chip8_names.h"
#include "include/sdl_config.h"
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t Chip8_Emulate(const char* file_name) {
  Chip8 chip8;
  Register _register;
  SDL sdl;
  uint16_t opcode = 0;
  uint8_t emulating = TRUE;
  chip8._register = &_register;

  Chip8_InitializeRegisters(&chip8);
  Chip8_InitializeMemory(&chip8);
  // We do not return since we need to see all possible errors.
  if (Chip8_ReadFile(&chip8, file_name) == EXIT_FAILURE) emulating = FALSE;
  if (Chip8_SDLInitialize(&chip8, &sdl) == EXIT_FAILURE) emulating = FALSE;
  
  while (emulating) {
    Chip8_SDLRender(&sdl);
    Chip8_ProcessInstruction(&chip8, opcode);
    SDL_Delay(100);
    Chip8_SDLReadInput(&chip8, &sdl, &emulating);
  }

  Chip8_SDLQuit(&sdl);
}
// I do break the 80 col rule here, but if I followed it, it would make this code
// pretty hard to debug
void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
  switch (least_significant_byte) {
    case CLS:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x CLS\n",
                            chip8->_register->program_counter, opcode);
      Chip8_ClearDisplay_00E0(chip8, opcode);
      break;
    case RET:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x RET 0x%04x\n",
                            chip8->_register->program_counter, opcode,
                            chip8->stack[chip8->_register->stack_pointer]);
      Chip8_Return_00EE(chip8, opcode);
      break;
      // If for some reason the ROM uses the old jump to location opcode
      // call this instruction instead
    default:
      CHIP8_LOG_INSTRUCTION("0x%03x 0x%04x NOP\n",
                            chip8->_register->program_counter, opcode);
      Chip8_JumpToLocation_1nnn(chip8, opcode);
      break;
  }
}

void Chip8_OpcodeStartsAt2To7(Chip8* chip8, uint16_t opcode) {
  uint8_t most_significant_nibble = Chip8_ReadForthNibble(opcode);
  switch (most_significant_nibble) {
    case SEVxVy:
      Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0(chip8, opcode);
      break;
  }
}

void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode) {
  Chip8_OpcodeHandler execute[kNumberOfOpcodesThatStartWith8];
  Chip8_Execute(chip8, opcode, Chip8_ReadForthNibble(opcode),
                kNumberOfOpcodesThatStartWith8,
                Chip8_CreateArithmeticOpcodeTable, execute,
                kOpcodeStartsWith8NOP);
}

void Chip8_OpcodeNNN(Chip8* chip8, uint16_t opcode) {
  Chip8_OpcodeHandler execute[kOpcodeNNNFunctionPointerArraySize];
  Chip8_Execute(chip8, opcode, Chip8_ReadForthNibble(opcode),
                kOpcodeNNNFunctionPointerArraySize,
                Chip8_CreateAddressOpcodeTable, execute, kOpcodeNNNNOP);
}

void Chip8_OpcodeKK(Chip8* chip8, uint16_t opcode) {
  Chip8_OpcodeHandler execute[kOpcodeKKFunctionPointerArraySize];
  Chip8_Execute(chip8, opcode, Chip8_ReadForthNibble(opcode),
                kOpcodeKKFunctionPointerArraySize,
                Chip8_CreateMemoryOpcodeTable, execute, kOpcodeKKNOP);
}

// largest function parameters I know!
void Chip8_Execute(
    Chip8* chip8, uint16_t opcode, uint8_t kIdentifier,
    const uint8_t kFunctionPointerArraySize,
    void (*CreateFunctionPointerArray)(Chip8_OpcodeHandler* mnemonic[]),
    Chip8_OpcodeHandler execute[], const uint8_t kNOP)
    {
  if (kIdentifier >= kFunctionPointerArraySize) {
    kIdentifier = kNOP;
  }
  CreateFunctionPointerArray(&execute);
#ifdef _DEBUG
  Chip8_PrintAddressAssembly(chip8, opcode, execute[kIdentifier].kAssembly);
#endif
  execute[kIdentifier].instruction(chip8, opcode);
}

void Chip8_OpcodeStartsAt9ToE(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
  uint8_t most_significant_nibble = Chip8_ReadFirstNibble(opcode);
  switch (most_significant_nibble) {
    case kSNEVxVy:
      Chip8_SkipIfRegisterXDoesNotEqualStatusRegister_9xy0(chip8, opcode);
      break;
    case kDRWVxVyNibble:
      Chip8_Display_Dxyn(chip8, opcode);
      break;
    case kE:

      if (least_significant_byte == kSKPVx) {
        Chip8_SkipIfKeyIsPressed_Ex9E(chip8, opcode);
        break;
      }
      if (least_significant_byte == SKNPVx) {
        Chip8_SkipIfKeyIsNotPressed_ExA1(chip8, opcode);
        break;
      }
  }
}

void Chip8_OpcodeStartsWithF(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
  switch (least_significant_byte) {
    case kLDVxDT:
      Chip8_RegisterEqualDelayTimer_Fx07(chip8, opcode);
      break;
    case kLDVxK:
      Chip8_StoreKeyPressInRegisterX_Fx0A(chip8, opcode);
      break;
    case kLDDTVx:
      Chip8_Fx15(chip8, opcode);
      break;
    case kLDSTVx:
      Chip8_SetSoundTimerToRegisterX_Fx18(chip8, opcode);
      break;
    case kADDIVx:
      Chip8_IndexPlusRegisterX_Fx1E(chip8, opcode);
      break;
    case kLDFVx:
      Chip8_IndexEqualsRegisterX_Fx29(chip8, opcode);
      break;
    case kLDBVx:
      Chip8_BCDConversion_Fx33(chip8, opcode);
      break;
    case kLDIVx:
      Chip8_Fx55(chip8, opcode);
      break;
    // The only Opcode that needs more than 2 arguements
    case kLDVxI:
      Chip8_IndexRegisterFill_Fx65(
          chip8, &chip8->memory[chip8->_register->program_counter], opcode);
      break;
  }
}

void Chip8_ProcessInstruction(Chip8* chip8, uint16_t opcode) {
  const uint8_t kIdentifier = Chip8_ReadForthNibble(opcode);
  opcode = chip8->memory[chip8->_register->program_counter];
  opcode <<= 8;
  opcode |= chip8->memory[chip8->_register->program_counter + 1];

  if (kIdentifier == 0) {
    Chip8_OpcodesStartsWith0(chip8, opcode);
    return;
  }

  // All NNN opcodes start with 1,2 and A,B
  if (Chip8_IsNNNOpcode(kIdentifier)) {
    Chip8_OpcodeNNN(chip8, opcode);
    return;
  }

  if (Chip8_IsKKOpcode(kIdentifier)) {
    Chip8_OpcodeKK(chip8, opcode);
    return;
  }

  if (kIdentifier == 8) {
    Chip8_OpcodeStartsWith8(chip8, opcode);
    return;
  }

  if (kIdentifier == 0xF) {
    Chip8_OpcodeStartsWithF(chip8, opcode);
    return;
  } 
  else {
    Chip8_OpcodeStartsAt9ToE(chip8, opcode);
    return;
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
    // These tables are used for opcodes which use the same arguments in the
// kAssembly message

// These opcodes were my absolute favourate. Almost all of them went from
// 0 to 6, all of them took the same argurments (two chip8 registers) 
// tldr: all the xy0 functions but 9xy0, 5xy0 and Dxyn
void Chip8_CreateArithmeticOpcodeTable(Chip8_OpcodeHandler* opcode[]) {

  opcode[kLDVxVy]->instruction  = Chip8_LoadRegisterYToRegsiterX_8xy0;
  opcode[kORVxVy]->instruction  = Chip8_BitwiseOrRegisterXByRegisterY_8xy1;
  opcode[kANDVxVy]->instruction = Chip8_BitwiseAndRegisterXByRegisterY_8xy2;
  opcode[kXORVxVy]->instruction = Chip8_BitwiseXorRegisterXByRegisterY_8xy3;
  opcode[kADDVxVy]->instruction = Chip8_AddRegisterXByRegisterY_8xy4;
  opcode[kSUBVxVy]->instruction = Chip8_SubRegisterXByRegisterY_8xy5;
  opcode[kSHRVxVy]->instruction = Chip8_ShiftRegisterXRight_8xy6;
  opcode[kSHLVxVy]->instruction = Chip8_ShiftRegisterXLeft_8xyE;
#ifdef _DEBUG
  opcode[kLDVxVy]->kAssembly    = "0x%03x 0x%04x  LD   V%01x, V%01x\n";
  opcode[kORVxVy]->kAssembly    = "0x%03x 0x%04x  OR   V%01x, V%01x\n";
  opcode[kANDVxVy]->kAssembly   = "0x%03x 0x%04x  AND  V%01x, V%01x\n";
  opcode[kXORVxVy]->kAssembly   = "0x%03x 0x%04x  XOR  V%01x, V%01x\n";
  opcode[kADDVxVy]->kAssembly   = "0x%03x 0x%04x  ADD  V%01x, V%01x\n";
  opcode[kSUBVxVy]->kAssembly   = "0x%03x 0x%04x  SUB  V%01x, V%01x\n";
  opcode[kSHRVxVy]->kAssembly   = "0x%03x 0x%04x  SHR  V%01x, V%01x\n";
  opcode[kSHLVxVy]->kAssembly   = "0x%03x 0x%04x  SHL  V%01x, V%01x\n";
#endif
  // There are no instructions for numbers 7 to 13, use a nop if called.
  for (size_t i = 7; i < 0xE; i++) {
    opcode[i]->instruction = Chip8_NOP;
#ifdef _DEBUG
    opcode[i]->kAssembly   = "0x%03x 0x%04x  NOP  V%01x, V%01x\n";
#endif
  }
}

// If the Assembly message requires the memory address and nothing else.
// tdlr: all the NNN functions
void Chip8_CreateAddressOpcodeTable(Chip8_OpcodeHandler* opcode[]) {
  // Rare. but there are some chip8 programs that use 0 to jump to a location
  // the opcode 0 instruction won't be called ever, but leaving it there to fill
  // the array.
  opcode[0]->instruction         = Chip8_JumpToLocation_1nnn;
  opcode[kJPaddr]->instruction   = Chip8_JumpToLocation_1nnn;
  opcode[kCALLaddr]->instruction = Chip8_Call_2nnn;
  opcode[kLDIaddr]->instruction  = Chip8_StoreMemoryInIndexRegister_Annn;
  opcode[kJPV0addr]->instruction = Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn;
#ifdef _DEBUG
  opcode[0]->kAssembly           = "0x%03x 0x%04x  JP    V%03x \n";
  opcode[kJPaddr]->kAssembly     = "0x%03x 0x%04x  JP    V%03x \n";
  opcode[kCALLaddr]->kAssembly   = "0x%03x 0x%04x  CALL  V%03x \n";
  opcode[kLDIaddr]->kAssembly    = "0x%03x 0x%04x  LD[I] V%03x \n";
  opcode[kJPV0addr]->kAssembly   = "0x%03x 0x%04x  JPV   V%03x \n";
#endif
  // There are no instructions for numbers 3 to 9, use a nop if called.
  for (size_t i = 3; i < 0xA; i++) {
    opcode[i]->instruction = Chip8_NOP;
#ifdef _DEBUG
    opcode[i]->kAssembly   = "0x%03x 0x%04x  NOP  V%03x \n";
#endif
  }
}

// tdlr: all the KK functions
void Chip8_CreateMemoryOpcodeTable(Chip8_OpcodeHandler* opcode[]) {

  opcode[kSEVxbyte]->instruction  =
      Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk;
  opcode[kSNEVxbyte]->instruction =
      Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk;
  // This opcode is a Register Register funny
  opcode[5]->instruction = Chip8_NOP;
  opcode[kLDVxbyte]->instruction  = 
    Chip8_LoadMemoryToRegisterX_6xkk;
  opcode[kADDVxbyte]->instruction = 
    Chip8_AddMemoryToRegisterX_7xkk;
  opcode[kRNDVxbyte]->instruction =
      Chip8_SetRegisterXToRandomByteANDMemory_Cxkk;
#ifdef _DEBUG
  opcode[kSEVxbyte]->kAssembly =  "0x%03x 0x%04x  SE    V%02x\n";
  opcode[kSNEVxbyte]->kAssembly = "0x%03x 0x%04x  SNE   V%02x\n";
  opcode[kLDVxbyte]->kAssembly =  "0x%03x 0x%04x  LD    V%02x\n";
  opcode[kADDVxbyte]->kAssembly = "0x%03x 0x%04x  ADD   V%02x\n";
  opcode[kRNDVxbyte]->kAssembly = "0x%03x 0x%04x  RND   V%02x\n";
#endif
  for (size_t i = 8; i < 0xC; i++) {
    opcode[i]->instruction = Chip8_NOP;
#ifdef _DEBUG
    opcode[i]->kAssembly = "0x%03x 0x%04x  NOP  V%02x\n";
#endif
  }
}
// Don't bother with creating these instruction if I didn't enable the
// Chip8 Log Instruction meme
#ifdef _DEBUG

void Chip8_PrintArithmeticAssembly(Chip8* chip8, uint16_t opcode,
                                   const char* kAssembly) {
  char buffer[50];
  sprintf_s(buffer, kAssembly, 50,
            chip8->_register->program_counter, opcode,
            chip8->_register->general_perpose[Chip8_ReadThirdNibble(opcode)],
            chip8->_register->general_perpose[Chip8_ReadSecondNibble(opcode)]);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintMemoryAssembly(Chip8* chip8, uint16_t opcode,
  const char* kAssembly) {
  char buffer[50];
  sprintf_s(buffer, 50, kAssembly,
            chip8->_register->program_counter, opcode,
            chip8->_register->general_perpose[Chip8_ReadThirdNibble(opcode)],
            Chip8_ReadLoByteFromWord(chip8->memory));
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintIndexAssembly(Chip8* chip8, uint16_t opcode,
                              const char* kAssembly) {
  char buffer[50];
  sprintf_s(buffer, 50, kAssembly,
            chip8->_register->program_counter, opcode,
            chip8->_register->general_perpose[Chip8_ReadThirdNibble(opcode)],
            chip8->_register->index);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}

void Chip8_PrintAddressAssembly(Chip8* chip8, uint16_t opcode,
                                const char* kAssembly) {
  char buffer[kMaxBufferSize];
  sprintf_s(buffer, kAssembly, kMaxBufferSize,
            chip8->_register->program_counter, opcode, opcode);
  CHIP8_LOG_INSTRUCTION("%s", buffer);
}
#endif