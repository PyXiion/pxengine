//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_MEMORY_STREAM_HPP
#define PX_MEMORY_STREAM_HPP
#include <vector>
#include <string>
#include <cstdint>
#include "exports.hpp"
#include "serializable.hpp"
#include "system.hpp"

namespace px {

  class PX_API MemoryStream {
  public:
    MemoryStream();
    ~MemoryStream();

    MemoryStream(const MemoryStream &);
    MemoryStream& operator=(const MemoryStream &);

    MemoryStream(MemoryStream &&) noexcept;
    MemoryStream& operator=(MemoryStream &&) noexcept;

    void append(const void *data, std::size_t size);

    void clear();
    void reset();

    void loadFromFile(const std::string &filename);

    [[nodiscard]] std::size_t readPosition() const;
    [[nodiscard]] const void* data() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool eof() const;

    explicit operator bool() const;

    // Overload of operator >>
    // read from memory stream
    MemoryStream &operator>>(bool &data);

    template <Integer T>
    MemoryStream &operator>>(T &data);

    template <Integer T, std::size_t N>
    MemoryStream &operator>>(T data[N]) {
      for (int i = 0; i < N; i++)
        *this >> data[i];
      return *this;
    }

    MemoryStream &operator>>(float &data);
    MemoryStream &operator>>(double &data);

    MemoryStream &operator>>(char *data);
    MemoryStream &operator>>(std::string &data);

    MemoryStream &operator>>(wchar_t *data);
    MemoryStream &operator>>(std::wstring &data);

    inline MemoryStream &operator>>(Serializable &data) {
      data.deserialize(*this);
      return *this;
    }

    // Overload of operator <<
    // write to memory stream
    MemoryStream &operator<<(bool data);

    template <Integer T>
    MemoryStream &operator<<(T data);

    template <Integer T, std::size_t N>
    MemoryStream &operator<<(const T data[N]) {
      for (int i = 0; i < N; i++)
        *this << data[i];
      return *this;
    }

    MemoryStream &operator<<(float data);
    MemoryStream &operator<<(double data);

    MemoryStream &operator<<(const char *data);
    MemoryStream &operator<<(const std::string &data);

    MemoryStream &operator<<(const wchar_t *data);
    MemoryStream &operator<<(const std::wstring &data);

    inline MemoryStream &operator<<(Serializable &data) {
      data.serialize(*this);
      return *this;
    }

  private:
    bool checkSize(std::size_t size);

    std::vector<std::byte> m_data;
    std::size_t            m_readPos{};
    bool                   m_isValid{true};
  };

} // px

std::ofstream &operator<<(std::ofstream &ofs, const px::MemoryStream &ms);
std::ifstream &operator>>(std::ifstream &ifs, px::MemoryStream &ms);

#endif //PX_MEMORY_STREAM_HPP
