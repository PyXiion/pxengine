cmake_minimum_required(VERSION 3.22)

project(engine)

set(CMAKE_CXX_STANDARD 20)

set(PX_GAME_DIR "${PROJECT_SOURCE_DIR}/game")
set(PX_CORE_SHADERS_DIR "${PX_GAME_DIR}/data/core/shaders")

include_directories(
  ext/glad/include
  ext/include
)

add_subdirectory(ext/glad)
add_subdirectory(ext/include/imgui)
add_subdirectory(
    /home/pyxiion/CLionProjects/px
    ./libpx
)
add_subdirectory(
    /home/pyxiion/Projects/_EXT_/bgfx
    ./libbin
)

include_directories(/home/pyxiion/Projects/_EXT_/bgfx/bgfx/include)
include_directories(/home/pyxiion/Projects/_EXT_/bgfx/bx/include)

add_subdirectory(src/engine)
add_subdirectory(src/game)

install(TARGETS game
  CONFIGURATIONS Debug
  RUNTIME DESTINATION bin)
