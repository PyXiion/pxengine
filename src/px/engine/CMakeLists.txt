include("${PROJECT_SOURCE_DIR}/cmake/shaders.cmake")

set(PX_ENGINE_SHADER_SOURCES
    shaders/fs_cubes.sc
    shaders/vs_cubes.sc

    shaders/fs_mesh.sc
    shaders/vs_mesh.sc

    common/imgui/fs_imgui_image.sc
    common/imgui/vs_imgui_image.sc

    common/imgui/fs_ocornut_imgui.sc
    common/imgui/vs_ocornut_imgui.sc
)
set(PX_ENGINE_SOURCES
    engine.cpp engine.hpp
    general.hpp
    settings.hpp

    3d/cube.cpp 3d/cube.hpp

    common/controls.cpp common/controls.hpp
    common/dynamic_enum.cpp common/dynamic_enum.hpp
    common/format.hpp
    common/frame_limiter.cpp common/frame_limiter.hpp
    common/utils.cpp common/utils.hpp

    common/imgui/imgui.cpp common/imgui/imgui.hpp

    components/component.cpp components/component.hpp

    events/event.hpp
    events/event_fabric.hpp
    events/event_manager.cpp events/event_manager.hpp

    graphics/camera.cpp graphics/camera.hpp
    graphics/renderer.cpp graphics/renderer.hpp

    math/matrix.cpp math/matrix.hpp
    math/vector.cpp math/vector.hpp

    resources/bgfx_handle.hpp
    resources/resource_manager.cpp resources/resource_manager.hpp

    system/general.cpp system/general.hpp
    system/keycodes.hpp
    system/window.cpp system/window.hpp

    ui/debug/info_window.cpp ui/debug/info_window.hpp

    utils/bgfx_utils.cpp utils/bgfx_utils.hpp

    world/game_object.cpp world/game_object.hpp
    components/transform.cpp components/transform.hpp
    world/world.cpp world/world.hpp
    resources/localization.cpp
    resources/localization.hpp
    components/graphics/sprite_render.cpp
    components/graphics/sprite_renderer.hpp
    graphics/image.cpp
    graphics/image.hpp
    graphics/texture.cpp
    graphics/texture.hpp
    ui/widgets.hpp
    graphics/mesh.cpp
    graphics/mesh.hpp
    graphics/vertex.hpp
    graphics/shader.cpp
    graphics/shader.hpp
    ui/settings_window.cpp
    ui/settings_window.hpp
    graphics/model.cpp
    graphics/model.hpp
    components/graphics/model_renderer.cpp
    components/graphics/model_renderer.hpp
    graphics/render_states.cpp
    graphics/render_states.hpp
    components/transform.cpp
    components/transform.hpp
    events/common/mouse_event.hpp
    events/common/key_event.hpp
)

foreach(SHADER ${PX_ENGINE_SHADER_SOURCES})
  add_bgfx_shader(${SHADER} ${PX_ENGINE_SHADERS_DIR})
endforeach()
source_group("Shader Files" FILES ${PX_ENGINE_SHADER_SOURCES})

add_library(px_engine
    ${PX_ENGINE_SOURCES}
    ${PX_ENGINE_SHADER_SOURCES}
)

find_package(Angelscript REQUIRED)
find_package(eventpp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(fmt REQUIRED)
find_package(easy_profiler REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

target_link_libraries(px_engine easy_profiler)
target_link_libraries(px_engine fmt)
target_link_libraries(px_engine angelscript)
target_link_libraries(px_engine yaml-cpp)
target_link_libraries(px_engine glfw GL)
target_link_libraries(px_engine glm::glm)
target_link_libraries(px_engine imgui)
target_link_libraries(px_engine eventpp::eventpp)
target_link_libraries(px_engine bgfx bx)
target_link_libraries(px_engine assimp)
target_link_libraries(px_engine px)