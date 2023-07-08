cmake_minimum_required(VERSION 3.0)

project(engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(
  ext/glad/include
)


add_executable(engine
  # Main
  src/main.cpp

  # Engine
  src/engine/engine.hpp
  src/engine/engine.cpp

  # Common
  src/engine/common/dynamic_enum.hpp
  src/engine/common/dynamic_enum.cpp

  src/engine/common/frame_limiter.hpp
  src/engine/common/frame_limiter.cpp

  src/engine/common/matrix4x4.hpp
  src/engine/common/matrix4x4.cpp

  src/engine/common/vector2.hpp
  src/engine/common/vector2.cpp

  src/engine/common/vector3.hpp
  src/engine/common/vector3.cpp

  src/engine/common/utils.hpp
  src/engine/common/utils.cpp

  # Graphics
  src/engine/graphics/renderer.hpp

  src/engine/graphics/shader.hpp

  src/engine/graphics/vertex_array.hpp

  # Graphics OpenGL
  src/engine/graphics/gl/renderer.hpp
  src/engine/graphics/gl/renderer.cpp

  src/engine/graphics/gl/shader.hpp
  src/engine/graphics/gl/shader.cpp

  src/engine/graphics/gl/vertex_array.hpp
  src/engine/graphics/gl/vertex_array.cpp

  # Graphics Vulkan
  # TODO

  # Scripts
  src/engine/scripts/docgen.hpp
  src/engine/scripts/docgen.cpp

  src/engine/scripts/engine.hpp
  src/engine/scripts/engine.cpp

  src/engine/scripts/module.hpp
  src/engine/scripts/module.cpp

  src/engine/scripts/function.hpp
  src/engine/scripts/function.cpp

  src/engine/scripts/binding.hpp
  src/engine/scripts/binding.cpp

  ## Script addons
  src/engine/scripts/addons/scriptstdstring.h
  src/engine/scripts/addons/scriptstdstring.cpp
  # src/engine/scripts/addons/scriptstdstring_utils.cpp

  src/engine/scripts/addons/scriptbuilder.h
  src/engine/scripts/addons/scriptbuilder.cpp

  src/engine/scripts/addons/scriptarray.h
  src/engine/scripts/addons/scriptarray.cpp

  # System
  src/engine/system/window.hpp
  src/engine/system/window.cpp
)

add_subdirectory(ext/glad)

find_package(Angelscript REQUIRED)
find_package(glfw3 REQUIRED)
find_package(eventpp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(fmt REQUIRED)
find_package(easy_profiler REQUIRED)

target_link_libraries(engine easy_profiler)
target_link_libraries(engine fmt)
target_link_libraries(engine angelscript)
target_link_libraries(engine yaml-cpp)
target_link_libraries(engine glfw GL glad)
target_link_libraries(engine eventpp::eventpp)

install(TARGETS engine
  CONFIGURATIONS Debug
  RUNTIME DESTINATION bin)
