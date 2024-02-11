// Copyright (c) 2023.

//
// Created by pyxiion on 25.12.23.
//
#include <px/engine/engine.hpp>
#include <px/engine/components/graphics/sprite_renderer.hpp>

class App : public px::EventListener {
public:

  explicit App(px::Engine &engine)
    : m_engine(engine) {
    listen(engine.onInit, &App::init);
  }

  void init(px::Engine &engine) {
    listen(engine.onDraw, &App::draw);

    auto &world = engine.createNewWorld();

    auto camera = world.createGameObject<px::Camera>();
    engine.setCamera(camera);
    camera->setPosition(0, 0, 10);
    camera->lookAt(0, 0, 0);

    m_sprite = world.createGameObject();

    auto transform = m_sprite->addComponent<px::Transform>();
    transform->setPosition(0, 0, 0);

    m_sprite->addComponent<px::SpriteRenderer>("example.sprite.banana");
  }

  void draw() {

  }

private:
  px::Engine &m_engine;
  px::GameObjectPtr m_sprite;
};

int main(int argc, char *argv[]) {
  START_EASYLOGGINGPP(argc, argv);
  px::Engine engine;
  App app(engine);
  engine.run();
}