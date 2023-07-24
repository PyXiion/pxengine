file(GLOB_RECURSE PX_GAME_SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "magic/*.cpp"
)

include_directories(..)

add_executable(game
  ${PX_GAME_SOURCES}
)

target_link_libraries(game px_engine)
