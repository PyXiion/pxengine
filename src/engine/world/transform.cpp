#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
