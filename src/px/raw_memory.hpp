//
// Created by pyxiion on 28.10.23.
//

#ifndef PX_RAW_MEMORY_HPP
#define PX_RAW_MEMORY_HPP

#include <cstring>
#include <stdexcept>
#include "exports.hpp"

namespace px {

  template <class T>
  class PX_API RawConstMemory {
  public:
    static_assert(not std::is_same_v<T, void>, "Type T cannot be void");
    static_assert(sizeof(T) > 0, "Type T cannot be nothing");

    explicit RawConstMemory(const void *data = nullptr, std::size_t sizeInBytes = 0)
        : m_data(reinterpret_cast<const T*>(data))
        , m_size(sizeInBytes / sizeof(T)) {}

    explicit RawConstMemory(const T *data = nullptr, std::size_t n = 0)
      : m_data(data)
      , m_size(n) {}

    explicit RawConstMemory(const T *begin, const T *end)
        : m_data(begin)
        , m_size(end - begin) {}

    template <class U>
    explicit RawConstMemory(const RawConstMemory<U> &other)
      : m_data(reinterpret_cast<const T*>(other.begin()))
      , m_size(other.size() * sizeof(U) / sizeof(T)) {};

    void read(T *data, std::size_t start, std::size_t n) const {
      if (not checkSize(start, n)) {
        throw std::out_of_range("Invalid range: Start + n exceeds the size of the memory.");
      }
      std::memcpy(data, m_data + start, n * sizeof(T));
    }

    [[nodiscard]] const T *data() const {
      return m_data;
    }
    [[nodiscard]] const T *begin() const {
      return m_data;
    }
    [[nodiscard]] const T *end() const {
      return m_data + m_size;
    }
    [[nodiscard]] std::size_t size() const {
      return m_size;
    }

  private:
    [[nodiscard]] bool checkSize(std::size_t start, std::size_t size) const {
      return (start + size) <= m_size;
    }

    const T    *m_data{};
    std::size_t m_size{};
  };

} // px

#endif //PX_RAW_MEMORY_HPP
