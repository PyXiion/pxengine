#include "player.hpp"

px::Player::Player()
{
  auto &engine = getWorld().getEngine();

  listen(engine.onTick, [this](float dt) {
    onTick(std::forward<float>(dt));
  });
  listen(engine.onDraw, [this] {
    onDraw();
  });
}

px::Player::~Player() {
}

void px::Player::onTick(float dt)
{
}

void px::Player::onDraw()
{
}