#include "info_window.hpp"
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
  m_engine.onPostUpdate.append(std::bind(&DebugInfoWindow::onUpdate, this, std::placeholders::_1));
  m_engine.onPostTick.append(std::bind(&DebugInfoWindow::onTick, this, std::placeholders::_1));
  m_engine.onGuiDraw.append(std::bind(&DebugInfoWindow::onGuiDraw, this));
  EASY_BLOCK("px::DebugInfoWindow::DebugInfoWindow")
  m_engine.onInit.append([this](auto &engine) {

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
  if (m_secondTimer > 1.0f)
  {
    m_secondTimer = 0;
    graphUpdate = true;
  }

  ImGui::Begin("Debug Window");
  {
    // Main information
    ImGui::Text("Version: %s", px::info::engineVersion.c_str());
    ImGui::Text("Build: %d", px::info::engineBuild);

    ImGui::Separator();
    
    // Sys information
    ImGui::Text("Mem used: %lldMB", px::getUsedMemory() / 1024);

    ImGui::Separator();

    // FPS and TPS
    float frameDeltaTime = m_frameDeltaTime;
    float tickDeltaTime = m_tickDeltaTime;
    
    float fps = 1.0f / frameDeltaTime;
    float tps = 1.0f / tickDeltaTime;

    ImGui::Text("FPS: %.0f", fps);
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Ms: %.4f", frameDeltaTime);

    drawGraph("FPS graph", m_frames, fps, graphUpdate);
    
    ImGui::Text("TPS: %.0f", tps);
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Ms: %.4f", tickDeltaTime);

    drawGraph("TPS graph", m_ticks, tps, graphUpdate);

    // World section
    World *world = m_engine.getWorld();
    ImGui::BeginDisabled(world == nullptr);
    if (ImGui::CollapsingHeader("World objects") && world)
    {
      // 
      if (ImGui::Button("Create new object"))
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
        ImGui::Text("Selected object address: 0x%p", static_cast<void*>(m_worldSelectedObject));
        ImGui::Text("Selected object name: %s", m_worldSelectedObject->getName().c_str());

        if (ImGui::Button("Destroy"))
          m_worldSelectedObject->destroy();
      }
    }
    ImGui::EndDisabled();

    if (ImGui::CollapsingHeader("Controls"))
    {
      Controls &controls = m_engine.getControls();

      ImGui::Text("Vertical: %f", controls.getAxis(ControlAxis::Vertical));
      ImGui::Text("Horizontal: %f", controls.getAxis(ControlAxis::Horizontal));
    }
  }
  ImGui::End();
}

void px::DebugInfoWindow::drawGraph(const char *label, std::vector<float> &graph, float currentValue, bool update)
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
  char overlay[32];
  sprintf(overlay, "Average: %.1f", average);

  ImGui::PlotLines(label, graph.data(), graph.size(), 0, overlay, 0.0f, max * 1.2f, ImVec2(300, 100), 4);
}
