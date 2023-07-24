#include <iostream>

#include <easy/profiler.h>
#include <args.hxx>
#include <engine/general.hpp>
#include <fmt/format.h>
#include "game.hpp"

const std::string argsDesc = "PXE - PyXiion's Engine v" PX_ENGINE_VERSION;
const std::string argsEpilog = "";

int main(int argc, char **argv)
{
  args::ArgumentParser parser(argsDesc, argsEpilog);
  args::HelpFlag help(parser, "help", "Показать это меню", {'h', "help"});
  args::CompletionFlag completion(parser, {"complete"});

  args::ValueFlag<std::string> profilerOut(parser, "profilerOut", "Выходной файл профайлера", {'p', "profiler"});
  
  try
  {
    parser.ParseCLI(argc, argv);
  }
  catch (const args::Completion &e)
  {
    std::cout << e.what();
    return EXIT_SUCCESS;
  }
  catch (const args::Help &)
  {
    std::cout << parser;
    return EXIT_SUCCESS;
  }
  catch (const args::ParseError &e)
  {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return EXIT_FAILURE;
  }

  profiler::setEnabled(profilerOut);

  px::Engine engine;
  px::Game game(engine);

  engine.run();

  if (profiler::isEnabled())
    profiler::dumpBlocksToFile(args::get(profilerOut).c_str());
  return EXIT_SUCCESS;
}
