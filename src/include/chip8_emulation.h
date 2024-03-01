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
extern void Chip8_Create8xyOpcodeTable(Chip8_OpcodeHandler table[]);
extern void Chip8_CreateNNNOpcodeTable(Chip8_OpcodeHandler opcode[]);
extern void Chip8_CreateKKOpcodeTable(Chip8_OpcodeHandler opcode[]);
extern void Chip8_CreateFxOpcodeTable(Chip8_OpcodeHandler fx[]);
extern void Chip8_PrintXYAssembly(Chip8* chip8, uint16_t opcode,
                                          const char* assembly);
extern void Chip8_PrintKKAssembly(Chip8* chip8, uint16_t opcode,
                                      const char* assembly);
extern void Chip8_PrintXAssembly(Chip8* chip8, uint16_t opcode,
                                     const char* assembly);
extern void Chip8_PrintNNNAssembly(Chip8* chip8, uint16_t opcode,
                                       const char* kAssembly);
extern void Chip8_OpcodeKK(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeNNN(Chip8* chip8, uint16_t opcode);
extern void Chip8_OpcodeStartsWith8(Chip8* chip8, uint16_t opcode);

extern void Chip8_Execute(
    Chip8* chip8, uint16_t opcode, uint8_t kIdentifier,
    const uint8_t kFunctionPointerArraySize,
    void (*CreateFunctionPointerArray)(Chip8_OpcodeHandler* mnemonic),
    Chip8_OpcodeHandler execute[], const uint8_t kNOP,
    void (*PrintAssembly)(Chip8* chip8, uint16_t opcode,
                          const char* kAssembly));
extern uint8_t Chip8_IsKKOpcode(const uint8_t kIdentifier);
extern uint8_t Chip8_IsNNNOpcode(const uint8_t kIdentifier);