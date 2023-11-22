//
// Created by pyxiion on 29.10.23.
//
#include "gtest/gtest.h"
#include "../src/px/bit_array.hpp"

TEST(BitArrayTest, BitReferenceConversion) {
  px::BitArray<8> arr;
  arr[0] = true;
  arr[1] = false;

  bool bit0 = static_cast<bool>(arr[0]);
  bool bit1 = static_cast<bool>(arr[1]);

  EXPECT_TRUE(bit0);
  EXPECT_FALSE(bit1);
}

TEST(BitArrayTest, BitReferenceAssignment) {
  px::BitArray<16> arr;
  arr[0] = true;
  arr[1] = false;

  EXPECT_TRUE(static_cast<bool>(arr[0]));
  EXPECT_FALSE(static_cast<bool>(arr[1]));

  arr[0] = false;
  arr[1] = true;

  EXPECT_FALSE(static_cast<bool>(arr[0]));
  EXPECT_TRUE(static_cast<bool>(arr[1]));
}

TEST(BitArrayTest, BitReferenceOutOfRangeAccess) {
  px::BitArray<8> arr;

  // Попытка доступа к недопустимому индексу должна вызывать исключение
  EXPECT_THROW(arr.at(8), std::out_of_range);
}

TEST(BitArrayTest, BitReferenceConstAccess) {
  const px::BitArray<8> arr;

  // Проверка константного доступа
  bool bit0 = static_cast<bool>(arr[0]);
  bool bit1 = static_cast<bool>(arr[1]);

  EXPECT_FALSE(bit0);
  EXPECT_FALSE(bit1);
}

TEST(BitArrayTest, BitArraySize) {
  px::BitArray<24> arr;
  px::BitArray<25> arr2;

  // Проверка размера массива
  EXPECT_EQ(sizeof(arr),  3);
  EXPECT_EQ(sizeof(arr2), 4);
}

TEST(BitArrayTest, BitReferenceAssignmentLargeArray) {
  constexpr std::size_t N = 8 * 3;
  auto &arr = *new px::BitArray<N>;

  for (std::size_t i = 0; i < N; ++i) {
    arr[i] = (i % 2 == 0);
  }

  for (std::size_t i = 0; i < N; ++i) {
    bool expectedValue = (i % 2 == 0);
    EXPECT_EQ(static_cast<bool>(arr[i]), expectedValue);
  }

  delete &arr;
}