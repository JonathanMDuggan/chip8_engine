#pragma once
#define CHIP8_APPLICATION_NAME "Chip8 Engine"
#define TRUE 1
#define FALSE 0
enum Chip8_WindowDefaults{
  kChip8DefaultWindowHeight = 320,
  kChip8DefaultWindowLength = 640 
};
// LOGGING MACROS:
// 
// enable if you want the console filled with useless infomation like mouse movement
// #define CHIP8_SDL_SHOW_MORE_LOG_INFO

#ifdef _DEBUG
  #define CHIP8_LOG_INFO(fmt, ...)        printf("\x1b[36m""[i] Chip8:""\x1b[0m " fmt, __VA_ARGS__)
  #define CHIP8_LOG_ERROR(fmt, ...)       printf("\x1b[31m""[-] Chip8:""\x1b[0m " fmt, __VA_ARGS__)
  #define CHIP8_LOG_SUCCESS(fmt, ...)     printf("\x1b[32m""[+] Chip8:""\x1b[0m " fmt, __VA_ARGS__)

// Shows what chip8 opcode is excuted during runtime
  #define CHIP8_LOG_INSTRUCTION(fmt, ...) printf("\x1b[33m""[ASM] Chip8:""\x1b[0m " fmt, __VA_ARGS__)


  #define CHIP8_SDL_LOG_INFO(fmt, ...)      printf("\x1b[36m""[i] SDL:""\x1b[0m " fmt, __VA_ARGS__)

// FOR MOUSE MOVEMENT
  #ifdef CHIP8_SDL_SHOW_MORE_LOG_INFO
    #define CHIP8_SDL_LOG_EXTRA_INFO(fmt, ...)  printf("\x1b[36m""[i] SDL:""\x1b[0m " fmt, __VA_ARGS__)
  #else
    #define CHIP8_SDL_LOG_EXTRA_INFO(fmt, ...)
  #endif 

  #define CHIP8_SDL_LOG_ERROR(fmt, ...)       printf("\x1b[31m""[-] SDL:""\x1b[0m " fmt, __VA_ARGS__)
  #define CHIP8_SDL_LOG_SUCCESS(fmt, ...)     printf("\x1b[32m""[+] SDL:""\x1b[0m " fmt, __VA_ARGS__)
#else // _DEBUG IS NOT DEFINED
  #define CHIP8_LOG_INSTRUCTION(fmt, ...)
  #define CHIP8_LOG_INFO(fmt, ...)
  #define CHIP8_LOG_ERROR(fmt, ...)
  #define CHIP8_LOG_SUCCESS(fmt, ...)

  #define CHIP8_SDL_LOG_INFO(fmt, ...)
  #define CHIP8_SDL_LOG_ERROR(fmt, ...)
  #define CHIP8_SDL_LOG_SUCCESS(fmt, ...)
#endif 