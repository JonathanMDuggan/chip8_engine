#pragma once
#include "include/chip8_emulation.h"
#include "include/chip8_processor.h"
#include "include/chip8_load_ROM.h"
#include "include/chip8_operators.h"
#include "include/chip8_instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
uint8_t Chip8_EmulationMainLoop(Chip8* chip8, const char* file_name) {
  uint16_t opcode = 0;
  opcode = chip8->memory[chip8->_register->program_counter];
  opcode = opcode << 8;
  opcode |= chip8->memory[chip8->_register->program_counter + 1];
  if(Chip8)
  switch () {
  }
}
void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode) {
  uint8_t last_hexidecmial = Chip8_ReadFirstNibble(opcode);
  switch (last_hexidecmial) {
  case LDVxVy: Chip8_LoadRegisterYToRegsiterX_8xy0(chip8, opcode); break;
  case ORVxVy: Chip8_BitwiseOrRegisterXByRegisterY_8xy1(chip8, opcode) break;
  case ANDVxVy: Chip8_BitwiseAndRegisterXByRegisterY_8xy2(chip8, opcode) break;
  case XORVxVy: Chip8_BitwiseXorRegisterXByRegisterY_8xy3(chip8, opcode) break;





   
  }
}