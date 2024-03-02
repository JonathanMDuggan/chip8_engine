#pragma once
#include "chip8_processor.h"
#include "sdl_config.h"
#include "chip8_instruction_set.h"
#include <stdint.h>
extern uint8_t Chip8_Emulate(const char* file_name);
extern void Chip8_ProcessInstruction(Chip8* chip8, uint16_t opcode, 
                                     Chip8_OpcodeHandler* nnn,
                                     Chip8_OpcodeHandler* kk, 
                                     Chip8_OpcodeHandler* _8xy, 
                                     Chip8_OpcodeHandler* fx,
                                     Chip8_OpcodeHandler* xy, 
                                     Chip8_OpcodeHandler* x);

extern void Chip8_OpcodesStartsWith0(Chip8* chip8, uint16_t opcode);
extern void Chip8_Create8xyOpcodeTable(Chip8_OpcodeHandler* _8xy);
extern void Chip8_CreateNNNOpcodeTable(Chip8_OpcodeHandler* nnn);
extern void Chip8_CreateKKOpcodeTable(Chip8_OpcodeHandler* kk);
extern void Chip8_CreateXOpcodeTable(Chip8_OpcodeHandler* x);
extern void Chip8_CreateXYOpcodeTable(Chip8_OpcodeHandler* xy);
extern void Chip8_CreateFxOpcodeTable(Chip8_OpcodeHandler* fx);
extern void Chip8_PrintXYAssembly(Chip8* chip8, uint16_t opcode,
                                          const char* assembly);
extern void Chip8_PrintKKAssembly(Chip8* chip8, uint16_t opcode,
                                      const char* assembly);
extern void Chip8_PrintXAssembly(Chip8* chip8, uint16_t opcode,
                                     const char* assembly);
extern void Chip8_PrintNNNAssembly(Chip8* chip8, uint16_t opcode,
                                       const char* kAssembly);
extern uint8_t Chip8_IsKKOpcode(const uint8_t kIdentifier);
extern uint8_t Chip8_IsNNNOpcode(const uint8_t kIdentifier);
extern uint8_t Chip8_IsXYOpcode(const uint8_t kIdentifier);