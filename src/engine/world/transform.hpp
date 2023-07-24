#pragma once
#include "../math/vector.hpp"
#include "../math/matrix.hpp"

namespace px
{
  class Transform
  {
  public:
    Transform(Vector3 position, Vector3 eulerAngles);

    const glm::mat4 &getTransformMatrix();

  private:
    Matrix4x4 m_transform;
    bool m_isCached;

    Vector3 m_position;
    Vector3 m_rotation;

    void calculate();
  };
}
