#include "utils.hpp"
#include <fstream>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

std::string px::io::loadFileToString(const std::string &filename)
{
  if (!fs::exists(filename))
  {
    throw std::runtime_error(fmt::format("Не удалось прочитать файл {}, так как он не существует.", filename));
  }

  std::ifstream ifs(filename);

  return std::string(std::istreambuf_iterator<char>(ifs),
                     std::istreambuf_iterator<char>()  );
}
