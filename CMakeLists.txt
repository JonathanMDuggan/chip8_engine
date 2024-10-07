# CMakeList.txt : CMake project for chip8_engine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# change the path to where your imgui dir is located
set(IMGUI_DIR "dep/imgui")
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(NFD_DIR "dep/nativefiledialog/src")
include_directories(${NFD_DIR} "${NFD_DIR}/include")

#set(SDL2_DIR "dep/SDL2-2.30.0/cmake")
#set(SDL2_image_DIR "dep/SDL2_image-2.8.2/cmake")
#set(SDL2_mixer_DIR "dep/SDL2_mixer-2.8.0/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
## Thought I needed this, kept it anyway
find_package(SDL2_mixer REQUIRED)

# for some reason, if I use the project function, CMake cannot find the version
# of SD2 , even when explicitly written, so it is commented out.
#project("chip8_engine")

# Add source to this project's executable.
add_executable (chip8_engine
                                                  "src/main.cc"
  "src/include/chip8_processor.h"                 "src/chip8_processor.c" 
  "src/include/chip8_instruction_set.h"           "src/chip8_instruction_set.c"
  "src/include/chip8_load_ROM.h"                  "src/chip8_load_ROM.c"
  "src/include/chip8_operators.h"                 "src/chip8_operators.c"
  "src/include/sdl_config.h"                      "src/sdl_config.c"
  "src/include/chip8_names.h"                     
  "src/include/chip8_emulation.h"                 "src/chip8_emulation.c"  
  "src/imgui_config.cc"                           "src/include/imgui_config.h" 

  ${IMGUI_DIR}/imgui.h                            ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdl2.h         ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.h ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
                                                  ${IMGUI_DIR}/imgui_draw.cpp 
                                                  ${IMGUI_DIR}/imgui_demo.cpp
                                                  ${IMGUI_DIR}/imgui_tables.cpp
                                                  ${IMGUI_DIR}/imgui_widgets.cpp

  "${NFD_DIR}/include/nfd.h"                     "${NFD_DIR}/nfd_win.cpp"     
  "${NFD_DIR}/common.h"                           
  "${NFD_DIR}/nfd_common.c" 
)

# Include directory for header files
target_include_directories(chip8_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against SDL2 library
target_link_libraries(chip8_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(chip8_engine SDL2_image::SDL2_image)
target_link_libraries(chip8_engine SDL2_mixer::SDL2_mixer)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chip8_engine PROPERTY CXX_STANDARD 20)
endif()

file(COPY ${SDL2_image_DIR}/../lib/x64/SDL2_image.dll DESTINATION ${DST_DIR}/)
file(COPY ${SDL2_DIR}/../lib/x64/SDL2.dll DESTINATION ${DST_DIR}/)
file(COPY ${SDL2_mixer_DIR}/../lib/x64/SDL2_mixer.dll DESTINATION ${DST_DIR}/)


# Move the resource folder into the exe directory
file(COPY resources DESTINATION ${DST_DIR}/)
# TODO: Add tests and install targets if needed