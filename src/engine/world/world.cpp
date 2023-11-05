#include "world.hpp"
#include <fmt/format.h>

px::World::World(px::Engine &engine)
  : m_engine(engine) {
}

px::World::~World() = default;

px::Engine &px::World::getEngine()
{
  return m_engine;
}

px::GameObjectPtr px::World::getObjectByName(const std::string &name) {
  EASY_BLOCK("px::World::getObjectByName");
  auto it = m_gameObjectsByName.find(name);
  if (it != m_gameObjectsByName.end())
  {
    return it->second.lock();
  }
  throw std::out_of_range(fmt::format("Объект с именем \"{}\" не существует!", name));
}

const std::list<px::GameObjectPtr> &px::World::getAllGameObjects() const {
  return m_gameObjects;
}

void px::World::updateObjectName(GameObjectPtr &gameObject, const std::string &newName) {
  EASY_BLOCK("px::World::updateObjectName");
  std::lock_guard lk(m_gameObjectsMutex);
  if (m_gameObjectsByName.contains(newName)) {
    throw std::runtime_error("Данное имя уже занято!");
  }

  std::string oldName = gameObject->getName();

  // удалить старое имя, если оно есть
  auto oldIterator = m_gameObjectsByName.find(oldName);
  if (oldIterator != m_gameObjectsByName.end())
    m_gameObjectsByName.erase(oldIterator);

  m_gameObjectsByName[newName] = gameObject;
}

void px::World::destroyObject(GameObjectIter &iterator) {
  EASY_BLOCK("px::World::destroyObject");
  // Get the object from it.
  auto obj = *iterator;
  auto &name = obj->getName();

  // Lock objects.
  std::lock_guard lk(m_gameObjectsMutex);

  // Remove the object from the list so that it can be deleted if there are no references to it.
  m_gameObjects.erase(iterator);

  // Remove its name from dict.
  m_gameObjectsByName.erase(name);
}