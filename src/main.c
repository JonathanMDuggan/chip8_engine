#pragma once
#define _CRT_SECURE_NO_WARNNINGS
#include <stdlib.h>
#include <SDL.h>
#include <ctype.h>
#include "chip8_processor.h"
#include "chip8_instruction_set.h"
#include "sdl_config.h"

int main(int argc, char** argv){
	char input = 'a';
	
	SDLStart();
	printf("Press X to close window: ");

	for (;;) {
		scanf("%c", &input);
		if (toupper(input) != 'X'){
			continue;
		}
		break;
	}

	SDLCleanUp();
	return EXIT_SUCCESS;
}