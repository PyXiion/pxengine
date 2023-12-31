// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#include "resource_traits.hpp"
#include <string>
#include <filesystem>
#include <easylogging++.h>
#include "px/string_utils.hpp"

namespace fs = std::filesystem;

namespace px {
  std::vector<std::string> parseId(const std::string &id) {
    return px::split(id, ".");
  }

  namespace resources {
    void setCurrentResourceId(const std::string &value) {
      currentResourceId = value;
    }
    const std::string &getCurrentResourceId() {
      return currentResourceId;
    }

    std::string getDefaultPath(const std::string &root, const std::string &id, const std::vector<std::string> &extensions) {
      fs::path path = root;
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
        CLOG(ERROR, "PXEngine") << "Failed to find a resource file (" << path << ")";
        throw std::runtime_error("Failed to find file");
      }
    }

    // text files
    std::vector<std::string> Traits<std::string>::extensions = {
        "", ".txt", ".text", ".log", ".msg"
    };

    Resource<std::string> Traits<std::string>::load(std::ifstream &ifs) {
      return makeResource<std::string>(std::istreambuf_iterator<char>(ifs),
                                       std::istreambuf_iterator<char>());
    }

    // binary files
    std::vector<std::string> Traits<std::vector<char>>::extensions = {
        "", ".bin", ".ttf"
    };

    Resource<std::vector<char>> Traits<std::vector<char>>::load(std::istream &is) {
      return makeResource<std::vector<char>>(std::istreambuf_iterator<char>(is),
                                             std::istreambuf_iterator<char>());
    }
  }

} // px