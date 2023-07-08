#include <iostream>

#include <easy/profiler.h>
#include "engine/engine.hpp"
#include "engine/system/general.hpp"

int main()
{
  EASY_PROFILER_ENABLE;

  px::Engine engine;

  engine.run();

  profiler::dumpBlocksToFile("debug_profile.prof");
  return EXIT_SUCCESS;
}
