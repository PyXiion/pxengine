cmake_minimum_required(VERSION 3.22)

project(px_engine)

set(CMAKE_CXX_STANDARD 20)

set(PX_RESOURCES_DIR        "${PROJECT_SOURCE_DIR}/data")
set(PX_ENGINE_RESOURCES_DIR "${PX_RESOURCES_DIR}/core")
set(PX_ENGINE_SHADERS_DIR   "${PX_ENGINE_RESOURCES_DIR}/shaders")

include_directories(
  ext/include
  src
)

add_subdirectory(bgfx.cmake)

add_subdirectory(ext/imgui)
include_directories(ext/imgui/include)

add_subdirectory(src)
add_subdirectory(examples)

#install(TARGETS game
#  CONFIGURATIONS Debug
#  RUNTIME DESTINATION bin)
