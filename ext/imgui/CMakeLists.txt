add_library(imgui
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui.cpp

  backend/imgui_impl_glfw.cpp
  backend/imgui_impl_opengl3.cpp
)