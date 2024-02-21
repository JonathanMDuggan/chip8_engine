#pragma once
#include "chip8_processor.h"
#include "sdl_config.h"
#include <stdint.h>
extern uint8_t Chip8_Emulate(const char* file_name);
extern void Chip8_ProcessInstruction(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsAt9ToE(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsWithF(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode);