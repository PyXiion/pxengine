// Copyright (c) 2023.

//
// Created by pyxiion on 19.11.23.
//

#ifndef PX_ENGINE_LOCALIZATION_HPP
#define PX_ENGINE_LOCALIZATION_HPP
#include <unordered_map>
#include <string>

namespace px {

  class Localization {
  public:
    Localization() = default;
    Localization(const std::string &folderPath, const std::string &languageCode);

    void setFolder(const std::string &path);
    void loadLanguage(const std::string &languageCode);

    const std::string &operator[](const std::string &key) const;

    const std::string &get(const std::string &key) const;
    const char *getc(const std::string &key) const;

  private:
    std::string m_folder;
    std::unordered_map<std::string, std::string> m_dict;
  };

} // px

#endif //PX_ENGINE_LOCALIZATION_HPP
