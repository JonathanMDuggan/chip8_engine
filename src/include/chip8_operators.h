#ifdef __cplusplus
extern "C" {
#endif
#pragma once
#include <SDL.h>

#include "chip8_processor.h"  // We are using the Chip8 typedef!
enum NibbleBitMask {
  kNibble4BitMask = 0xF000,
  kNibble3BitMask = 0x0F00,
  kNibble2BitMask = 0x00F0,
  kNibble1BitMask = 0x000F
};

// Bit manipulation functions
extern inline uint16_t Chip8_MoveNibbleRight(uint16_t value,
                                             uint8_t x_amount_of_places);
extern uint8_t Chip8ConvertInputToHex(uint16_t input);
extern uint8_t Chip8_ReadHexFromWord(uint16_t value, uint8_t pos);
extern inline uint8_t Chip8_ReadLoByteFromWord(uint16_t value);
extern inline uint8_t Chip8_ReadForthNibble(uint16_t value);
extern inline uint8_t Chip8_ReadThirdNibble(uint16_t value);
extern inline uint8_t Chip8_ReadSecondNibble(uint16_t value);
extern inline uint8_t Chip8_ReadFirstNibble(uint16_t value);
extern uint8_t Chip8_ReadBitFromByte(uint8_t value, uint8_t pos);
// Bitwise Operations

extern void Chip8_RegisterXBitwiseAndData(uint8_t* register_x,
                                          const uint8_t kData);

extern void Chip8_RegisterXBitwiseXorData(uint8_t* register_x,
                                          const uint8_t kData);

extern void Chip8_RegisterXBitwiseOrData(uint8_t* register_x,
                                         const uint8_t kData);

extern void Chip8_RegisterXEqualData(uint8_t* register_x, const uint8_t kData);
// Operations

extern void Chip8_RegisterXPlusData(uint8_t* register_x, const uint8_t kData,
                                    Chip8* chip8);

extern void Chip8_RegisterXMinusData(uint8_t* register_x, const uint8_t kData,
                                     Chip8* chip8);
// Higher Order Functions:

extern void Chip8_RegisterToRegisterOperation(Chip8* chip8, uint16_t memory,
                                              void (*operation)(uint8_t*,
                                                                uint8_t));
extern void Chip8_MemoryRead(Chip8* chip8, uint16_t memory,
                             void (*operation)(uint8_t*, uint8_t));
void Chip8_MemoryReadFlag(Chip8* chip8, uint16_t memory,
                          void (*operation)(uint8_t*, const uint8_t, Chip8*));
extern void Chip8_RegisterToRegisterOperationFlag(
    Chip8* chip8, uint16_t memory,
    void (*operation)(uint8_t*, const uint8_t, Chip8*));
// Random Number Generators
extern uint16_t Chip8_Read12bitFromWord(uint16_t value);
extern void Chip8_ProcessInput(Chip8* chip8, SDL_Keycode input);
extern void Chip8_ProcessRelease(Chip8* chip8, SDL_Keycode input);
extern uint8_t Chip8_GetRandom8bitNumber();
#ifdef __cplusplus
}
#endif