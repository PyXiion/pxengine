// Copyright (c) 2023.

//
// Created by pyxiion on 21.11.23.
//

#include <px/engine/engine.hpp>
#include "px/engine/graphics/model.hpp"
#include "px/engine/components/graphics/model_renderer.hpp"
#include "px/engine/events/common/key_event.hpp"
#include "px/engine/graphics/uniform.hpp"

class App : public px::EventListener {
public:
  explicit App(px::Engine &engine)
    : m_engine(engine) {
    listen(engine.onInit, &App::init);
    listen(engine.onUpdate, &App::update);
  }

  void init(px::Engine &engine) {
    u_time = px::makeUniform("u_time", bgfx::UniformType::Vec4);

    // create world
    auto &world = engine.createNewWorld();

    // new game object
    auto gameObject = world.createGameObject();
    gameObject->setName("Object");

    auto transform = gameObject->addComponent<px::Transform>();
    transform->setPosition({0, 0, 0});

    auto renderer = gameObject->addComponent<px::ModelRenderer>();
    renderer->setModel("example.model.cube");

    px::RenderStates states = engine.getRenderer().getDefaultRenderStates();
    states.shaderPtr = engine.getResourceManager().loadShader("core.shaders.vs_mesh", "core.shaders.fs_mesh");
    renderer->setRenderStates(states);

    m_camera = world.createGameObject<px::Camera>();
    m_camera->perspective(60.0f, 16.0f/9.0f);
    m_camera->setPosition(10, 0, 0);
    m_camera->lookAt(0, 0, 0);

    engine.setCamera(m_camera);

    // hide cursor
    engine.getEventManager().listen<px::KeyPressedEvent>(this, [&engine](const px::KeyPressedEventPtr &event) {
      if (event->code == px::KeyCode_Escape) {
        engine.getWindow().setCursorHidden(not engine.getWindow().isCursorHidden());
      }
    });

    m_controls = &engine.getControls();
  }

  void update(float dt) {
    m_time += dt;
    *u_time = m_time;

    if (m_controls and m_engine.getWindow().isCursorHidden()) {
      m_camera->updateRotation(
          m_controls->getAxis(px::ControlAxis::MouseX) * dt,
          m_controls->getAxis(px::ControlAxis::MouseY) * dt
          );
    }
  }

  void gui() {

  }

private:
  px::Engine &m_engine;

  px::Controls *m_controls{};

  px::CameraPtr m_camera;

  px::UniformPtr u_time;
  float m_time = 0;

  float m;
};

int main(int argc, char *argv[]) {
  START_EASYLOGGINGPP(argc, argv);

  px::Engine engine;
  App app(engine);

  engine.run();
}