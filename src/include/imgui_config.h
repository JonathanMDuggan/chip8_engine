#pragma once
#include "sdl_config.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "nfd.h"
namespace Chip8GUI{
extern void Create(SDL* sdl);
extern void DefaultImGui();
extern void Quit(SDL* sdl);
extern void Run(SDL* sdl);
extern void MenuBar(SDL* sdl);
}