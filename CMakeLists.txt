# CMakeList.txt : CMake project for chip8_engine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# For some reason, if I enable this, Cmake will not required any SDL2 version
# There is nothing online that I can find which explains this bug
# So I won't be using this function anymore.
# project ("chip8_engine")

# Add source to this project's executable.
add_executable (chip8_engine
                                        "src/main.c"
  "src/include/chip8_processor.h"       "src/chip8_processor.c" 
  "src/include/chip8_instruction_set.h" "src/chip8_instruction_set.c"
  "src/include/chip8_load_ROM.h"        "src/chip8_load_ROM.c"
  "src/include/chip8_operators.h"       "src/chip8_operators.c"
  "src/include/sdl_config.h"            "src/sdl_config.c"
  "src/include/chip8_names.h"
)

# Include directory for header files
target_include_directories(chip8_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against SDL2 library
target_link_libraries(chip8_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(chip8_engine SDL2_image::SDL2_image)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chip8_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed