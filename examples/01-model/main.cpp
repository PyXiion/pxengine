// Copyright (c) 2023.

//
// Created by pyxiion on 21.11.23.
//

#include <px/engine/engine.hpp>
#include "px/macros.hpp"

class App : px::EventListener {
public:
  App(px::Engine &engine)
    : m_engine(engine) {
    listen(engine.onInit, &App::init);
    listen(engine.onDraw, &App::draw);
  }

  void init(px::Engine &engine) {
  }

  void draw() {
    std::cout << "Рисую пока рисуется" << std::endl;
  }

private:
  px::Engine &m_engine;
};

int main() {
  px::Engine engine;
  App app(engine);

  engine.run();
}