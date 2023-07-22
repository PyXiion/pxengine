file(GLOB_RECURSE PX_ENGINE_SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "common/*.cpp"
  "graphics/*.cpp" "graphics/gl/*.cpp"
  "math/*.cpp"
  "scripts/*.cpp" "scripts/addons/*.cpp"
  "system/*.cpp"
  "world/*.cpp"
)

add_library(px_engine ${PX_ENGINE_SOURCES})

find_package(Angelscript REQUIRED)
find_package(eventpp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(fmt REQUIRED)
find_package(easy_profiler REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(px_engine easy_profiler)
target_link_libraries(px_engine fmt)
target_link_libraries(px_engine angelscript)
target_link_libraries(px_engine yaml-cpp)
target_link_libraries(px_engine glfw GL glad)
target_link_libraries(px_engine eventpp::eventpp)
