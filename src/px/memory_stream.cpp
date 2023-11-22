//
// Created by pyxiion on 22.10.23.
//

#include <cstring>
#include <fstream>
#include "memory_stream.hpp"

namespace px {
  MemoryStream::MemoryStream() = default;
  MemoryStream::~MemoryStream() = default;

  MemoryStream::MemoryStream(const MemoryStream &) = default;
  MemoryStream &MemoryStream::operator=(const MemoryStream &) = default;

  MemoryStream::MemoryStream(MemoryStream &&) noexcept = default;

  MemoryStream &MemoryStream::operator=(MemoryStream &&) noexcept = default;

  void MemoryStream::append(const void *data, std::size_t size) {
    if (data && (size > 0)) {
      const std::size_t start = m_data.size();
      m_data.resize(start + size);
      std::memcpy(&m_data[start], data, size);
    }
  }

  void MemoryStream::clear() {
    m_data.clear();
    reset();
  }

  void MemoryStream::reset() {
    m_readPos = 0;
    m_isValid = true;
  }

  void MemoryStream::loadFromFile(const std::string &filename) {
    clear();

    std::ifstream ifs(filename, std::ios::in | std::ios::binary);

    std::size_t size;
    ifs.seekg(0, std::ios::end);
    size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    m_data.resize(size);
    ifs.read(reinterpret_cast<char *>(m_data.data()), size);
  }

  std::size_t MemoryStream::readPosition() const {
    return m_readPos;
  }

  const void *MemoryStream::data() const {
    return m_data.empty() ? nullptr : m_data.data();
  }

  std::size_t MemoryStream::size() const {
    return m_data.size() * sizeof(decltype(m_data[0]));
  }

  bool MemoryStream::eof() const {
    return m_readPos >= m_data.size();
  }

  MemoryStream::operator bool() const {
    return m_isValid;
  }

  MemoryStream &MemoryStream::operator>>(bool &data) {
    std::uint8_t value;
    if (*this >> value)
      data = (value != 0);

    return *this;
  }

  MemoryStream &MemoryStream::operator>>(float &data) {
    if (checkSize(sizeof(data))) {
      std::memcpy(&data, &m_data[m_readPos], sizeof(data));
      m_readPos += sizeof(data);
    }
    return *this;
  }

  MemoryStream &MemoryStream::operator>>(double &data) {
    if (checkSize(sizeof(data))) {
      std::memcpy(&data, &m_data[m_readPos], sizeof(data));
      m_readPos += sizeof(data);
    }
    return *this;
  }

  MemoryStream &MemoryStream::operator>>(char *data) {
    std::uint32_t length{};
    *this >> length;

    if (length > 0 && checkSize(length)) {
      std::memcpy(data, &m_data[m_readPos], length);
      data[length] = '\0';

      m_readPos += length;
    }
    return *this;
  }

  MemoryStream &MemoryStream::operator>>(std::string &data) {
    std::uint32_t length{};
    *this >> length;

    data.clear();
    if (length > 0 && checkSize(length)) {
      data.assign(reinterpret_cast<char*>(&m_data[m_readPos]), length);

      m_readPos += length;
    }
    return *this;
  }

  MemoryStream &MemoryStream::operator>>(wchar_t *data) {
    std::uint32_t length{};
    *this >> length;

    if (length > 0 && checkSize(length * sizeof(std::uint32_t))) {
      for (std::uint32_t i = 0; i < length; i++) {
        std::uint32_t character{};
        *this >> character;
        data[i] = static_cast<wchar_t>(character);
      }
      data[length] = L'\0';
    }
    return *this;
  }

