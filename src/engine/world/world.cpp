#include "world.hpp"
#include <fmt/format.h>

px::World::World(px::Engine &engine)
  : m_engine(engine)
{
}

px::World::~World()
{
}

px::Engine &px::World::getEngine()
{
  return m_engine;
}

px::GameObjectPtr px::World::getObjectByName(const std::string &name)
{
  EASY_BLOCK("px::World::getObjectByName");
  auto it = m_gameObjectsByName.find(name);
  if (it != m_gameObjectsByName.end())
  {
    return it->second.lock();
  }
  throw std::out_of_range(fmt::format("Объект с именем \"{}\" не существует!", name));
}

const std::list<px::GameObjectPtr> &px::World::getAllGameObjects() const
{
  return m_gameObjects;
}

void px::World::updateObjectName(GameObjectPtr gameobject, const std::string newName)
{
  EASY_BLOCK("px::World::updateObjectName");
  if (m_gameObjectsByName.contains(newName))
  {
    throw std::runtime_error("Данное имя уже занято!");
  }

  std::string oldName = gameobject->getName();

  // удалить старое имя, если оно есть
  auto oldIterator = m_gameObjectsByName.find(oldName);
  if (oldIterator != m_gameObjectsByName.end())
    m_gameObjectsByName.erase(oldIterator);

  m_gameObjectsByName[newName] = gameobject;
}

void px::World::destroyObject(GameObjectIter &iterator)
{
  EASY_BLOCK("px::World::destroyObject");
  auto obj = *iterator;
  auto &name = obj->getName();
  
  m_gameObjectsByName.erase(name);
  m_gameObjects.erase(iterator);
}