#pragma once
#include "include/chip8_emulation.h"
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include "include/chip8_operators.h"
#include "include/chip8_instruction_set.h"
#include "include/chip8_names.h"
#include <stdio.h>
#include <stdlib.h>


uint8_t Chip8_EmulationMainLoop(Chip8* chip8, const char* file_name) {
  uint16_t opcode = 0;

  do{
    opcode = chip8->memory[chip8->_register->program_counter];
    opcode <<= 8;
    opcode |= chip8->memory[chip8->_register->program_counter + 1];


    if (Chip8_ReadForthNibble(opcode) == 0) {
      Chip8_OpcodesStartsWith0(chip8, opcode);
      continue;
    }
    if (Chip8_ReadFirstNibble(opcode) > 0 && 
        Chip8_ReadFirstNibble(opcode) < 8) {
      Chip8_OpcodeStartsAt2To7(chip8, opcode);
      continue;
    }
    if (Chip8_ReadFirstNibble(opcode) == 8) {
      Chip8_OpcodeStartsWith8(chip8, opcode);
      continue;
    }
    if (Chip8_ReadFirstNibble(opcode) == 0xF) {
      Chip8_OpcodeStartsWithF(chip8, opcode);
      continue;
    }
    else {
      Chip8_OpcodeStartsAt9ToE(chip8, opcode);
      continue;
    }

  } while (true);
}
// I do break the 80 col rule here, but if I followed it, it would make this code
// pretty hard to debug
void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode) {
  uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
  switch (least_significant_byte) {
  case CLS: Chip8_ClearDisplay_00E0(chip8, opcode);
    break;
  case RET: Chip8_Return_00EE(chip8, opcode);
    break;
    // If for some reason the ROM uses the old jump to location opcode
    // call this function instead
  default:  Chip8_JumpToLocation_1nnn(chip8, opcode);
    CHIP8_LOG_INFO("ROM used discontinued opcode: %x", opcode);
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
  case LDVxbyte:  Chip8_LoadMemoryToRegisterX_6xkk(chip8, opcode);     
    break;
  case ADDVxbyte: Chip8_AddMemoryToRegisterX_7xkk(chip8, opcode);   
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
  case kSUBNVxVy: break;
  case kSHLVxVy: 
    Chip8_ShiftRegisterXLeft_8xyE 
    break;
  default: printf("[i] Chip8 Invaild Instruction: %x\n", 
                   most_significant_nibble);
    break;
  }
}
void Chip8_OpcodeStartsAt9ToE(Chip8* chip8, uint16_t opcode) {
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
    Chip8_Dxyn(chip8, opcode); 
    break;
  case kE: uint8_t least_significant_byte = Chip8_ReadLoByteFromWord(opcode);
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
    Chip8_Fx29(chip8, opcode);                        
    break;
  case kLDBVx:  
    Chip8_Fx33(chip8, opcode);                         
    break;
  case kLDIVx: 
    Chip8_Fx55(chip8, opcode);                         
    break;
  // The only Opcode that needs more than 2 arguements
  case kLDVxI: 
    Chip8_IndexRegisterFill_Fx65(Chip8* chip8,
                                &chip8->memory[
                                 chip8->_register->program_counter],
                                 opcode); 
    break;
  }
}
