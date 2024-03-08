# CMakeList.txt : CMake project for chip8_engine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
# Thought I needed this, kept it anyway
find_package(SDL2_mixer REQUIRED)
# for some reason, if I use the project function, CMake cannot find the version
# of SD2 , even when explicitly written, so it is commented out until I find
# out how to fix that.
#
#project("chip8_engine")

# Add source to this project's executable.
add_executable (chip8_engine
                                        "src/main.cc"
  "src/include/chip8_processor.h"       "src/chip8_processor.c" 
  "src/include/chip8_instruction_set.h" "src/chip8_instruction_set.c"
  "src/include/chip8_load_ROM.h"        "src/chip8_load_ROM.c"
  "src/include/chip8_operators.h"       "src/chip8_operators.c"
  "src/include/sdl_config.h"            "src/sdl_config.c"
  "src/include/chip8_names.h"
  "src/include/chip8_emulation.h"       "src/chip8_emulation.c"  "src/imgui_config.cc" "src/include/imgui_config.h")

# Include directory for header files
target_include_directories(chip8_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against SDL2 library
target_link_libraries(chip8_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(chip8_engine SDL2_image::SDL2_image)
target_link_libraries(chip8_engine SDL2_mixer::SDL2_mixer)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chip8_engine PROPERTY CXX_STANDARD 20)
endif()

# Move the resource folder into the exe directory
file(COPY resources DESTINATION ${DST_DIR}/)
# TODO: Add tests and install targets if needed

# Dear ImGui
# Change the path to where imgui is installed
set(IMGUI_DIR "G:/dev/SDK/imgui")
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)