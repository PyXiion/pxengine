//
// Created by pyxiion on 22.10.23.
//
#include "gtest/gtest.h"
#include "../src/px/system.hpp"

TEST(SystemTest, EndianConvert) {
  std::int8_t i8 = 30;
  EXPECT_EQ(px::convertEndianness(i8),  30);

  std::int16_t i16 = 30;
  EXPECT_EQ(px::convertEndianness(i16), 7680);

  std::int32_t i32 = 30;
  EXPECT_EQ(px::convertEndianness(i32), 503316480);

  std::int64_t i64 = 30;
  EXPECT_EQ(px::convertEndianness(i64), 2161727821137838080LL);
}