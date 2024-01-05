//
// Created by pyxiion on 29.10.23.
//

#ifndef PX_EXCEPTION_HPP
#define PX_EXCEPTION_HPP

#include <exception>

#define PX_NEW_EXCEPTION(exception, parent)       \
class exception : public parent {                 \
public:                                           \
  explicit exception(std::string msg = "")   \
    : parent(std::move(msg)) {}                              \
}


namespace px {

  class Exception : public std::exception {
  public:
    inline explicit Exception(std::string msg = "")
      : m_msg(std::move(msg)) {}

    [[nodiscard]] const char *what() const noexcept override {
      return m_msg.empty() ? "Unknown exception" : m_msg.c_str();
    }

  private:
    std::string m_msg;
  };

} // px

#endif //PX_EXCEPTION_HPP
