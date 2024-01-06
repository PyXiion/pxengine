#include "game_object.hpp"
#include "world.hpp"

thread_local px::GameObject *px::GameObject::currentGameObject;

px::GameObject::GameObject()
  : m_world(px::World::current_world) {

}

void px::GameObject::destroy()
{
  m_world->destroyObject(m_self);
}

const std::string &px::GameObject::getName() const
{
  return m_name;
}

px::World *px::GameObject::getWorld()
{
  return m_world;
}

px::Engine *px::GameObject::getEngine() {
  return m_world ? &m_world->m_engine : nullptr;
}

void px::GameObject::setName(const std::string &name)
{
  m_world->updateObjectName(*m_self, name); // Вызывается перед m_name = name, так как
                                            // класс World должен удалить прошлое имя,
                                            // которое здесь хранится, из индексов
  m_name = name;
}

void px::GameObject::guiEditor() {}

bool px::GameObject::removeComponent(px::Component *componentToRemove) {
  if (m_components.empty())
    return false;

  for (const auto &component : m_components) {
    if (component.get() == componentToRemove) {
      m_components.remove(component);
      return true;
    }
  }
  return false;
}
