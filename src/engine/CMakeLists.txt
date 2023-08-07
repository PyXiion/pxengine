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

find_package(Angelscript REQUIRED) # https://clck.ru/35ENso
find_package(eventpp REQUIRED) # https://clck.ru/35ENxa
find_package(yaml-cpp REQUIRED) # https://clck.ru/35EP9f
find_package(fmt REQUIRED) # https://clck.ru/V5GiN
find_package(easy_profiler REQUIRED) # https://clck.ru/35ENq8
find_package(glfw3 3.3 REQUIRED) # https://clck.ru/35EPAi
find_package(glm REQUIRED) # https://clck.ru/35ENvv

target_link_libraries(px_engine easy_profiler)
target_link_libraries(px_engine fmt)
target_link_libraries(px_engine angelscript)
target_link_libraries(px_engine yaml-cpp)
target_link_libraries(px_engine glfw GL glad)
target_link_libraries(px_engine glm::glm)
target_link_libraries(px_engine imgui)
target_link_libraries(px_engine eventpp::eventpp)
