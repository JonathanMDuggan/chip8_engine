#pragma once
#include <stdlib.h>
#include <SDL.h>

#include "chip8_processor.h"
#include "chip8_instruction_set.h"
#include "sdl_config.h"

int main(int argc, char** argv) {
	uint16_t some_hex_number = 0x1834;
	uint8_t  myUInt8 = ReadFirstNibble(some_hex_number);
	sdlVideoStuff();
	printf("Value of uint8_t in hexadecimal: 0x%02X\n", myUInt8);
	return EXIT_SUCCESS;
}