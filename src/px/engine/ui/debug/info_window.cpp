#include "info_window.hpp"
#include <px/engine/common/imgui/imgui.hpp>

#include <numeric>
#include <imgui/imgui.h>

#include <easy/profiler.h>
#include "../../general.hpp"
#include "../../system/general.hpp"
#include "../../engine.hpp"

px::DebugInfoWindow::DebugInfoWindow(px::Engine &engine)
  : m_engine(engine)
  , m_secondTimer(0.0f)
  , m_worldSelectedObject(nullptr)
{
  EASY_BLOCK("px::DebugInfoWindow::DebugInfoWindow")
  m_engine.onInit.append([this](auto &engine) {
    m_localization = &engine.getResourceManager().loadLocalization("core.lang");
  });

  listen(m_engine.onPostUpdate, &DebugInfoWindow::onUpdate);
  listen(m_engine.onPostTick,   &DebugInfoWindow::onTick);
  listen(m_engine.onGuiDraw,    &DebugInfoWindow::onGuiDraw);

  m_frames.reserve(kGraphPointCount);
  m_ticks.reserve(kGraphPointCount);
}

void px::DebugInfoWindow::onUpdate(float dt)
{
  m_frameDeltaTime = dt;
}

void px::DebugInfoWindow::onTick(float dt)
{
  m_tickDeltaTime = dt;
  m_secondTimer += dt;
}

void px::DebugInfoWindow::onGuiDraw()
{
  EASY_BLOCK("px::DebugInfoWindow::onGuiDraw");
  bool graphUpdate = false;
  if (m_secondTimer > 0.3f) {
    m_secondTimer = 0;
    graphUpdate = true;
  }

  const std::string title = m_localization->get("ui.debug-window.title") + "##debugwindow";
  if (ImGui::Begin(title.c_str())) {
    // Main information
    ImGui::TextFmt(m_localization->get("ui.debug-window.version"), PX_ENGINE_VERSION);
    ImGui::TextFmt(m_localization->get("ui.debug-window.build"), PX_ENGINE_BUILD);

    ImGui::Separator();

    #pragma region Common
    if (ImGui::CollapsingHeader("Common")) {
      // Sys information
      ImGui::TextFmt(m_localization->get("ui.debug-window.mem-used"), px::getUsedMemory() / 1024);

      ImGui::Separator();

      // FPS and TPS
      float frameDeltaTime = m_frameDeltaTime;
      float tickDeltaTime = m_tickDeltaTime;

      float fps = 1.0f / frameDeltaTime;
      float tps = 1.0f / tickDeltaTime;

      ImGui::TextFmt(m_localization->get("ui.debug-window.graph.fps"), fps);
      if (ImGui::IsItemHovered())
        ImGui::SetTooltipFmt("Ms: {}", frameDeltaTime);

      drawGraph("FPS graph", m_frames, fps, graphUpdate);

      ImGui::TextFmt(m_localization->get("ui.debug-window.graph.tps"), tps);
      if (ImGui::IsItemHovered())
        ImGui::SetTooltipFmt("Ms: {}", tickDeltaTime);

      drawGraph("TPS graph", m_ticks, tps, graphUpdate);
    }
    #pragma endregion

    #pragma region World
    World *world = m_engine.getWorld();
    ImGui::BeginDisabled(world == nullptr);
    if (ImGui::CollapsingHeader(m_localization->getc("ui.debug-window.world-objects.title")) && world)
    {
      // 
      if (ImGui::Button(m_localization->getc("ui.debug-window.world-objects.create")))
      {
        world->createGameObject();
      }

      // Game objects lists
      bool isSelectedExists = false;
      if (ImGui::BeginListBox("##worldobjects"))
      {
        auto &gameObjects = world->getAllGameObjects();
        for (auto &obj : gameObjects)
        {
          const bool isSelected = obj.get() == m_worldSelectedObject;
          if (ImGui::Selectable(obj->getName().c_str(), isSelected))
            m_worldSelectedObject = obj.get();
          
          if (isSelected)
          {
            ImGui::SetItemDefaultFocus();
            isSelectedExists = true;
          }
        }

        ImGui::EndListBox();
      }

      // Selected game object info
      if (isSelectedExists)
      {
        ImGui::TextFmt(m_localization->getc("ui.debug-window.world-objects.selected.address"),
                       fmt::ptr(m_worldSelectedObject));
        ImGui::TextFmt(m_localization->getc("ui.debug-window.world-objects.selected.name"),
                       m_worldSelectedObject->getName().c_str());

        m_worldSelectedObject->guiEditor();

        ImGui::BeginGroup(); {
          auto components = m_worldSelectedObject->getComponents<Component>();
          for (size_t i = 0; i < components.size(); i++) {
            ImGui::PushID(i);
            components[i]->guiEditor();
            ImGui::PopID();
          }
        } ImGui::EndGroup();

        if (ImGui::Button(m_localization->getc("ui.debug-window.world-objects.selected.destroy")))
          m_worldSelectedObject->destroy();
      }
    }
    ImGui::EndDisabled();
    #pragma endregion

    #pragma region Controls
    if (ImGui::CollapsingHeader(m_localization->getc("ui.debug-window.controls")))
    {
      Controls &controls = m_engine.getControls();

      ImGui::Text("Vertical: %f", controls.getAxis(ControlAxis::Vertical));
      ImGui::Text("Horizontal: %f", controls.getAxis(ControlAxis::Horizontal));
    }
    #pragma endregion

    #pragma region Event IDs
    if (ImGui::CollapsingHeader(m_localization->getc("ui.debug-window.events.title"))) {
      EventManager &eventManager = m_engine.getEventManager();
      auto pairs = eventManager.getEventIds();

      ImGuiTableFlags flags = ImGuiTableFlags_RowBg;
      if (ImGui::BeginTable("##eventstable", 2, flags)) {
        ImGui::TableSetupColumn(m_localization->getc("ui.debug-window.events.string-id"));
        ImGui::TableSetupColumn(m_localization->getc("ui.debug-window.events.num-id"));
        ImGui::TableHeadersRow();

        std::for_each(pairs.rbegin(), pairs.rend(), [](auto pair) {
          auto [k, v] = pair;
          ImGui::TableNextRow();

          ImGui::TableSetColumnIndex(0);
          ImGui::Text("%s", k.c_str());

          ImGui::TableSetColumnIndex(1);
          ImGui::Text("%d", v);
        });

        ImGui::EndTable();
      }
    }
    #pragma endregion

    #pragma region Open things
    if (ImGui::CollapsingHeader(m_localization->getc("ui.debug-window.open.title"))) {
      if (ImGui::Button(m_localization->getc("ui.debug-window.open.settings"))) {
        m_engine.getSettingsWindow().setOpened(true);
      }
    }
    #pragma endregion

    ImGui::Separator();
  }
  ImGui::End();
}

void px::DebugInfoWindow::drawGraph(const char *label, std::vector<float> &graph, float currentValue, bool update) const
{
  if (update)
  {
    if (graph.size() >= kGraphPointCount)
    {
      graph.erase(graph.begin());
    }
    graph.push_back(currentValue);
  }

  float sum = std::reduce(graph.begin(), graph.end());
  float max = *std::max_element(graph.begin(), graph.end());
  float average = sum / static_cast<float>(graph.size());
  char overlay[64] {};
  fmt::format_to(overlay, fmt::runtime(m_localization->get("ui.debug-window.graph.average")), average);

  ImGui::PlotLines(label, graph.data(), graph.size(), 0, overlay, 0.0f, max * 1.2f, ImVec2(300, 100), 4);
}
