// Copyright (c) 2023.

#include "transform.hpp"
#include "px/engine/common/imgui/imgui.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../engine.hpp"

px::Transform::Transform(px::Vector3 position, px::Vector3 eulerAngles)
  : m_transform()
  , m_isCached(false)
  , m_position(position)
  , m_rotation(eulerAngles)
{
}

void px::Transform::move(Vector3 offset)
{
  m_isCached = false;
  m_position += offset;
}

void px::Transform::rotate(Vector3 angles)
{
  m_isCached = false;
  m_rotation += angles;
}

void px::Transform::setPosition(Vector3 position)
{
  m_isCached = false;
  m_position = position;
}

void px::Transform::setRotation(Vector3 eulerAngles)
{
  m_isCached = false;
  m_rotation = eulerAngles;
}

px::Vector3 px::Transform::getRotation() const
{
  return m_rotation;
}

px::Vector3 px::Transform::getPosition() const
{
  return m_position;
}

const glm::mat4 &px::Transform::getTransformMatrix()
{
  if (!m_isCached)
    calculate();
  return m_transform;
}

void px::Transform::calculate()
{
  m_isCached = true;

  m_transform = glm::mat4(1.0f);
  m_transform = glm::translate(m_transform, m_position);

  m_transform = glm::rotate(m_transform, 1.0f, m_rotation);
}

void px::Transform::guiEditor() {
  bool changed = false;
  changed |= ImGui::InputVector3("Position", m_position);
  changed |= ImGui::InputVector3("Rotation", m_rotation);

  if (ImGui::SmallButton("Teleport to the camera")) {
    auto camera = getGameObject()->getEngine()->getCamera();
    setPosition(camera->getPosition());
    changed = true;
  }

  m_isCached = m_isCached and not changed;
}
