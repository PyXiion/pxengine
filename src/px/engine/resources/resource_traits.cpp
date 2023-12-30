// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#include "resource_traits.hpp"
#include "px/string_utils.hpp"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace px {
  std::vector<std::string> parseId(const std::string &id) {
    return px::split(id, ".");
  }

  namespace resources {
    std::string getDefaultPath(const std::string &id, const std::vector<std::string> &extensions) {
      fs::path path;
      for (auto &&i : parseId(id)) {
        path /= i;
      }

      if (extensions.empty()) {
        return path;
      } else {
        for (auto &ext : extensions) {
          path.replace_extension(ext);
          if (fs::exists(path))
            return path;
        }
        throw std::runtime_error("Failed to find file");
      }
    }

    // text files
    template<>
    struct Traits<std::string> {
      inline static std::vector<std::string> extensions = {
          ".txt", ".text", ""
      };

      static Resource<std::string> load(std::ifstream &ifs) {
        return makeResource<std::string>(std::istreambuf_iterator<char>(ifs),
                                         std::istreambuf_iterator<char>());
      };
    };
  } // resources

} // px