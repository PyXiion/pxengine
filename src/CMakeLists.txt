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
)

add_library(px ${PX_LIB_SOURCES})

add_subdirectory(px/engine)