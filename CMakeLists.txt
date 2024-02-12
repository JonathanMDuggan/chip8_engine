# CMakeList.txt : CMake project for chip8_engine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

find_package(SDL2 REQUIRED)

# For some reason, if I enable this, Cmake will not required any SDL2 version
# There is nothing online that I can find which explains this bug
# So I won't be using this function anymore.
# project ("chip8_engine")

# Add source to this project's executable.
add_executable (chip8_engine

                            "main.c"
  "chip8_processor.h"       "chip8_processor.c" 
  "chip8_instruction_set.h" "chip8_instruction_set.c"
  "chip8_load_ROM.h"        "chip8_load_ROM.c"
  "chip8_operators.h"       "chip8_operators.c"
  "sdl_config.h"            "sdl_config.c"

 "chip8_names.h")

# Include directory for header files
target_include_directories(chip8_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against SDL2 library
target_link_libraries(chip8_engine SDL2::SDL2 SDL2::SDL2main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chip8_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed