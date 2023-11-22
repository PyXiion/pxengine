project(google_tests)
add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src)

# adding the google_tests_run target
add_executable(google_tests_run
    system_test.cpp
    memory_stream_test.cpp
    bit_array.cpp
    threading_task_queue_test.cpp
    raw_memory_test.cpp
    string_utils_test.cpp
)

# linking google_tests_run with google_tests_run which will be tested
target_link_libraries(google_tests_run px)

target_link_libraries(google_tests_run gtest gtest_main)
