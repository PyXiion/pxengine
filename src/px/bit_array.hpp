//
// Created by pyxiion on 29.10.23.
//

#ifndef PX_BIT_ARRAY_HPP
#define PX_BIT_ARRAY_HPP
#include <cstddef>
#include <cstdint>
#include "macros.hpp"

namespace px {

  template <std::size_t N>
  class BitArray {
    static_assert(N > 0, "N must be > 0");

    class BitReference {
    public:
      inline BitReference(std::byte &byte, std::uint8_t bit)
        : m_byte(byte)
        , m_bit(bit) {}

      inline explicit operator bool() const {
        return (bool)((m_byte >> m_bit) & (std::byte)1);
      }

      inline BitReference &operator=(bool value) {
        m_byte = (m_byte & ~((std::byte)1 << m_bit)) | ((std::byte)value << m_bit);
        return *this;
      }

    private:
      std::byte   &m_byte;
      std::uint8_t m_bit;
    };

    class ConstBitReference {
    public:
      inline ConstBitReference(const std::byte &byte, std::uint8_t bit)
          : m_byte(byte)
          , m_bit(bit) {}

      inline explicit operator bool() const {
        return (bool)((m_byte >> m_bit) & (std::byte)1);
      }

    private:
      const std::byte &m_byte;
      std::uint8_t     m_bit;
    };

  public:
    BitArray() {
      std::fill(std::begin(m_data), std::end(m_data), std::byte(0));
    }

    inline BitReference at(std::size_t bit) {
      if (bit >= N)
        throw std::out_of_range("Bit is out of range");
      std::size_t byte = bit / 8;
      return BitReference(m_data[byte], bit % 8);
    }

    inline ConstBitReference at(std::size_t bit) const {
      if (bit >= N)
        throw std::out_of_range("Bit is out of range");
      std::size_t byte = bit / 8;
      return ConstBitReference(m_data[byte], bit % 8);
    }

    inline BitReference operator[](std::size_t bit) {
      return at(bit);
    }
    inline ConstBitReference operator[](std::size_t bit) const {
      return at(bit);
    }

  private:
    std::byte m_data[PX_DIVISION_ROUND_UP(N, 8)]{};
  };
} // px

#endif //PX_BIT_ARRAY_HPP
