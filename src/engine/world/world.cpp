#include "world.hpp"

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

void px::World::destroyObject(GameObjectIter &iterator)
{
  m_gameObjects.erase(iterator);
}

void px::World::destroyObject(GameObject &gameObject)
{
  destroyObject(gameObject.m_self);
}