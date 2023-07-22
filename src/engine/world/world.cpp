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

