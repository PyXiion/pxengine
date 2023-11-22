//
// Created by pyxiion on 03.11.23.
//

#include "gtest/gtest.h"
#include "../src/px/raw_memory.hpp"

// Test case for read() function
TEST(RawConstMemoryTest, ReadTest) {
  int data[5] = {5, 3, 2, 6, 2};
  px::RawConstMemory<int> memory(data, 5);

  int readed[5];
  memory.read(readed, 0, 5);

  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(readed[i], data[i])
              << "  i = " << i;
  }
}

// Test case for begin() function
TEST(RawConstMemoryTest, BeginTest) {
  int data[5] = {};
  px::RawConstMemory<int> memory(data, 5);
  const int *begin = memory.begin();

  ASSERT_EQ(begin, &data[0]);
}

// Test case for end() function
TEST(RawConstMemoryTest, EndTest) {
  int data[5] = {};
  px::RawConstMemory<int> memory(data, 5);
  const int *end = memory.end();

  ASSERT_EQ(end, &data[5]);
}

// Test case for size() function
TEST(RawConstMemoryTest, SizeTest) {
  int data[5] = {};
  px::RawConstMemory<int> memory(data, 5);

  ASSERT_EQ(memory.size(), 5);
}

// Test case for constructor function
TEST(RawConstMemoryTest, TypeConverationTest) {
  int data[5] = {6, 3, 8, 3, 2};
  const char *cdata = reinterpret_cast<const char *>(&data);
  px::RawConstMemory<int> memory(data, 5);

  char readed[5 * sizeof(int)];
  px::RawConstMemory<char> cmemory(memory);
  cmemory.read(readed, 0, 5 * sizeof(int));

  for (int i = 0; i < 5 * sizeof(int); i++) {
    EXPECT_EQ(cdata[i], readed[i])
              << "  i = " << i;
  }
}

// Test case for constructor function
TEST(RawConstMemoryTest, TypeConverationTest2) {
  const int N = 5;
  const int readedN = N * sizeof(int) / sizeof(double);

  int data[N] = {6, 3, 8, 3, 2};
  auto cdata = reinterpret_cast<const double *>(&data);
  px::RawConstMemory<int> memory(data, N);

  double readed[readedN];
  px::RawConstMemory<double> cmemory(memory);
  cmemory.read(readed, 0, readedN);

  for (int i = 0; i < readedN; i++) {
    EXPECT_DOUBLE_EQ(cdata[i], readed[i])
              << "  i = " << i;
  }
}