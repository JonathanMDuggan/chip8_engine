#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "include/sdl_config.h"
#include "include/imgui_config.h"
#include "include/chip8_emulation.h"
#include "imgui.h" 
void Chip8GUI::Create(SDL* sdl) { 
  Chip8_SDLInitialize(sdl);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplSDL2_InitForSDLRenderer(sdl->window, sdl->renderer);
  ImGui_ImplSDLRenderer2_Init(sdl->renderer);
  DefaultImGui();
}

void Chip8GUI::DefaultImGui() { 
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Regular.ttf", 16.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 7.0f));
}

void Chip8GUI::Quit(SDL* sdl) { 
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  Chip8_SDLQuit(sdl);
}

void Chip8GUI::Run(SDL* sdl) {
  uint8_t running = true;

  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
      ImGui_ImplSDL2_ProcessEvent(&e);
    }

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    Chip8GUI::MenuBar(sdl);
    ImGui::Render();
    SDL_RenderClear(sdl->renderer);

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(sdl->renderer);
  }
  Quit(sdl);
}

void Chip8GUI::MenuBar(SDL* sdl) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open", "CTRL O")) {
        nfdchar_t* outPath = NULL;
        nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
        if (result == NFD_OKAY) {
          puts("Success!");
          puts(outPath);
          Chip8_Emulate(outPath, sdl);
          free(outPath);
        } else if (result == NFD_CANCEL) {
          puts("User pressed cancel.");
        } else {
          printf("Error: %s\n", NFD_GetError());
        }
      }
      ImGui::EndMenu();
    }
  }
  ImGui::EndMainMenuBar();
}