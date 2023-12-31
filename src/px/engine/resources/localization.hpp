// Copyright (c) 2023.

//
// Created by pyxiion on 19.11.23.
//

#ifndef PX_ENGINE_LOCALIZATION_HPP
#define PX_ENGINE_LOCALIZATION_HPP
#include <unordered_map>
#include <string>
#include "resource_traits.hpp"

namespace px {

  class Localization {
  public:
    Localization() = default;

    void loadLanguage(std::istream &input);

    const std::string &operator[](const std::string &key) const;

    const std::string &get(const std::string &key) const;
    const char *getc(const std::string &key) const;

  private:
    std::unordered_map<std::string, std::string> m_dict;
  };

  namespace resources {
    template<>
    struct Traits<Localization> {
      static std::vector<std::string> extensions;

      static Resource<Localization> load(std::istream &is);
    };

  }
} // px

#endif //PX_ENGINE_LOCALIZATION_HPP
