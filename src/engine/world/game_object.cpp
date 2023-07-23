#include "game_object.hpp"
#include "world.hpp"

void px::GameObject::destroy()
{
  m_world.destroyObject(m_iter);
}

px::World &px::GameObject::getWorld()
{
  return m_world;
}