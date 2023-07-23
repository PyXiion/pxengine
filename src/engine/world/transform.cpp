#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

px::Transform::Transform(glm::vec3 position, glm::vec3 eulerAngles)
  : m_transform()
  , m_isCached(false)
  , m_position(position)
  , m_rotation(eulerAngles)
{
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
}