  MemoryStream &MemoryStream::operator>>(std::wstring &data) {
    std::uint32_t length{};
    *this >> length;

    data.clear();
    if (length > 0 && checkSize(length * sizeof(std::uint32_t))) {
      for (std::uint32_t i = 0; i < length; i++) {
        std::uint32_t character{};
        *this >> character;
        data += static_cast<wchar_t>(character);
      }
    }
    return *this;
  }
  template<Integer T>
  MemoryStream &MemoryStream::operator>>(T &data) {
    if (checkSize(sizeof(data))) {
      std::memcpy(&data, &m_data[m_readPos], sizeof(data));
      data = px::toNetworkEndianness(data);
      m_readPos += sizeof(data);
    }

    return *this;
  }
  // define definitions
  template MemoryStream &MemoryStream::operator>>(std::int8_t &data);
  template MemoryStream &MemoryStream::operator>>(std::int16_t &data);
  template MemoryStream &MemoryStream::operator>>(std::int32_t &data);

  template MemoryStream &MemoryStream::operator>>(std::int64_t &data);
  template MemoryStream &MemoryStream::operator>>(std::uint8_t &data);
  template MemoryStream &MemoryStream::operator>>(std::uint16_t &data);
  template MemoryStream &MemoryStream::operator>>(std::uint32_t &data);


  template MemoryStream &MemoryStream::operator>>(std::uint64_t &data);

  MemoryStream &MemoryStream::operator<<(bool data) {
    *this << static_cast<std::uint8_t>(data);
    return *this;
  }

  MemoryStream &MemoryStream::operator<<(float data) {
    append(&data, sizeof(data));
    return *this;
  }

  MemoryStream &MemoryStream::operator<<(double data) {
    append(&data, sizeof(data));
    return *this;
  }

  MemoryStream &MemoryStream::operator<<(const char *data) {
    // length
    const auto length = static_cast<std::uint32_t>(std::strlen(data));
    *this << length;

    // then characters
    append(data, length * sizeof(char));
    return *this;
  }

  MemoryStream &MemoryStream::operator<<(const std::string &data) {
    // length
    const auto length =  static_cast<std::uint32_t>(data.size());
    *this << length;

    // then characters
    if (length > 0)
      append(data.c_str(), length * sizeof(std::string::value_type));

    return *this;
  }

  MemoryStream &MemoryStream::operator<<(const wchar_t *data) {
    // length
    const auto length = static_cast<std::uint32_t>(std::wcslen(data));
    *this << length;

    // then characters
    for (const wchar_t* c = data; *c != L'\0'; ++c)
      *this << static_cast<std::uint32_t>(*c);

    return *this;
  }

  MemoryStream &MemoryStream::operator<<(const std::wstring &data) {
    // length
    const auto length = static_cast<std::uint32_t>(data.size());
    *this << length;

    // then characters
    if (length > 0)
    {
      for (const wchar_t c : data)
        *this << static_cast<std::uint32_t>(c);
    }

    return *this;
  }

  bool MemoryStream::checkSize(std::size_t size) {
    m_isValid = m_isValid && (m_readPos + size <= m_data.size());
    return m_isValid;
  }

  template<Integer T>
  MemoryStream &MemoryStream::operator<<(T data) {
    data = px::toNetworkEndianness(data);
    append(&data, sizeof(data));
    return *this;
  }
  // define definitions
  template MemoryStream &MemoryStream::operator<<(std::int8_t data);
  template MemoryStream &MemoryStream::operator<<(std::int16_t data);
  template MemoryStream &MemoryStream::operator<<(std::int32_t data);
  template MemoryStream &MemoryStream::operator<<(std::int64_t data);

  template MemoryStream &MemoryStream::operator<<(std::uint8_t data);
  template MemoryStream &MemoryStream::operator<<(std::uint16_t data);
  template MemoryStream &MemoryStream::operator<<(std::uint32_t data);
  template MemoryStream &MemoryStream::operator<<(std::uint64_t data);

} // px

std::ofstream &operator<<(std::ofstream &ofs, const px::MemoryStream &ms) {
  if (ofs.is_open()) {
    auto size = ms.size();
    ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
    ofs.write(static_cast<const char *>(ms.data()), size);
  }

  return ofs;
}

std::ifstream &operator>>(std::ifstream &ifs, px::MemoryStream &ms) {
  if (ifs.is_open()) {
    std::size_t size;
    ifs >> size;

    ms.clear();
    std::vector<char> data(size);
    ifs.read(data.data(), data.size());
    ms.append(data.data(), data.size());
  }

  return ifs;
}
