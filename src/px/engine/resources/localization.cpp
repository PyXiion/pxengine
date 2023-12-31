// Copyright (c) 2023.

//
// Created by pyxiion on 19.11.23.
//

#include "localization.hpp"
#include <filesystem>
#include <stack>
#include <yaml-cpp/yaml.h>
#include <fmt/format.h>
#include <easy/profiler.h>
#include <easylogging++.h>
#include "resource_traits.hpp"

namespace px {
  void Localization::loadLanguage(std::istream &input) {
    EASY_BLOCK("Localization::loadLanguage")
    using std::filesystem::path;
    YAML::Node root = YAML::Load(input);

    struct NodeAndPath {
      YAML::Node node;
      std::vector<std::string> path;
    };
    std::stack<NodeAndPath> nodes;
    nodes.push({root, {}});

    CLOG(INFO, "PXEngine") << "Loading a localization config:";
    while (not nodes.empty()) {
      auto [node, path] = nodes.top();
      nodes.pop();

      // iterate children
      for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
        auto key = it->first.as<std::string>();
        auto &value = it->second;

        // append current path
        path.push_back(key);
        if (value.IsMap()) {
          nodes.push({value, path});
        } else {
          std::string dictKey = fmt::format("{}", fmt::join(path, "."));
          auto str = value.as<std::string>();
          m_dict[dictKey] = str;

          CLOG(INFO, "PXEngine") << "\t" << dictKey << " = \"" << str << "\"";

          // return to our map
        }
        // return to parent
        path.pop_back();
      }
    }

    CLOG(INFO, "PXEngine") << "Loaded " << m_dict.size() << " strings";
  }

  const std::string &Localization::operator[](const std::string &key) const {
    auto it = m_dict.find(key);
    if (it == m_dict.end()) {
      CLOG(ERROR, "PXEngine") << "Access to a non-existent localization key " << key;
      throw std::out_of_range(fmt::format("There's no \"{}\" key in localization files.", key));
    }

    return it->second;
  }

  const std::string &Localization::get(const std::string &key) const {
    return (*this)[key];
  }

  const char *Localization::getc(const std::string &key) const {
    return (*this)[key].c_str();
  }

  std::vector<std::string> resources::Traits<Localization>::extensions {
      ".yml", ".yaml"
  };
  Resource<Localization> resources::Traits<Localization>::load(std::istream &is) {
    auto loc = makeResource<Localization>();
    loc->loadLanguage(is);
    return loc;
  }
} // px