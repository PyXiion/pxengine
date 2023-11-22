add_executable(00_helloworld
  main.cpp
)
target_link_libraries(00_helloworld px_engine)

include(../../cmake/resources.cmake)
copy_resource_folder(00_helloworld)