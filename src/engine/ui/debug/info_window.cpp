#include "info_window.hpp"
#include <numeric>

#include <imgui/imgui.h>

#include "../../engine.hpp"

px::DebugInfoWindow::DebugInfoWindow(px::Engine &engine)
  : m_engine(engine)
  , m_secondTimer(0.0f)
{
  m_engine.onPostUpdate.append(std::bind(&DebugInfoWindow::onUpdate, this, std::placeholders::_1));
  m_engine.onPostTick.append(std::bind(&DebugInfoWindow::onTick, this, std::placeholders::_1));
  m_engine.onGuiDraw.append(std::bind(&DebugInfoWindow::onGuiDraw, this));

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
  bool graphUpdate = false;
  if (m_secondTimer > 1.0f)
  {
    m_secondTimer = 0;
    graphUpdate = true;
  }

  ImGui::Begin("Debug Window");
  {
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
  }
  ImGui::End();
}

void px::DebugInfoWindow::drawGraph(const char *label, std::vector<float> &graph, float currentValue, bool update)
{
  const float maxOffset = 5.0f;
  if (update)
  {
    if (graph.size() >= kGraphPointCount)
    {
      graph.erase(graph.begin());
    }
    graph.push_back(currentValue);
  }

  float average = std::reduce(graph.begin(), graph.end()) / static_cast<float>(graph.size());
  char overlay[32];
  sprintf(overlay, "Average: %.1f", average);

  ImGui::PlotLines(label, graph.data(), graph.size(), 0, overlay, 0.0f, average + maxOffset, ImVec2(300, 100), 4);
}
