#pragma once
#include "chip8_processor.h"
#include "sdl_config.h"
#include "chip8_instruction_set.h"
#include <stdint.h>
extern uint8_t Chip8_Emulate(const char* file_name);
extern void Chip8_ProcessInstruction(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsAt9ToE(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsWithF(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode);
extern void Chip8_CreateArithmeticOpcodeTable(Chip8_OpcodeHandler* table[]);
extern void Chip8_PrintArithmeticAssembly(Chip8* chip8, uint16_t opcode,
                                          const char* assembly);
extern void Chip8_PrintMemoryAssembly(Chip8* chip8, uint16_t opcode,
                                      const char* assembly);
extern void Chip8_PrintIndexAssembly(Chip8* chip8, uint16_t opcode,
                                     const char* assembly);