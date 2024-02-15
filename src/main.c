#pragma once
#define _CRT_SECURE_NO_WARNNINGS
#include <stdlib.h>
#include <SDL.h>
#include <ctype.h>
#include "include/chip8_processor.h"
#include "include/chip8_instruction_set.h"
#include "include/sdl_config.h"

int main(int argc, char** argv){
	char input = 'a';
	char inputBuffer[2] = {'a', '\0'};
	SDLStart();
	printf("Press X to close window: ");

	for (;;) {
		if (sscanf(inputBuffer, "%c", &input) != 1) {
			continue;
		}
		if (toupper(input) != 'X'){
			continue;
		}
		break;
	}
	return EXIT_SUCCESS;
}