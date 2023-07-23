#pragma once
#include <glm/glm.hpp>

namespace px
{
  class Transform
  {
  public:
    Transform(glm::vec3 position, glm::vec3 eulerAngles);

    const glm::mat4 &getTransformMatrix();

  private:
    glm::mat4 m_transform;
    bool m_isCached;

    glm::vec3 m_position;
    glm::vec3 m_rotation;

    void calculate();
  };
}
