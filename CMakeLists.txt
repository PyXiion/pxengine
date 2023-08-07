cmake_minimum_required(VERSION 3.22)

project(engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(
  ext/glad/include
  ext/args
  ext/
)

add_subdirectory(ext/glad)
add_subdirectory(ext/imgui)

add_subdirectory(src/engine)
add_subdirectory(src/game)

install(TARGETS game
  CONFIGURATIONS Debug
  RUNTIME DESTINATION bin)
