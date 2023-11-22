set(imgui_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
include_directories("${imgui_INCLUDE_DIRS}")
add_library(imgui
  include/imgui/imconfig.h
  imgui.cpp
  include/imgui/imgui.h
  imgui_demo.cpp
  imgui_draw.cpp
  include/imgui/imgui_internal.h
  imgui_tables.cpp
  imgui_widgets.cpp
  include/imgui/imstb_rectpack.h
  include/imgui/imstb_textedit.h
  include/imgui/imstb_truetype.h
)
