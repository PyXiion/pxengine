add_executable(01_model
  main.cpp
)
target_link_libraries(01_model px_engine)

include(../../cmake/resources.cmake)
copy_resource_folder(01_model)