#pragma once
#include "imgui.h"
#include "include/sdl_config.h"
#include "include/imgui_config.h"

void Chip8GUI::CreateGUI() {
  ImGui::Begin("Hello");
  ImGui::Button("Button");
  ImGui::End();

  ImGui::ShowDemoWindow();
}
