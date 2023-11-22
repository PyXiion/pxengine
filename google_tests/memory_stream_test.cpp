//
// Created by pyxiion on 22.10.23.
//
#include <cstring>
#include "gtest/gtest.h"
#include "../src/px/memory_stream.hpp"

// Fixture class for MemoryStream tests
class MemoryStreamTest : public ::testing::Test {
protected:
  px::MemoryStream stream;
};

TEST_F(MemoryStreamTest, Append) {
  const char *data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);

  EXPECT_EQ(stream.size(), size);
  EXPECT_FALSE(stream.eof());
}

TEST_F(MemoryStreamTest, Clear) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);
  stream.clear();

  EXPECT_EQ(stream.size(), 0);
  EXPECT_EQ(stream.readPosition(), 0);
  EXPECT_TRUE(stream.eof());
}

TEST_F(MemoryStreamTest, Reset) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);
  stream.reset();

  EXPECT_EQ(stream.size(), size);
  EXPECT_EQ(stream.readPosition(), 0);
  EXPECT_FALSE(stream.eof());
}

TEST_F(MemoryStreamTest, ReadPosition) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);

  EXPECT_EQ(stream.readPosition(), 0);

  // Read some data
  bool value;
  stream >> value;

  EXPECT_EQ(stream.readPosition(), sizeof(bool));
}

TEST_F(MemoryStreamTest, Data) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);

  const void* streamData = stream.data();

  EXPECT_EQ(std::memcmp(streamData, data, size), 0);
}

TEST_F(MemoryStreamTest, Size) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);

  EXPECT_EQ(stream.size(), size);
}

TEST_F(MemoryStreamTest, Eof) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream << data;

  EXPECT_FALSE(stream.eof());

  // Read all data
  char buffer[16];
  stream >> buffer;

  EXPECT_TRUE(stream.eof());
}

TEST_F(MemoryStreamTest, OperatorBool) {
  const char* data = "Hello, World!";
  std::size_t size = std::strlen(data);

  stream.append(data, size);

  EXPECT_TRUE(static_cast<bool>(stream));

  // Read all data
  char buffer[16];
  stream >> buffer;

  EXPECT_FALSE(static_cast<bool>(stream));
}

TEST_F(MemoryStreamTest, WriteRead) {
  const std::uint8_t  u8    =   128;
  const std::uint16_t u16   =   7'000;
  const std::uint32_t u32   =   150'354'000;
  const std::uint64_t u64   =   534'545'124'643'360;

  const char         *cstr  =   "Hello world from C!";
  const std::string   str   =   "Hello world from C++!";

  const wchar_t      *cwstr =  L"Hello world from C!";
  const std::wstring  wstr  =  L"Hello world from C++!";

  const float  f32          =  235.5325f;
  const double f64          =  5315734.3253;

  stream << u8 << u16
         << u32   << u64
         << cstr  << str
         << cwstr << wstr
         << f32   << f64;

  std::uint8_t  readed_u8;
  std::uint16_t readed_u16;
  std::uint32_t readed_u32;
  std::uint64_t readed_u64;

  char          readed_cstr[strlen(cstr)];
  std::string   readed_str;

  wchar_t       readed_cwstr[wcslen(cwstr)];
  std::wstring  readed_wstr;

  float  readed_f32;
  double readed_f64;

  stream >> readed_u8 >> readed_u16
         >> readed_u32   >> readed_u64
         >> readed_cstr  >> readed_str
         >> readed_cwstr >> readed_wstr
         >> readed_f32   >> readed_f64;

  EXPECT_EQ(u8,  readed_u8 );
  EXPECT_EQ(u16, readed_u16);
  EXPECT_EQ(u32, readed_u32);
  EXPECT_EQ(u64, readed_u64);

  EXPECT_STREQ(cstr,  readed_cstr);
  EXPECT_EQ   (str,   readed_str );

//  EXPECT_STREQ(wcstr,  readed_wcstr); // TODO check wcstring
  EXPECT_EQ(wstr,   readed_wstr );

  EXPECT_EQ(f32, readed_f32);
  EXPECT_EQ(f64, readed_f64);
}