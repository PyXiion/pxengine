//
// Created by pyxiion on 29.10.23.
//

#ifndef PX_EXCEPTION_HPP
#define PX_EXCEPTION_HPP

#include <exception>

namespace px {

  class Exception : public std::exception {
  public:
    inline explicit Exception(const char *msg = nullptr)
      : m_msg(msg) {}

    [[nodiscard]] inline const char *what() const noexcept override {
      return m_msg ? m_msg : "Unknown exception";
    }

  private:
    const char *m_msg;
  };

} // px

#endif //PX_EXCEPTION_HPP
