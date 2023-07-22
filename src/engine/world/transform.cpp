#include "transform.hpp"

px::Transform::Transform(px::Vector3 position, Vector3 eulerAngles)
  : m_transform()
  , m_isCached(false)
  , m_position(position)
  , m_rotation(eulerAngles)
{
}

const px::Matrix4x4 &px::Transform::getTransformMatrix() const
{
  return m_transform;
}

void px::Transform::calculate()
{
  m_isCached = true;

  m_transform.reset();
  m_transform.rotate(m_rotation);
  m_transform.move(m_position);
}
