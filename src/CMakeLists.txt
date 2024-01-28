set(PX_LIB_SOURCES
    px/binary_tree.hpp px/binary_tree.cpp
    px/bit_array.hpp
    px/exception.hpp
    px/exports.hpp
    px/hash_utils.hpp
    px/macros.hpp
    px/memory_stream.hpp px/memory_stream.cpp
    px/px.hpp
    px/raw_memory.hpp
    px/serializable.hpp
    px/string_utils.hpp
    px/system.hpp
    px/templates.hpp
    px/visit_overloaded.hpp

    px/threading/task_queue.hpp
    px/utils/constexpr_hash.hpp
    px/utils/static_warning.hpp
    px/px.cpp
    px/memory/ref_counting.cpp
    px/memory/ref_counting.hpp
    px/memory/ref.cpp
    px/memory/ref.hpp
    px/memory/weak_ref.cpp
    px/memory/weak_ref.hpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("DEBUG")
  add_compile_definitions(PX_DEBUG)
endif ()

add_compile_definitions(PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL)

add_library(px ${PX_LIB_SOURCES})

find_package(Angelscript REQUIRED)

target_link_libraries(px angelscript easyloggingpp)

add_subdirectory(px/engine)