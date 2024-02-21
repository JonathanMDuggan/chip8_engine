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
      Chip8_ClearDisplay_00E0(chip8, opcode);
      break;
    case RET:
      Chip8_Return_00EE(chip8, opcode);
      CHIP8_LOG_INSTRUCTION(
          "Status Register: 0x%04x: Program Counter: 0x%04: RET: 0x%04x\n",
          chip8->_register->status, chip8->_register->program_counter, opcode);
      break;
      // If for some reason the ROM uses the old jump to location opcode
      // call this function instead
    default:
      Chip8_JumpToLocation_1nnn(chip8, opcode);
      CHIP8_LOG_INSTRUCTION("ROM used discontinued opcode: 0x%04x\n", opcode);
      break;
  }
}

void Chip8_OpcodeStartsAt2To7(Chip8* chip8, uint16_t opcode) {
  uint8_t most_significant_nibble = Chip8_ReadForthNibble(opcode);
  switch (most_significant_nibble) {
    case JPaddr:
      Chip8_JumpToLocation_1nnn(chip8, opcode);
      break;
    case CALLaddr:
      Chip8_Call_2nnn(chip8, opcode);
      CHIP8_LOG_INSTRUCTION(
          "Status Register: 0x%04x: Program Counter: 0x%04: CALL: 0x%04x\n",
          chip8->_register->status, chip8->_register->program_counter, opcode);
      break;
    case SEVxbyte:
      Chip8_SkipNextInstrucionIfRegisterXEqualMemory_3xkk(chip8, opcode);
      break;
    case SNEVxbyte:
      Chip8_SkipNextInstrucionIfRegisterXDoesNotEqualMemory_4xkk(chip8, opcode);
      break;
    case SEVxVy:
      Chip8_SkipNextInstrucionIfRegisterXEqualRegisterY_5xy0(chip8, opcode);
      break;
    case LDVxbyte:
      Chip8_LoadMemoryToRegisterX_6xkk(chip8, opcode);
      break;
    case ADDVxbyte:
      Chip8_AddMemoryToRegisterX_7xkk(chip8, opcode);
      break;
  }
}

void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode) {
  uint8_t most_significant_nibble = Chip8_ReadFirstNibble(opcode);
  switch (most_significant_nibble) {
    case kLDVxVy:
      Chip8_LoadRegisterYToRegsiterX_8xy0(chip8, opcode);
      break;
    case kORVxVy:
      Chip8_BitwiseOrRegisterXByRegisterY_8xy1(chip8, opcode);
      break;
    case kANDVxVy:
      Chip8_BitwiseAndRegisterXByRegisterY_8xy2(chip8, opcode);
      break;
    case kXORVxVy:
      Chip8_BitwiseXorRegisterXByRegisterY_8xy3(chip8, opcode);
      break;
    case kADDVxVy:
      Chip8_AddRegisterXByRegisterY_8xy4(chip8, opcode);
      break;
    case kSUBVxVy:
      Chip8_SubRegisterXByRegisterY_8xy5(chip8, opcode);
      break;
    case kSHRVxVy:
      Chip8_ShiftRegisterXRight_8xy6(chip8, opcode);
      break;
    case kSUBNVxVy:
      break;
    case kSHLVxVy:
      Chip8_ShiftRegisterXLeft_8xyE(chip8, opcode);
      break;
    default:
      printf("[i] Chip8 Invaild Instruction: %x\n", most_significant_nibble);
      break;
  }
}
void Chip8_OpcodeStartsAt9ToE(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte;
  uint8_t most_significant_nibble = Chip8_ReadFirstNibble(opcode);
  switch (most_significant_nibble) {
    case kSNEVxVy:
      Chip8_SkipIfRegisterXDoesNotEqualStatusRegister_9xy0(chip8, opcode);
      break;
    case kLDIaddr:
      Chip8_StoreMemoryInIndexRegister_Annn(chip8, opcode);
      break;
    case kJPV0addr:
      Chip8_JumpToLocationInMemoryPlusRegister0_Bnnn(chip8, opcode);
      break;
    case kRNDVxbyte:
      Chip8_SetRegisterXToRandomByteANDMemory_Cxkk(chip8, opcode);
      break;
    case kDRWVxVyNibble:
      Chip8_Display_Dxyn(chip8, opcode);
      break;
    case kE:
      least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
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
  opcode = chip8->memory[chip8->_register->program_counter];
  opcode <<= 8;
  opcode |= chip8->memory[chip8->_register->program_counter + 1];

  if (Chip8_ReadForthNibble(opcode) == 0) {
    Chip8_OpcodesStartsWith0(chip8, opcode);
    return;
  }

  if (Chip8_ReadFirstNibble(opcode) > 0 && Chip8_ReadFirstNibble(opcode) < 8) {
    Chip8_OpcodeStartsAt2To7(chip8, opcode);
    return;
  }

  if (Chip8_ReadFirstNibble(opcode) == 8) {
    Chip8_OpcodeStartsWith8(chip8, opcode);
    return;
  }

  if (Chip8_ReadFirstNibble(opcode) == 0xF) {
    Chip8_OpcodeStartsWithF(chip8, opcode);
    return;
  } 
  else {
    Chip8_OpcodeStartsAt9ToE(chip8, opcode);
    return;
  }
}