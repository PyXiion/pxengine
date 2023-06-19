cmake_minimum_required(VERSION 3.0)

project(engine)

add_executable(engine main.cpp)

install(TARGETS engine RUNTIME DESTINATION bin)
