//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_PX_HPP
#define PX_PX_HPP
#include <fmt/format.h>
#include <easylogging++.h>

#define PX_ANYTHING_TO_VOID_PTR(var) (const_cast<void*>(reinterpret_cast<const void*>(var)))

#define PX_NO_MOVE(classname)                        \
  classname(classname &&other) = delete;             \
  classname& operator=(classname &&other) = delete;

#define PX_NO_COPY(classname)                            \
  classname(const classname &other) = delete;            \
  classname& operator=(const classname &other) = delete;

#define PX_NO_COPY_NO_MOVE(classname)   \
  PX_NO_COPY(classname)                 \
                                        \
  PX_NO_MOVE(classname)

#define PX_THROW_AND_LOG(logger, errorType, message, ...) \
  do { auto msg = fmt::format(message, ##__VA_ARGS__); CLOG(ERROR, logger) << msg; throw errorType(msg); } while (0)


#endif //PX_PX_HPP
