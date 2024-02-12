#pragma once
#include "chip8_processor.h" // We are using the Chip8 typedef!
enum NibbleBitMask {
  kNibble4BitMask = 0xF000, kNibble3BitMask = 0x0F00,
  kNibble2BitMask = 0x00F0, kNibble1BitMask = 0x000F
};

// Bit manipulation functions
extern inline uint16_t MoveNibbleRight(
  uint16_t value, uint8_t x_amount_of_places
);
extern inline uint8_t ReadLoByteFromWord(uint16_t value);
extern inline uint8_t ReadForthNibble(uint16_t value);
extern inline uint8_t ReadThirdNibble(uint16_t value);
extern inline uint8_t ReadSecondNibble(uint16_t value);
extern inline uint8_t ReadFirstNibble(uint16_t value);

// Bitwise Operations

extern void RegisterXBitwiseAndData(uint8_t*      register_x,
                                    const uint8_t kData);

extern void RegisterXBitwiseXorData(uint8_t*      register_x,
                                    const uint8_t kData);

extern void RegisterXBitwiseOrData(uint8_t*      register_x,
                                   const uint8_t kData);

extern void RegisterXEqualData(uint8_t*      register_x,
                               const uint8_t kData);
// Operations

extern void RegisterXPlusData(uint8_t*      register_x,
                              const uint8_t kData,
                              Chip8*        chip8);

extern void RegisterXMinusData(uint8_t*      register_x,
                               const uint8_t kData,
                               Chip8*        chip8);
// Higher Order Functions:

extern void Chip8RegisterToRegisterOperation(Chip8* chip8, uint16_t memory,
                                             void(*operation)(uint8_t*, uint8_t));
extern void Chip8MemoryRead(Chip8* chip8, uint16_t memory,
                            void(*operation)(uint8_t*, uint8_t));
void Chip8MemoryReadFlag(Chip8* chip8, uint16_t memory,
  void(*operation)(uint8_t*, const uint8_t, Chip8*));
extern void Chip8RegisterToRegisterOperationFlag(Chip8* chip8, uint16_t memory,
  void(*operation)(uint8_t*, const uint8_t, Chip8*));
// Random Number Generator
extern uint16_t Read12bitFromWord(uint16_t value);