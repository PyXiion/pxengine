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
    px/script/angel_script.hpp px/script/angel_script.cpp
    px/script/common.hpp
    px/script/template/signatures.hpp
    px/script/template/names.hpp
    px/script/function.hpp
    px/script/addons/scriptbuilder/scriptbuilder.cpp
    px/script/addons/scriptstdstring/scriptstdstring.cpp
    px/script/addons/scriptstdstring/scriptstdstring_utils.cpp
    px/script/addons/scriptarray/scriptarray.cpp
    px/script/module.cpp
    px/script/module.hpp
    px/script/module_builder.cpp
    px/script/module_builder.hpp
    px/script/function.cpp
    px/script/object_type_builder.cpp
    px/script/object_type_builder.hpp
    px/utils/static_warning.hpp
    px/px.cpp
    px/script/object_type.cpp
    px/script/object_type.hpp
    px/script/as_counter.hpp
    px/script/as_counter.cpp
    px/script/object_type_builder.hpp
    px/script/exceptions.hpp
    px/script/common.cpp
    px/script/addons/scripttypeof/c_script_type_of.cpp
    px/script/addons/scripttypeof/c_script_type_of.hpp
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

add_library(px ${PX_LIB_SOURCES})

find_package(Angelscript REQUIRED)

target_link_libraries(px angelscript easyloggingpp)

add_subdirectory(px/engine)