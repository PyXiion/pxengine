// Copyright (c) 2023.

//
// Created by pyxiion on 12.11.23.
//

#include <px/engine/engine.hpp>
#include "px/macros.hpp"

int main() {
  px::Engine engine;

  engine.onGuiDraw.append([]{
    ImGui::Begin("Hello world window!");

    static char name[50] = "world\0";

    ImGui::InputText("Your name", name, 50);

    ImGui::Text("Hello, %s!", name);
    ImGui::Text(U8("Привет, %s!"), name);

    ImGui::End();
  });

  engine.run();
}