// Copyright (c) 2023.

//
// Created by pyxiion on 05.11.23.
//

#include "gtest/gtest.h"
#include "../src/px/string_utils.hpp"

TEST(StringUtilsTest, StringSplit) {
  std::string str1 = "Hello world, Hello world!";
  auto splitted = px::split(str1, " ");

  EXPECT_EQ(splitted.size(), 4);

  std::vector<std::string> expected = {
      "Hello",
      "world,",
      "Hello",
      "world!"
  };

  for (std::size_t i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected[i], splitted[i]);
  }
}