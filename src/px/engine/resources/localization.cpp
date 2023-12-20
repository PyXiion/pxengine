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

namespace px {
  Localization::Localization(const std::string &folderPath, const std::string &languageCode) {
    setFolder(folderPath);
    loadLanguage(languageCode);
  }

  void Localization::setFolder(const std::string &path) {
    m_folder = path;
  }

  void Localization::loadLanguage(const std::string &languageCode) {
    EASY_BLOCK("Localization::loadLanguage")
    using std::filesystem::path;
    YAML::Node root = YAML::LoadFile(path(m_folder) / (languageCode + ".yaml"));

    struct NodeAndPath {
      YAML::Node node;
      std::vector<std::string> path;
    };
    std::stack<NodeAndPath> nodes;
    nodes.push({root, {}});

    CLOG(INFO, "PXEngine") << "Localization (" << languageCode << "):";
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
    if (m_folder.empty())
      throw std::out_of_range("There's no localization.");

    auto it = m_dict.find(key);
    if (it == m_dict.end()) {
      CLOG(ERROR, "PXEngine") << "Access to a non-existent localisation key " << key;
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
} // px