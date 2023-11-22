// Copyright (c) 2023.

//
// Created by pyxiion on 05.11.23.
//

#ifndef ENGINE_STRING_UTILS_HPP
#define ENGINE_STRING_UTILS_HPP
#include <vector>
#include <string>

namespace px {

  inline std::vector<std::string> split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> result;
    std::size_t start{}, end{};

    while ((end = str.find(delimiter, start)) != std::string::npos) {
      std::string token;
      token = str.substr(start, end - start);
      result.push_back(std::move(token));

      start = end + delimiter.size();
    }
    result.push_back(str.substr(start));

    return result;
  }

  inline std::string join(const std::vector<std::string> &strs, const std::string &join) {
    std::string result;
    for (std::size_t i = 0; i < strs.size(); i++) {
      result += strs[i];
      if (i < strs.size() - 1)
        result += join;
    }
    return result;
  }

} // px

#endif //ENGINE_STRING_UTILS_HPP
