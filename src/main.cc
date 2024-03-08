#pragma once
#include <cstdlib>
#include <SDL.h>
#include <ctype.h>
#include "imgui.h"
#include "include/chip8_names.h"
#include "include/chip8_emulation.h"
#include "include/imgui_config.h"

int main(int argc, char** argv){
  CHIP8_LOG_INFO("Launching Chip8 Engine in _DEBUG mode!\n");
  // ROM/test_opcode.ch8
  // ROM/IBM Logo.ch8
  // ROM/3-corax+.ch8
  // ROM/tetris.rom
  // ROM/breakout.rom
 // if (Chip8_Emulate("ROM/tetris.rom") == EXIT_FAILURE) {
 //   CHIP8_LOG_INFO("FAILED TO EMULATE\n");
 // }
  Chip8GUI::CreateGUI();
  return EXIT_SUCCESS;
}