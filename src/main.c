#pragma once
#define _CRT_SECURE_NO_WARNNINGS
#include <stdlib.h>
#include <SDL.h>
#include <ctype.h>
#include "include/chip8_processor.h"
#include "include/chip8_instruction_set.h"
#include "include/sdl_config.h"
int main(int argc, char** argv){
	Chip8 chip8;
	Register _register;
	chip8._register = &_register;
	Chip8InitializeRegisters(&chip8);
	Chip8InitializeMemory(&chip8);
	Chip8ReadFile(&chip8, "ROM/Bowling [Gooitzen van der Wal].ch8");
	SDLStart(&chip8);
	return EXIT_SUCCESS;
}