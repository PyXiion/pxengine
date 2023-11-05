#pragma once
#include <engine/engine.hpp>

namespace px
{
  class Player : public GameObject, private EventListener
  {
  public:
    Player();
    ~Player() override;

  private:
    void onTick(float dt);

    void onDraw();
  };
}