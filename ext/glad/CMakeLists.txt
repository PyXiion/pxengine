cmake_minimum_required(VERSION 3.22)

add_library(glad
  include/glad/glad.h
  src/glad.c

  include/KHR/khrplatform.h
)
