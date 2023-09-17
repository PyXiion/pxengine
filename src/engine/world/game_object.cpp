#include "game_object.hpp"
#include "world.hpp"

void px::GameObject::destroy()
{
  m_world.destroyObject(m_self);
}

const std::string &px::GameObject::getName() const
{
  return m_name;
}

px::World &px::GameObject::getWorld()
{
  return m_world;
}

void px::GameObject::setName(const std::string &name)
{
  m_world.updateObjectName(*m_self, name); // вызывается перед m_name = name, так как оно получает оттуда прошлое имя
  m_name = name;
}